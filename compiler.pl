#!/usr/bin/env perl
use strict;
use warnings;

use Bolt::Tokenizer;

use Perl6::Slurp;
use Data::Dumper;

# Read in the source file
my $source = slurp (shift @ARGV);

# Tokenize the source code
my $token_list = Bolt::Tokenizer::tokenize($source);





sub error_line
{
  my ($input_str, $error_line) = @_;
  
  my $start = substr $input_str, $error_line->{position};
  $start =~ /^([^\r\n]+)/;
  
  return "Unable to parse (line TODO):\n\t$1";
}

sub print_errors
{
  my ($src_str, @token_list) = @_;
  
  # find error elements
  my @errors = grep { $_->{token} =~ /^error$/ } @token_list;
  # convert them to error lines
  my @error_lines = map { error_line($src_str, $_) } @errors;
  
  foreach my $line (@error_lines) { print "$line\n\n"; }
}

print Dumper($token_list);
print "\n\n";

print_errors($source, @$token_list);
