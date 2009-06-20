package MSFB;
use strict;
use Data::Dumper;
use Config::Any;
use base qw(WWW::Facebook::API);
use Log::Common;

my $access='/home/deonwu/access.log';
my $error='/home/deonwu/error.log';
sub new_client {
    my ($self, $cgi, $cfg_file) = @_;
	
	my $config = _load_config($cfg_file);
	
	my $self = $self->new(desktop     => 0,
            api_key     => $config->{facebook}->{'api_key'},
            secret      => $config->{facebook}->{'secret'},
            app_path    => $config->{facebook}->{'app_path'}
			);
	
	$self->{'log'} = Log::Common->new(access => $access,
					   error  => $error,
					   no_stderr => 1,
					   class => "debug");

	$self->query($cgi);
	my $auth_token = $cgi->param('auth_token');
	
	$self->info("new client auth token:$auth_token");
	$self->info("app config:$cfg_file");
	$self->info("api key:$config->{facebook}->{'api_key'}");
	$self->auth->get_session($auth_token) if $auth_token;

    return $self;
}

sub require_login {
	my ($self, ) = @_;
	
    return '<fb:redirect url="' . $self->get_app_url . '" /><!-- in_fb_cavans -->'
        unless $self->canvas->in_fb_canvas();	
}

sub require_add {
	
	my ($self, ) = @_;
	
    return '<fb:redirect url="' . $self->get_add_url . '" /><!-- get_user -->'
        unless $self->canvas->get_user();	
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
    return $config;
}

  
sub _init_log {
	my ($access, $error) = @_;
			   
}
