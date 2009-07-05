package SUGAR;
use Exporter 'import';
@EXPORT_OK = qw(get_quiz get_featured_list check_out_quiz load_sugar_user save_sugar_user get_sugar_list get_country_list);
use strict;
use Data::Dumper;

our $UPLOAD_ROOT = $ENV{UPLOAD_ROOT} || $ENV{APACHE_ROOT};
our $PHOTO_URL = $ENV{PHOTO_URL} || '/';

sub get_featured_list {
	my ($dbh, $count) = @_;
	my ($list) = $dbh->selectall_arrayref(q{SELECT a.fb_uid, a.gender, a.location, a.photo FROM
				ms_sugar_user a, ms_sugar_featured b 
				where a.fb_uid = b.fb_uid order by b.create_date desc limit 0, 10}, {Slice => {}});
	
	my $list_count = @{$list};
	
	return [] if !$list_count;
	
	for(my $i= $count - $list_count; $i > 0; $i--){
		push @{$list}, $list->[0];
	}
	
	return @{$list};
}

sub get_country_list {
	my ($dbh, $count) = @_;
	my ($list) = $dbh->selectall_arrayref(q{SELECT cid, name from country_list}, {Slice => {}});
	
	return $list;
}


sub get_sugar_list {
	my ($dbh, $count, $start, $limit) = @_;
	my ($list) = $dbh->selectall_arrayref(q{SELECT a.fb_uid, a.gender, a.location, photo FROM
				ms_sugar_user a 
				where joined = 1 order by a.join_date desc limit ?, ?}, {Slice => {}}, $start || 0, $limit || 10);

	return () if !$list;
	
	my $list_count = @{$list};
	if ($count > 0){
		for(my $i= $count - $list_count; $i > 0; $i--){
			push @{$list}, $list->[0];
		}
	}
	
	return @{$list};
}

sub get_quiz {
	my ($dbh, $quiz_no) = @_;
	my ($question) = $dbh->selectrow_array("SELECT question FROM ms_sugar_question WHERE order_by=?", {}, $quiz_no);
	return $question;
}

sub check_out_quiz {
	my ($dbh, $fb, $quiz_result) = @_;
	
	my @yes = grep m/Yes/i, values %{$quiz_result};
	my $yes_count = $#yes + 1;
	my $fb_user = $fb->canvas->get_user();
	
	#check the result category
	my ($category, $description) = $dbh->selectrow_array("SELECT usr_id, description FROM ms_sugar_answer where min_yes <= ? and max_yes >= ?", {}, 
														  $yes_count, $yes_count);
	#save facebook user id and quiz result.
	if (!$dbh->selectrow_array("SELECT 1 from ms_sugar_user where fb_uid = ?", {}, $fb_user)){
		$dbh->do("insert ms_sugar_user(fb_uid, sugar_category) values(?,?)", {}, $fb_user, $category) or die $dbh->errstr;		
	}else {
		$dbh->do("update ms_sugar_user set sugar_category = ? where fb_uid = ?", {}, $category, $fb_user) or die $dbh->errstr;
	}
		
	#update quiz totally result.
	$dbh->do("update ms_sugar_answer set user_count = user_count + 1 where usr_id = ?", {}, $category) or die $dbh->errstr;
		
	my $totally_result = $dbh->selectall_arrayref("SELECT description, user_count FROM ms_sugar_answer", {Slice => {}}) or die $dbh->errstr;
	
	my $totally_count = $dbh->selectrow_array("SELECT sum(user_count) FROM ms_sugar_answer", {});
	
	return ($description, $totally_result, $totally_count);
}

sub create_sugar_user {
	my ($dbh, $fb, $args) = @_;
}

sub load_sugar_user {
	my ($dbh, $fb, $fb_uid) = @_;
		
	$fb_uid ||= $fb->canvas->get_user() || 614332206;
	my $u = $dbh->selectrow_hashref("select fb_uid, gender, location, seeking, photo, description from ms_sugar_user where fb_uid=?", {}, $fb_uid);
	if (!($u and $u->{gender})){
		my $fql_result = $fb->fql->query(query=>qq(select name, current_location, pic_big from user where uid=$fb_uid));
		$u = pop @{$fql_result} if $fql_result;
		$u ||=  {};
		$u->{fb_uid} = $fb_uid;
		$u->{photo} = $u->{pic} = $u->{pic_big} ;
	}
	
	return $u;
}

sub save_sugar_user {
	my ($dbh, $fb, $cgi) = @_;
	
	#fb_uid is reserved by Facebook
	my $fb_uid = $cgi->param("sg_uid");
	my ($gender, $location, $seeking, $photo, $description);
	$gender = $cgi->param("gender");
	$location = $cgi->param("location");
	$seeking = $cgi->param("seeking");
	#$email = $cgi->param("email");
	#$im = $cgi->param("im");
	$photo = handle_uploaded_photo($cgi) || $cgi->param("pic");
	$description = $cgi->param("description");
	
	if ($dbh->selectrow_array("SELECT 1 from ms_sugar_user where fb_uid = ?", {}, $fb_uid)){
		$dbh->do(q(update ms_sugar_user  
				   set gender = ?, location=?, seeking=?, photo=?, description=?, joined=1, join_date=now()
				   where fb_uid = ?), 
				{}, $gender, $location, $seeking, $photo, $description, $fb_uid) or die $dbh->errstr;
	}else {  #new
		$dbh->do(q{insert ms_sugar_user(fb_uid, sugar_category, joined, gender, location, seeking, photo, description, join_date)
				  values(?, 0, 1, ?, ?, ?, ?, ?, now())}, 
				{}, $fb_uid, $gender, $location, $seeking, $photo, $description) or die $dbh->errstr;
	}
	my $u = $dbh->selectrow_hashref("select fb_uid, gender, location, seeking, photo, description from ms_sugar_user where fb_uid=?", {}, $fb_uid);
	
	return ("OK", $u);
	
}

sub handle_uploaded_photo {
	my ($cgi) = @_;
	
	my $fh = $cgi->upload('photo');
	return "" unless $fh;

	my @ext = split /\./, $cgi->param('photo');
	my $ext = pop @ext;
	my $fb_uid = $cgi->param("sg_uid");
	
	my $save_file = "$UPLOAD_ROOT/$fb_uid.$ext";
	my $uploded_url = "$PHOTO_URL/$fb_uid.$ext";
	
	#print "save photo:$save_file!";
	#print "uploded_url:$uploded_url!";
	
	open LOG, ">$save_file";
	while (<$fh>) {
      print LOG $_;
	}
	close LOG;
	close $fh;
	
	return $uploded_url;
}

1;
