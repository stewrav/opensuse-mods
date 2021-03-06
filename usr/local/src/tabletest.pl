#!/usr/bin/perl
# Table test
# by S G Ravenhall
# October-November 2013

use warnings;
use strict;

use Env 'HOME';

my $basename   = ( split '/', $0 )[-1];
my $numtests   = 10;
my $numcorrect =  0;
my $again      = 'y';

my @tables_a   = (1..12);
my @tables_b   = (2,3,4,5,6,11);
my @types      = ('mult','mult','div');
my $type_index;

# If there is a configuration file then read it in
my $config = $HOME . '/.tables';
if ( -e $config ) {
	print "Reading $config...\n";
	open( CONFIG, '<', $config )
		or die "$basename: could not open $config file for reading: $!";
	while( <CONFIG> ) {
		my $line = $_;
		chomp $line;
		# print "\$line == $line\n";

		my ( $item, $value ) = $line =~ m/
			^
			\s*
			([\S]+)  # $item
			\s*
			=
			\s*
			(.*?)    # $value
			\s*
			$
		/mx;
		# print "\$item == $item\n\$value == $value\n\n";

		if ( $item eq 'numtests' ) {
			$numtests = $value;
			print "\tNumber of tests is $value\n";
		}
		elsif ( $item eq 'tables' ) {
			@tables_b = ( split ',', $value );
			print "\tTables are ";
			foreach (@tables_b) {
				print $_, ', ';
			}
			print "\n";
		}
	}
	close CONFIG;
}  # if -e $config

while ( $again =~ m/^y/mi ) {

	foreach my $test (1..$numtests) {
	
		my $a = $tables_a[ rand(@tables_a) ];
		my $b = $tables_b[ rand(@tables_b) ];
		my $c = $a * $b;
		my $answer;
		$type_index = 0
			if not defined $type_index or $type_index ge @types;

		until (defined $answer and $answer =~ m/^\d+$/m) {

			print "\n(Question $test/$numtests) \t ";

			if ($types[$type_index] eq 'mult') {
				print "$a times $b = ";
			}
			elsif ($types[$type_index] eq 'div') {
				print "$c divided by $b = ";
			}
			else {
				die;
			}
			$answer = <STDIN>;
		}

		if ( ( $types[$type_index] eq 'mult' and $c == $answer ) or
		     ( $types[$type_index] eq 'div'  and $a == $answer ) ) {
			print "                \t Correct!";
			$numcorrect += 1;
		}
		else {
			print "                \t Wrong\n";
			if ( $types[$type_index] eq 'mult' ) {
				print "                \t $a times $b = " . $c;
			}
			elsif ( $types[$type_index] eq 'div' ) {
				print "                \t $c divided by $b = " . $a;
			}
			else {
				die "$basename: did not understand type $types[$type_index]";
			}
		}
		print "\n";

		$type_index += 1;
	}

	print "You scored $numcorrect out of $numtests\n";
	
	print "Again? ";
	$again = <STDIN>;
	$numcorrect = 0;
}

