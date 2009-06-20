#!/usr/local/bin/perl

use strict;
use warnings;
use FindBin;
use lib "$FindBin::Bin/lib";

BEGIN {
    my @libs;
	push @libs, '/home/deonwu/lib/perl5';
    require lib;
    lib->import( @libs );
}

use CGI::Fast;
use HTML::Mason::CGIHandler;
use HTML::Entities;
                            
$::MASON_GLOBALS = [qw($schema $fb)]; # List of variable names to make global

%::MASON_CONFIG = (
    error_mode           => "output", # or fatal
    default_escape_flags => "h",
    );

my $handler;
while (my $cgi = CGI::Fast->new ) {
    unless ( $handler ) {
        # Untaint these two
        my $data_dir = $ENV{'MASON_DATA_ROOT'};
        if ( ! -d $data_dir ) {
            system("/bin/mkdir","-p",$data_dir);
        }
        $handler = HTML::Mason::CGIHandler->new(
            comp_root     => $ENV{'MASON_COMP_ROOT'},
            data_dir      => $data_dir,
            allow_globals => $::MASON_GLOBALS,
            %::MASON_CONFIG
            );
    }
    my($url) = ($ENV{'REQUEST_URI'}=~/(.*)/);
    $url =~ s{\?.*}{};
    $url =~ s{/ms}{};
    #$ENV{'REQUEST_URI'} = $url;
    eval {
        if ( ( !$handler->interp->comp_exists( $url ) )
             and ( $handler->interp->comp_exists( $url . "/index.html" ) ) ) {
            $url .= "/index.html";
        }
        
        $handler->_handler( { comp=>$url, cgi=>$cgi } );
	#$cgi->path_info = $url;
	#$handler->handle_cgi_object($cgi);
    };
    if ( my $raw_error = $@) {
        print $cgi->header();
        print "A really fatal error occured while processing your request:<br>\n";
        print "<pre>".encode_entities( $@ )."</pre>\n";
    }
}

exit;
