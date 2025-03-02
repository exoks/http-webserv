#!/usr/bin/perl
use lib '/Users/hael-mou/Desktop/webserv/_pages/auth-page/lib';
use strict;
use warnings;
use CGI;
use File::Slurp;
use Session;

my $cgi = CGI->new();

my $session_id = $cgi->cookie('session_id');
unless ($session_id)
{
	print $cgi->redirect(
		-url     => '/sign_in.pl',
		-status  => '302 Temporary Redirect',
	);
}
my $session  = Session->new(id => $session_id );
my $username = $session->get('username') || '';
my $password = $session->get('password') || '';

if ($ENV{'REQUEST_METHOD'} eq 'DELETE')
{
	my $cookie = CGI::Cookie->new(
		-name    => 'session_id',
		-value   => '',
		-path    => '/',
		-expires => 'Thu, 01 Jan 1970 00:00:00 GMT'
	);

	$session->delete_session();
	print $cgi->header( -status => '200 OK' );
	exit;
}

my $expires = $session->get('expire');
if ($username && $password && $expires && $expires > time())
{
	my $html_home = read_file('public/home.html');
	$html_home =~ s/<% USERNAME %>/$username/;

	print $cgi->header(-type => 'text/html');
	print $html_home;
	exit;
}

my $cookie = CGI::Cookie->new(
	-name    => 'session_id',
	-value   => '',
	-path    => '/',
	-expires => 'Thu, 01 Jan 1970 00:00:00 GMT'
);

print $cgi->redirect(
	-url     => '/sign_in.pl',
	-status  => '302 Temporary Redirect',
	-cookie  => $cookie,
);

$session->delete_session();
