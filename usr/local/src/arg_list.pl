#!/opt/BIOSTAT/bin/perl

use strict;
use warnings;
use diagnostics;
use Getopt::Std;

# Arg l is a boolean flag
# Arg f takes an argument
# Values stored in %opts
my %opts;
getopts('lf:', \%opts);

# Get base name of program by splitting full path name
# of program into an array and taking its last element
my $program_name=(split "/", $0)[-1];
print "program name=$program_name\n";

# Show option list
while( (my $key, my $value) = each(%opts) )
{
	print "option $key == $value\n";
}

# Create ARGC variable
my $argc=@ARGV;
print "argc == $argc\n";

# Show argument list
my $count=0;
foreach (@ARGV)
{
	$count+=1;
	print "arg$count == $_\n";
}

# Explicitly show first argument if there was one
print "First argument is $ARGV[0]\n" if @ARGV;

exit 0;
