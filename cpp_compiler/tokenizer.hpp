#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__

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
  TOKEN_PAREN_L,
  TOKEN_PAREN_R,
  TOKEN_BRACKET_L,
  TOKEN_BRACKET_R,
  TOKEN_CURLY_L,
  TOKEN_CURLY_R,
  TOKEN_VALUE_STRING,
  TOKEN_VALUE_FLOAT,
  TOKEN_VALUE_NUMBER,
  TOKEN_VALUE_CHAR,
  TOKEN_KEYWORD,
  TOKEN_OPERATOR,
  TOKEN_SYMBOL,
};


struct token
{
  token_type    type;
  const char*   value;
  size_t        len;

  token(token_type _type, const char *_value, size_t _len)
    : type(_type), value(_value), len(_len)
  { }

  bool operator!() const
  { 
    return this->type != TOKEN_ERROR;
  }
};


typedef std::vector<token> token_list;

token_list tokenizer(const char *input_str);

const char* token_name(enum token_type type);
void dump_tokens(const token_list& list);

#endif /* __TOKENIZER_HPP__ */
