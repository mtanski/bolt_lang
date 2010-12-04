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

  protected:

    const state& state_get() const
    {
      return this->state_stack.back();
    }

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
    virtual
    void run() {
      while (1) {
        if (this->eof()) {
          break;
        }
      }
    }

  private:
    t_symbol* parse_symbol()
    {
      const token* t;

      if ((t = this->try_eat_token(TOKEN_SYMBOL)))
        return new t_symbol(t->value, t->len);

      return NULL;
    }

    t_module_path* parse_symbol_path()
    {
      t_module_path output;
      const token*  module_part;

      while ((module_part = this->try_eat_token(TOKEN_SYMBOL))) {
        output.push_back({module_part->value, module_part->len});
        if (!this->try_eat_token(TOKEN_SEP_MEMBER)) break;
      }

      if (module_part == NULL) return NULL;

      return new t_module_path(std::move(output));
    }

    t_attr* parse_attrs()
    {
      t_attr        output;
      const token*  t;

      if (!this->try_eat_token(TOKEN_BRACKET_L)) return NULL;
      
      for (t = this->try_eat_token_except(TOKEN_BRACKET_R); t ;
           t = this->try_eat_token(TOKEN_BRACKET_R))
      {
        std::string key = { t->value, t->len };
        std::string value = { "true" };

        if (this->try_eat_token(TOKEN_OPERATOR, "=")) throw "No attribute value";

        if (!this->try_eat_token(TOKEN_SEP_TYPE)) break;
      }

      this->eat_token(TOKEN_BRACKET_R);
      return new t_attr(std::move(output));
    }

    t_symbol_list* parse_import_list()
    {
      t_symbol_list output;

      if (!this->try_eat_token(TOKEN_PAREN_L)) return NULL;

      do {
        t_module_path *current = this->parse_symbol_path();
        if (current = NULL) throw "Expected symbol path";
        output.list.push_back(current);
      } while(try_eat_token(TOKEN_SEP_LIST));

      this->eat_token(TOKEN_PAREN_R);
      return new t_symbol_list(std::move(output));
    }

    t_import* parse_import()
    {
      t_import output;

      if (this->try_eat_token(TOKEN_KEYWORD, "import")) return NULL;

      if (!(output.module_name = this->parse_symbol_path()))
        throw "Expected module name";

      output.symbols = this->parse_import_list();
      
      if (this->try_eat_token(TOKEN_KEYWORD, "as")) {
        output.as_name = this->parse_symbol_path();
        output.as_symbols = this->parse_import_list();
      }

      this->eat_token(TOKEN_SEP_STATEMENT);
      return new t_import(std::move(output));
    }

    t_struct* parse_struct()
    {
      t_struct output;

      if (!(output.name = this->parse_symbol())) return NULL;
      if (!this->try_eat_token(TOKEN_SEP_TYPE)) return NULL;
      if (!this->try_eat_token(TOKEN_KEYWORD, "struct")) return NULL;

      return new t_struct(std::move(output));
    }
};

