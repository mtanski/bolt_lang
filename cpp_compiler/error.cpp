#include "error.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"

std::string bad_statement::what_long() const
{
  const token* t = this->where.get();

  /* TODO: location */
  return std::string("Unable to determine statement from token (")
       + std::string(token_name(t->type))
       + std::string(")");
}

std::string bad_token::what_long() const
{
  const token* t = this->where.get();

  /* TODO: location */
  return std::string("Unexpected token (")
       + std::string(token_name(t->type))
       + std::string(") expected (")
       + std::string(token_name(this->type))
       + std::string(") instead");
}


std::string bad_token_value::what_long() const
{
  const token* t = this->where.get();

  /* TODO: location */
  return std::string("Unexpected token value (")
       + std::string(t->value, t->len)
       + std::string(") expected (")
       + std::string(this->value)
       + std::string(") instead");
}

