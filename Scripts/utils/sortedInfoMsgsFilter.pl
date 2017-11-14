#!/usr/local/bin/perl -w
# ====================================================================
#
# Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
# All Rights Reserved.
#
# Open64 is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# Open64 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
#
# ====================================================================
# 
# Select only +Oinfo messages, sorted.
#
# Command line parameters:
# $1 -- input file to filter. 
#
# Output is written to stdout.
#

use strict; 
#use warnings;
use File::Basename;

#---------------------------------------
#
# Command line args
#

# name of currently executing script
my $me;
my $me_fullpath = $0;
($me = $0) =~ s%.*/%%;

# test name
my $infile = shift @ARGV || "";

sub error {
  print STDERR "$me: ";
  print STDERR @_;
  print STDERR "\n";
  exit 1;
}

sub byline {
  my $ll = -1;
  my $rl = -1;

  if ($a =~ /\sline\s(\d+)/) {
    $ll = $1;
  }
  if ($b =~ /\sline\s(\d+)/) {
    $rl = $1;
  }
  if ($ll != $rl) {
    return $ll - $rl;
  }
  return  $a cmp $b;
}


#
# Validate command line parameters
#
if ($infile eq "") {
  error("invalid parameter");
}
open (IN, "< $infile") or
  error("can't open/access input file $infile");

my @outlines = ();

my $line;
while ($line = <IN>) {
  if ($line =~ /[iI]nfo\s+(\d+)\s*\:/) {
    push @outlines, $line;
  }
}
close IN;
my $l;
for $l (sort byline @outlines) {
  print $l;
}
exit 0;
