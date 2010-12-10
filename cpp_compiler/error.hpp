#ifndef __ERROR_HPP__
#define __ERROR_HPP__

/* C++ */
#include <exception>
#include <string>
/* bolt */
#include "boltdef.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"

class exception : public std::exception
{
  public:
    virtual
    std::string what_long() const = 0;
};

class bad_statement : public exception
{
  private:
    const state&  where;

  public:
    bad_statement(const state &_where)
      : where(_where)
    { }

    const char *what()
    {
      return "Unable to parse statement";
    }

    std::string what_long() const;
};

class bad_token : public exception
{
  private:
    const state&  where;
    token_type    type;

  public:
    bad_token(const state &_where, token_type _type)
      : where(_where), type(_type)
    { }

    const char *what()
    {
      return "Unexpected/bad token";
    }

    std::string what_long() const;
};

class bad_token_value : public exception
{
  private:
    const state&  where;
    token_type    type;
    const char*   value;

  public:
    bad_token_value(const state &_where, token_type _type, const char *_value)
      : where(_where), type(_type), value(_value)
    { }

    const char *what()
    {
      return "Bad token value";
    }

    std::string what_long() const;
};

#endif /* __ERROR_HPP__ */
