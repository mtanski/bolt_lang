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
#include "error.hpp"
#include "tokenizer.hpp"
#include "types.hpp"


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
      printf("%.*s / %s\n", current->len, current->value, value);
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
    const token* eat_token(token_type type, const char* value = NULL);

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

    t_container*    parse_struct();
    t_container*    parse_object();
};

#endif /* __PARSER_HPP__ */
