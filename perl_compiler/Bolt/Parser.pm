package Bolt::Parser;
use strict;
use warnings;

sub make_error
{
  my ($string) = @_;

  return { type => "error", message => $string, where => "TODO" };
}

sub is_elem
{
  my ($elem, $token_type, $value) = @_;

  if ($elem->{token} ne $token_type) {
    return undef;
  }

  if (defined $value && $elem->{string} ne $value) {
    return undef;
  }

  return $elem;
}

sub is_list
{
  my ($elements) = @_;

  return undef  unless (scalar @$elements);
  return undef  unless ($elements->[0]->{token} eq 'l_paren');

  return 1;
}

sub shift_end
{
  my ($elements) = @_;

  return shift @$elements unless ($elements->[0]->{token} eq "sep_statement") ;
  return undef;
}

# "eat" next token as long as it this type
# returns: token | undef
sub shift_next_tokens
{
  my ($elements, @accept_tokens) = @_;

  # out of elements
  return undef unless (scalar @$elements);

  # eat valid tokens in the element list
  foreach my $token (@accept_tokens) {
    return shift @$elements if ($token eq $elements->[0]->{token});
  }

  return undef;
}

# make a symbol path
sub eat_symbol_path
{
  my ($elements) = @_;
  my @path;

  # keep looking for symbol parts
  while (my $path_part = shift_next_tokens($elements, 'sep_member', 'symbol')) {
    push @path, $path_part;
  }

  # didn't find a path return nothing (TODO: return ERROR)
  return undef unless (@path);

  # return path element
  return { type => "symbol_path", path => \@path };
}

sub eat_list
{
  return undef;
}

sub parse_import
{
  my ($elements) = @_;

  my $import_AST = {
    type            => "import",
    import_module   => undef,
    import_symbols  => undef,
    as_module       => undef,
    as_symbols      => undef,
  };


  # shift off "import"
  shift @$elements;

  # module name
  return make_error("excpected symbol path")
    unless ($import_AST->{import_module} = eat_symbol_path($elements));

  # EOS
  return $import_AST
    if shift_end($elements);

  # import symbol list
  $import_AST->{import_sybols} = eat_list($elements)
    if (is_list($elements));

  # EOS
  return $import_AST
    if shift_end($elements);

  return make_error("expected something else")
    unless (shift_next_token($elements, 'keyword:as'));


  return $import_AST;
}

sub build_AST
{
  my ($token_list) = @_;
  my @AST;

  while (scalar @$token_list) {
    my $elem = $token_list->[0];

    use Data::Dumper;     
    # print Dumper($elem);

    if ($elem->{token} eq 'keyword' && $elem->{string} eq 'import') {
      print "Got here\n";
      push @AST, parse_import($token_list);
    }

    shift @$token_list;
  }

  return \@AST;
}


1;
