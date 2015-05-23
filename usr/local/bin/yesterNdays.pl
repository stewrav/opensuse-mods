#!/opt/BIOSTAT/bin/perl
#
#  Program: yesterNdays
#  Authors: Randal L. Schwartz, and
#         : Lawson Hanson, 19961010
#  Purpose: Prints yester N day's date
#    Usage: yesterNdays #
#     e.g.: yesterNdays 5
#  Returns: Sat Oct  5 13:50:23 1996
#
#  Converted from sh to perl by S.G.Ravenhall
#  08Feb2002

# Find number of command line arguments
$ARGC=@ARGV;

# Show usage note
unless ( $ARGC == 1 )
{
	# Get short program name
	@path_name=split "/", $0;
	$program_name=$path_name[-1];

	print "Usage: $program_name #\ne.g.:  $program_name 5\n";
	exit 1;
}

print scalar localtime (time - $ARGV[0] * 86400)."\n";

exit 0
