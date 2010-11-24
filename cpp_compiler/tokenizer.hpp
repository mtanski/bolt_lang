#include <string>
#include <vector>

enum token_type {
  TOKEN_ERROR           = -1,
  TOKEN_COMMENT_LINE    = 0,
  TOKEN_COMMENT_LONG,
  TOKEN_WHITESPACE,
  TOKEN_SEP_STATEMENT,
  TOKEN_SEP_LIST,
  TOKEN_SEP_TYPE,
  TOKEN_SEP_MEMBER,
  TOKEN_VALUE_STRING,
  TOKEN_VALUE_NUMBER,
  TOKEN_VALUE_FLOAT,
  TOKEN_VALUE_CHAR,
  TOKEN_PAREN_L,
  TOKEN_PAREN_R,
  TOKEN_BRACKET_L,
  TOKEN_BRACKET_R,
  TOKEN_CURLY_L,
  TOKEN_CURLY_R,
  TOKEN_KEYWORD,
  TOKEN_OPERATOR,
  TOKEN_SYMBOL,
};


struct token
{
  token_type    type;
  const char*   value;
  size_t        len;
};


typedef std::vector<token> token_list;

token_list tokenizer(const char *input_str);

