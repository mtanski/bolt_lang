#ifndef __PARSER_HPP__
#define __PARSER_HPP__

/* libc */
#include <cstring>
/* C++ */
#include <algorithm>
#include <map>
#include <string>
#include <vector>
/* bolt */
#include "tokenizer.hpp"


typedef std::vector<std::string>            t_module_path;
typedef std::string                         t_symbol;
typedef std::map<std::string, std::string>  t_attr;

struct base
{
  virtual
  const char* dump() const {
    throw "Unimplemented";
  }
};

struct t_symbol_list : public base
{
  std::vector<t_module_path*> list;

  ~t_symbol_list() {
    for(auto elem: list) {
      delete elem;
    }
  }
};

struct t_import : public base
{
  t_module_path   *module_name;
  t_symbol_list   *symbols;

  t_module_path   *as_name;
  t_symbol_list   *as_symbols;
};

struct t_attrs : public base
{
  t_attr           pairs;
};

struct t_struct : public base
{
  t_symbol        *name;
};

class state
{
  private:
    const token_list &tokens;
    unsigned          pos;

  private:
    state(const token_list& _t, unsigned _p)
      : tokens(_t), pos(_p)
    { }

  public:
    state(const token_list& _t)
      : tokens(_t), pos(0)
    { }

    state& operator=(const class state &src)
    {
      *this = src;
      return *this;
    }

    const token* get() const
    {
      return &this->tokens[pos];
    }

    bool eof() const
    {
      return (this->pos >= tokens.size());
    }

    class state operator++(int) const
    {
      return { this->tokens, this->pos + 1 };
    }
};

class parser
{
  private:
    token_list	          tokens;
    std::vector<state>    state_stack;

  public:
    parser(token_list input)
      : tokens(std::move(input))
    {
      this->state_stack.push_back({ this->tokens });
    }

    virtual
    void run() = 0;

    const state& state_get() const
    {
      return this->state_stack.back();
    }

  protected:

    const state& state_incr()
    {
      const state& cur = this->state_get();
      this->state_stack.push_back(std::move(cur++));
      return this->state_get();
    }

    bool is_token(token_type type, const char* value = NULL) const
    {
      const token *current = this->state_get().get();

      if (current->type != type) return false;
      if (value == NULL) return true;
      if (strncmp(current->value, value, current->len) != 0) return false;
      return true;
    }

    /** Eat any token that comes up next. */
    const token* eat_token()
    {
      const token* last = this->state_get().get();
      this->state_incr();
      return last;
    }

    /** Eat the token that matches paramaters, if not error out. */
    const token* eat_token(token_type type, const char* value = NULL)
    {
      if (is_token(type) == false) throw "Unexpected token type";
      if (is_token(type, value) == false) throw "Token value unexpected";

      const token* last = this->state_get().get();

      this->state_incr();
      return last;
    }

    /** Eat the next token if it matches paramters */
    const token* try_eat_token(token_type type, const char *value = NULL)
    {
      if (is_token(type, value) == false) return NULL;

      const token* last = this->state_get().get();

      this->state_incr();
      return last;
    }

    /** Eat the next token unless it matches paramters */
    const token* try_eat_token_except(token_type type, const char *value = NULL)
    {
      if (is_token(type, value) == true) return NULL;

      const token* last = this->state_get().get();

      this->state_incr();
      return last;
    }

    bool eof() const
    {
      return this->state_get().eof();
    }

#if 0
    template<typename FUNC, typename... ARGS>
    typename std::resultof<FUNC(ARGS...)>::type
    next_state(const FUNC &func, ARGS... func_arguments) {
      unsigned size = this->state_stack.size();
      try {
        return func(args...);
      } catch(...) {
        this->state_stack.resize = size;
        throw;
      }
    }
#endif

};

class parser_bolt : public parser
{
  public:
    parser_bolt(const token_list &input)
      : parser(input)
    { }

    virtual
    void run();

  private:
    t_symbol*       parse_symbol();
    t_module_path*  parse_symbol_path();

    t_attr*         parse_attrs();

    t_symbol_list*  parse_import_list();
    t_import*       parse_import();

    t_struct*       parse_struct();
};

#endif /* __PARSER_HPP__ */
