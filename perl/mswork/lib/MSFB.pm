package MSFB;
use strict;
use Data::Dumper;
use Config::Any;
use base qw(WWW::Facebook::API);
use Log::Common;
use FindBin;

my $CLIENT_CACHE = {};

sub new_client {
    my ($self, $cgi, $app_name, $cfg_file) = @_;
	
	my $client = $CLIENT_CACHE->{$app_name};
	unless ($client) {
		my $config;
		($config, $cfg_file) = _load_config($cfg_file || "$FindBin::Bin/$app_name/app_config.xml");
		$client = $self->new(desktop     => 0,
	            api_key     => $config->{facebook}->{'api_key'},
	            secret      => $config->{facebook}->{'secret'},
	            app_path    => $config->{facebook}->{'app_path'}
				);
		
		$client->{'log'} = Log::Common->new(access => $config->{log}->{'access'},
						   error  => $config->{log}->{'error'},
						   no_stderr => 1,
						   class => "debug");
		
		$client->{'debug'} = $config->{debug};
		$client->info("create new FB API for app:$app_name");
		$client->info("config file:$cfg_file");
		$CLIENT_CACHE->{$app_name} = $client;
	}
	
	$client->query($cgi);
	
	if ($client->{'debug'}){
		$client->info("request app:$app_name");
		$client->info("request api key:" . $client->api_key());
		$client->info("request app_path:" . $client->app_path());
		$client->info("user canvas user:" . $client->canvas->get_user());
	}
	
    return $client;
}

sub required_login {
	my ($self, ) = @_;
	
	unless($self->canvas->get_user()){
		print '<fb:redirect url="' . $self->get_login_url(canvas=>'1') . '" />'; 
		return 1; 
	}
	
	return 0;
}

sub update_session {
	my ($self, $cgi) = @_;
	$self->query($cgi);
	my $auth_token = $cgi->param('auth_token'); #or $self->auth->create_token();
	
	$self->info("update request session");
	
	$self->info("cgi:$cgi");
	$self->info("new client auth token:$auth_token");
	$self->auth->get_session($auth_token) if $auth_token;
	
	$self->info("user id:" . $self->session_uid());
	$self->info("user canvas user:" . $self->canvas->get_user());
}

sub info {
	my ($self, $msg) = @_;
	$self->{'log'}->access(message=>$msg)
}

sub error {
	my ($self, $msg) = @_;
	$self->{'log'}->error(message=>$msg)
}

#static class method.
sub _load_config {
    my ($cfg_file) = @_;

    my $config;
    my $cfg = Config::Any->load_files({files=>["$cfg_file"], use_ext=>1});
    for (@$cfg) {
        my $filename;
        ($filename, $config) = each %$_;
    }
    return ($config, $cfg_file);
}

  
sub _init_log {
	my ($access, $error) = @_;
			   
}

1;