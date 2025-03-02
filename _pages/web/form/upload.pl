#!/usr/bin/perl

use strict;
use warnings;
use CGI;

# Create CGI object
my $cgi = CGI->new();

# Print Content-Type header
print "Content-Type: text/html\n\n";

# Directory to save uploaded files
my $upload_dir = $ENV{'UPLOAD_DIR'} . '/';

if (!$upload_dir || $upload_dir eq '')
{
    print STDERR "upload Forbidden\n";
    exit 1;
}

# Process each file upload
foreach my $key ($cgi->param()) {
    next unless $key =~ /^upload_file/;
    my $upload_filehandle = $cgi->upload($key);
    my $filename = $cgi->param($key);

    if ($upload_filehandle) {
        # Generate unique filename if necessary
        my $upload_path = $upload_dir . $filename;

        # Save the uploaded file if not open print message in stderr
        open my $fh, '>', $upload_path or die exit 1;
        while (<$upload_filehandle>) {
            print $fh $_;
        }
        close $fh;

        print "<p>File uploaded successfully: <a href=\"/uploads/$filename\">$filename</a></p>";
    } else {
        print "<p>Error uploading file: " . $cgi->cgi_error() . "</p>";
    }
}
