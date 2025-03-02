#!/usr/bin/perl

package Session;

use strict;
use warnings;
use CGI;
use Digest::SHA qw(sha256_hex);
use Storable qw(store retrieve);

sub new {
    my ($class, %args) = @_;
    my $self = {
        id     => $args{id} || _generate_session_id(),
        data   => {},
    };

    mkdir 'sessions' unless -d 'sessions';

    bless $self, $class;
    $self->_load_session();

    if (!$self->{data}{expire}) {
        my $expiration_time = $args{expire} || 3600;
        $self->{data}{expire} = time() + $expiration_time;
        $self->_save_session();
    }

    return ($self);
}


sub _generate_session_id {
    my $session_id;
    my $session_file;

    do {
        $session_id = sha256_hex(time() . rand());
        $session_file = "sessions/" . $session_id . ".session";
    } while (-e $session_file);

    return ($session_id);
}

sub _load_session {
	my $self = shift;
	my $session_file = "sessions/" . $self->{id} . ".session";
	if (-e $session_file) {
		$self->{data} = retrieve($session_file);
	}
}

sub _save_session {
	my $self = shift;
	my $session_file = "sessions/" . $self->{id} . ".session";
	store($self->{data}, $session_file);
}


sub generate_cookie {
    my $self = shift;
    my $cookie = CGI->new->cookie(
        -name    => 'session_id',
        -value   => $self->{id},
        -path    => '/',
        -expires => $self->{data}{expire},
    );

    return $cookie;
}

sub set {
	my ($self, $key, $value) = @_;
	$self->{data}{$key} = $value;
	$self->_save_session();
}

sub get {
	my ($self, $key) = @_;
	return ($self->{data}{$key});
}

sub delete {
	my ($self, $key) = @_;
	delete($self->{data}{$key});
	$self->_save_session();
}

sub delete_session {
	my $self = shift;
	unlink("sessions/" . $self->{id} . ".session");
}

1;
