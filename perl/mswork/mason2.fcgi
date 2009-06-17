#!/usr/local/bin/perl
#
#use lib qw( /home/deonwu/lib/perl5 );
use FCGI;
use Socket qw( :crlf ); # server agnostic line endings in $CRLF
use HTML::Mason::CGIHandler;
my $counter = 0;
while ( FCGI::accept() >= 0 ) {
   $counter++;

   print
        "Content-Type: text/plain",
        $CRLF,
        $CRLF,
        "Hello World, in Perl FastCGI!",
        $CRLF,
        "I am process $$.",
        $CRLF,
        "I have served $counter request(s).",
        $CRLF;
}
exit; 
