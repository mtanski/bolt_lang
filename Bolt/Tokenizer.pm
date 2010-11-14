package Bolt::Tokenizer;
use strict;
use warnings;

our %tokens = (
  "comment_line"      => qr/ \/\/ [^\r\n]+ /x,
  "comment_long"      => qr{ /\* .*? \*/ }x,

  "whitespace"        => qr/ [\s\n\r]+ /x,

  "sep_stament"       => qr/ ; /x,
  "sep_list"          => qr/ , /x,
  "sep_type"          => qr/ : /x,
  "sep.member"        => qr/ \. /x,

  "value_string"      => qr/ " [^"]* " /x,
  "value_number"      => qr/ -? \d+ | -? 0x [\da-f]+ | -? 0b [01] | -? 0o [0-8] /x,
  "value_float"       => qr/ -? \d+ \. \d+ [fd]? /x,
  "value_char"        => qr/ ' [^']+ ' /x,

  "l_paren"           => qr/ \( /x,
  "r_paren"           => qr/ \) /x,
  "l_bracket"         => qr/ \[ /x,
  "r_bracket"         => qr/ \] /x,
  "l_curly"           => qr/ \{ /x,
  "r_curly"           => qr/ \} /x,

  "keyword"           => qr{ as | case | for | function | if | import | lambda | object | 
                             switch | struct | var }x,

  "operator"          => qr{ \+ | - | \* | \\ | % |
                             = |
                             == | != | < | > | <= | <= |
                             ! | & | \| | ^ |
                             and | or | not | xor }x,

  "symbol"            => qr{ \w+ [\w\d_-]+ }x
);

sub tokenize
{
  my ($src) = @_;

  # Output
  my @statements;

  $src =~ /\A/mg;

  OUTER: while (1) {
    # end of line
    if ($src =~ /\G \Z/mcgx) {
      last; 
    }

    # Itterate through 
    foreach my $token (keys %tokens) {
      my ($value) = $src =~ /\G ( $tokens{$token} )/mcgx;

      # don't stick whitespace inside of the statement list
      if ($value && ($token ne "whitespace" || $token =~ /^comment_/)) {
        push @statements, {
          "token"     => $token,
          "string"    => $value,
          "position"  => $-[0] 
        };
      } 

      # move onto the next match
      if ($value) {
        next OUTER;
      } 
    }
    
    # Find the faulty token and try to consume the whole line
    my ($fault) = $src =~ /\G ( \S+ ) [^\n\r]* \n/mcgx;
    warn "Unrecognized token: $fault\n";

    # Throw in an error token to the output
    push @statements, {
      "token"     => "error",
      "string"    => $fault,
      "position"  => $-[0] 
    };
    
    last;
  }

  return \@statements;
}

1;
