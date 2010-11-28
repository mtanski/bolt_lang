/* libc */
#include <string.h>
/* c++ */
#include <vector>
/* bolt */
#include "utils.h"
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
#define TOKEN_BLOCK(name, s, e)                             \
static size_t is_##name(const char *input_str)              \
{                                                           \
  if (strncmp(input_str, s, strlen(s)) == 0) {              \
    const char *end = strstr(input_str + strlen(s), e);     \
    if (end != NULL)                                        \
      return (end + strlen(e)) - input_str;                 \
  }                                                         \
  return 0;                                                 \
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

static
size_t is_operator(const char *input_str)
{
  static const char* operators[] = { "+", "-", "*", "/", "%", 
                                     "=",
                                     "==", "!=", "<", ">", "<=", ">=",
                                     "!", "&", "|", "^",
                                     "and", "or", "not", "xor", NULL };

  for (const char **cur = operators; *cur != NULL; cur++) {
    if (strcmp(input_str, *cur) == 0) {
      return strlen(*cur);
    }
  }

  return 0;
}

static
size_t is_symbol(const char *input_str)
{
  const char *end = input_str;

  switch (*end) {
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
    case '$':
    case '@':
      break;
    default:
      return 0;
  }

  for (end++; end != '\0'; end++) {
    switch (*end) {
      case 'a' ... 'z':
      case 'A' ... 'Z':
      case '_':
      case '$':
      case '@':
      case '!':
        break;
      default:
        return end - input_str;
    }
  }

  return end - input_str;
}

token_func token_functions[] = {
  is_comment_line,
  is_comment_long,
  is_whitespace,
  is_sep_statement,
  is_sep_list,
  is_sep_type,
  is_sep_member,
  is_l_paren,
  is_r_paren,
  is_l_bracket,
  is_r_bracket,
  is_l_curly,
  is_r_curly,
  is_value_string,
  is_value_float,
  is_value_number,
  is_value_char,
  is_keyword,
  is_operator,
  is_symbol,
  NULL
};

static
token make_error(const char *input_str)
{
  size_t offset;

  for (offset = 0; input_str[offset] != '\0'; offset++) {
    switch (input_str[offset]) {
      case '\r':
      case '\n':
        goto eol;
    }  
  }

eol:
  return { TOKEN_ERROR, input_str, offset };
}

static
token process_token(const char *input_str)
{
  for (int i = 0; token_functions[i] != '\0'; i++) {

    size_t eat_len = token_functions[i](input_str);
    if (eat_len == 0) continue;

    return { (token_type) i, input_str, eat_len };
  }

  /* Unknown token, error token eats chars till the end of the line */
  return make_error(input_str);
}

token_list tokenizer(const char *input_str)
{
  token_list output;

  while (*input_str != '\0') {

    token tmp = process_token(input_str);

    /* Advance input text pointer */
    input_str += tmp.len;

    /* Ignore comments & whitscape */
    switch (tmp.type) {
      case TOKEN_COMMENT_LINE:
      case TOKEN_COMMENT_LONG:
      case TOKEN_WHITESPACE:
        break;
      default:
        output.push_back(tmp);
    }
  }

  return output;
}


const char* token_name(enum token_type type)
{
#define __TOKEN_PRINT(x) case x: return STRINGIFY(x)

  switch (type) {
    __TOKEN_PRINT(TOKEN_ERROR);
    __TOKEN_PRINT(TOKEN_COMMENT_LINE);
    __TOKEN_PRINT(TOKEN_COMMENT_LONG);
    __TOKEN_PRINT(TOKEN_WHITESPACE);
    __TOKEN_PRINT(TOKEN_SEP_STATEMENT);
    __TOKEN_PRINT(TOKEN_SEP_LIST);
    __TOKEN_PRINT(TOKEN_SEP_TYPE);
    __TOKEN_PRINT(TOKEN_SEP_MEMBER);
    __TOKEN_PRINT(TOKEN_VALUE_STRING);
    __TOKEN_PRINT(TOKEN_VALUE_NUMBER);
    __TOKEN_PRINT(TOKEN_VALUE_FLOAT);
    __TOKEN_PRINT(TOKEN_VALUE_CHAR);
    __TOKEN_PRINT(TOKEN_PAREN_L);
    __TOKEN_PRINT(TOKEN_PAREN_R);
    __TOKEN_PRINT(TOKEN_BRACKET_L);
    __TOKEN_PRINT(TOKEN_BRACKET_R);
    __TOKEN_PRINT(TOKEN_CURLY_L);
    __TOKEN_PRINT(TOKEN_CURLY_R);
    __TOKEN_PRINT(TOKEN_KEYWORD);
    __TOKEN_PRINT(TOKEN_OPERATOR);
    __TOKEN_PRINT(TOKEN_SYMBOL);
    default:
      return "Unknown";
  }
}

void dump_tokens(const token_list& list)
{
  for (const token& t: list) {
    printf("  token: %s len: %i value: %.*s\n", token_name(t.type), t.len, t.len, t.value);
  }
}
