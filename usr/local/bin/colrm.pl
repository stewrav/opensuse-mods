#!/opt/BIOSTAT/bin/perl -w
# colrm, v2.0, by Eric Pement (pemente@northpark.edu) 2001/01/31
# Greatly revised, based on v1.3 1999/03/03 02:16:54 jsh Exp jsh $

use strict;
my $usage =<<"END";
  usage: colrm startcol [endcol] [file1 file2...]

  colrm 6      - Delete from column 6 to end-of-line
  colrm 7 7    - Delete column 7 only
  colrm 8 21   - Delete from col. 8 to col. 21, inclusive. The second
                 number must never be less than the first.
  colrm 6 foo  - if arg #2 is not numeric, it's considered a file

  Arg#1 is REQUIRED and must be an integer > 0. Column numbering begins
  with 1, not 0. Input comes from stdin, but filenames may appear on the
  command line. Changes are sent to the console; no file is altered on the
  disk. If no args are given or endcol < startcol, display this message.

END

my ($startcol, $arg2);
my $nonum = "\aERROR! First argument to colrm must be a valid number.\n\n";

sub loop {
  my ($act, $start, $arg) = @_;
  my ($val);
  if ($act eq "one") {     # if $act eq "one", $arg is a file
    unshift @ARGV, $arg;
    $val = '$_ = substr($_, 0, $start);';
  } else {                 # if $act eq "two", $arg is a number
    $val = 'substr($_, $start, $arg - $start) = "";';
  }

  if (@ARGV == 0) {
    while (<>) {
      chomp;
      eval($val);
      print "$_\n";
    }
  }
  else {
    while (@ARGV) {
      my $testf;
      $testf = shift(@ARGV);
      my $nofile = "\aERROR! File $testf does not exist or is not a text file.\n";
      die $nofile unless (-e $testf && -T _ );
      open(FH,"< $testf")   or die "Cannot open $testf for reading: $!\n";
      while (<FH>) {
        chomp;
        eval($val);
        print "$_\n";
      }
      close(FH)      or die "Cannot close $testf: $!\n";
    }
  }
}

# --- main routine begins ---

if (@ARGV == 0) {        # Issue help msg if no args at all
   die $usage;
} elsif (@ARGV == 1) {   # First arg must be numeric only
   $startcol = shift;
   die $nonum . $usage unless $startcol-- =~ /^[1-9][0-9]*$/ ;
   while (<>) {
      chomp;
      $_ = substr($_,0,$startcol);
      print "$_\n";
   }
} else {                 # We have 2 or more args on the command line
   $startcol = shift;
   die $nonum . $usage unless ( $startcol-- =~ /^[1-9][0-9]*$/ );
   $arg2 = shift;
   if ($arg2 =~ /^[1-9][0-9]*$/ && $arg2 <= $startcol ) {
     die $usage;
   }
   elsif ( $arg2 =~ /^[1-9][0-9]*$/ ) {
     loop ("two",$startcol,$arg2);
   }
   else {
     # arg2 is non-numeric, so it must be a file.
     my $nofil = "\aERROR! File $arg2 does not exist or is not a text file.\n";
     die $nofil unless ( -e $arg2 && -T _ );
     loop ("one",$startcol,$arg2);
   }
}
