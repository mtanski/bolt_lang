#include "tokenizer.hpp"


enum digit_base { BASE_16, BASE_10, BASE_8, BASE_2 }; 

static inline bool isoctal(char c)
{
  switch (c) {
    case '0' ... '7': return true;
  }

  return false;
}

static inline bool isbase(digit_base base, char c)
{
  switch (base) {
    BASE16: return isxdigit(c);
    BASE10: return isdigit(c);
    BASE8:  return isoctal(c);
    BASE2:  return (c == '0' || c == 1);
  }

  return false;
}


/* Prototype for token functions */
typedef size_t(*token_func)(const char *input_str);


/* Simple one character token eater */
#define TOKEN_CHAR(name, c)                     \
static size_t is_##name(const char *input_str)  \
{                                               \
  return (*input_str == c);                     \
}

/* Get a back betweeb start and end */
#define TOKEN_BLOCK(name, s, e)                 \
static size_t is_##name(const char *input_str)  \
{                                               \
  if (strcmp(input_str, s) == 0) {              \
    const char *end = strstr(input_str, e);     \
                                                \
    if (end != NULL)                            \
      return end - input_str;                   \
  }                                             \
  return 0;                                     \
}


/* Simple tokenizers */
TOKEN_CHAR(sep_statement, ';')
TOKEN_CHAR(sep_list,      ',')
TOKEN_CHAR(sep_type,      ':')
TOKEN_CHAR(sep_member,    '.')
TOKEN_CHAR(l_paren,       '(')
TOKEN_CHAR(r_paren,       ')')
TOKEN_CHAR(l_bracket,     '[')
TOKEN_CHAR(r_bracket,     ']')
TOKEN_CHAR(l_curly,       '{')
TOKEN_CHAR(r_curly,       '}')

/* Simple blocks */
TOKEN_BLOCK(comment_long,  "/*", "*/")
TOKEN_BLOCK(value_string,  "\"", "\"")
TOKEN_BLOCK(value_char,     "'", "'")


static
size_t is_comment_line(const char* input_str)
{
  if (strcmp(input_str, "//") == 0) {
    return strcspn(input_str, "\r\n");
  }

  return 0;
}

static
size_t is_whitespace(const char* input_str)
{
  size_t eat;

  for (eat = 0; *input_str != '\0'; eat++, input_str++) {
    switch (*input_str) {
      case ' ':
      case '\n':
      case '\r':
      case '\t':
        break;
      default:
        return eat;
    }
  }

  return eat;
}

static
size_t is_value_float(const char* input_str)
{
  const char *end = input_str;

  if (*end == '-')
    end++;

  for (; *end != '\0'; end++) {
    if (!isdigit(*end)) break;
  }

  if (*end != '.') {
    return 0;
  }

  for (; end != '\0'; end++) {
    if (!isdigit(*end)) break;
  }

  return (*end != '.') ? end - input_str : 0;
}


static
size_t is_value_number(const char* input_str)
{
  const char *end = input_str;
  digit_base  base = BASE_10;

  if (*end == '-')
    end++;

  if (*end == '0') {
    switch (end[1]) {
      case 'x': base = BASE_16; break;
      case 'o': base = BASE_8;  break;
      case 'b': base = BASE_2;  break;
      default:
        return 0;
    }
    end += 2;
  }

  if (!isbase(base, *end))
    return 0;

  for (end++; *end != '\0'; end++) {
    if (isbase(base, *end)) break;
  }

  return end - input_str;
}

static
size_t is_keyword(const char *input_str)
{
  static const char* keywords[] = { "as", "break", "continiue", "case", "for", "function", "if",
                                    "import", "labda", "object", "switch", "struct", "var", NULL };

  for (const char **cur = keywords; *cur != NULL; cur++) {
    if (strcmp(input_str, *cur) == 0) {
      return strlen(*cur);
    }
  }

  return 0;
}

token_func token_functions[] = {
  is_comment_line,
  is_comment_long,
  is_whitespace,
  is_sep_statement,
  is_sep_list,
  is_sep_type,
  is_sep_member,
  is_value_string,
  is_value_float,
  is_value_number,
  is_value_char,
  is_keyword,
};


token_list tokenizer(const char *input_str)
{
  token_list output;

  return output;
}

