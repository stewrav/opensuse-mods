#!/opt/BIOSTAT/bin/perl
#
# Show sequence from parameter 1 to parameter 2 in random order

use strict;
use warnings;

# Find program name
(my $basename = $0) =~ s/.*\///;

# Get user defined parameters
if(@ARGV != 2)
{
	print "Usage: $basename start end\n  e.g. $basename 1 10\n";
	exit 1;
}

# Give user parameters some nicer names
my $start = $ARGV[0];
my $end = $ARGV[1];

# Specify ordered array as input
my @input = ($start ... $end);

# Define empty output and temporary arrays
my @output = ();
my @temp = ();

# Loop while we have input data
while(scalar(@input) > 0)
{
	# Create a random number between 0 and the number of elements in the input array
	# This random number will be our array index
	my $random = int(rand(scalar(@input)));

	# Add the chosen element of the input array to the output array
	push(@output,$input[$random]);

	# Loop through each element of the input array
	foreach my $i (0 ... scalar(@input-1))
	{
		# If this was not the chosen element
		if($random != $i)
		{
			# Add this element to the temporary array
			push(@temp, $input[$i]);
		}
	}

	# Copy the temporary array over the top of the input array
	# ready to be processed again
	@input = @temp;

	# Empty the temporary array;
	@temp = ();
}

# Show the final result
foreach my $number (@output)
{
	print "$number\n";
}
