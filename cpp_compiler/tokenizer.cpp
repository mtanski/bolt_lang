#include "tokenizer.hpp"


/* Prototype for token functions */
typedef size_t(*token_func)(const char *input_str);


/* Simple one character token eater */
#define TOKEN_CHAR(name, c)                     \
static size_t is_##name(const char *input_str)  \
{                                               \
  return (*input_str == c);                     \
}


/* Simple tokenizers */
TOKEN_CHAR(sep_stement,   ';')
TOKEN_CHAR(sep_list,      ',')
TOKEN_CHAR(sep_type,      ':')
TOKEN_CHAR(sep_member,    '.')
TOKEN_CHAR(l_paren,       '(')
TOKEN_CHAR(r_paren,       ')')
TOKEN_CHAR(l_bracket,     '[')
TOKEN_CHAR(r_bracket,     ']')
TOKEN_CHAR(l_curly,       '{')
TOKEN_CHAR(r_curly,       '}')


static
size_t is_comment_line(const char* input_str)
{
  if (strcmp(input_str, "//") == 0) {
    return strcspn(input_str, "\r\n");
  }

  return 0;
}

static
size_t is_comment_long(const char* input_str)
{
  if (strcmp(input_str, "/*") == 0) {
    const char *end = strstr(input_str, "*/");

    if (end != NULL) {
      return end - input_str;
    }
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
size_t is_value_string(const char* input_str)
{
  if (*input_str == '"') {
    const char *end = strchr(input_str + 1, '"');

    if (end != NULL) {
      return end - input_str;
    }
  }

  return 0;
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
  is_value_number,
  is_value_float,
  is_value_char,
  is_keyword,
}


token_list tokenizer(const char *input_str)
{
  token_list output;

  return output;
}


