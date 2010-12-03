#include <algorithm>

typedef vector<str::string>   module_path;

struct base
{

};

struct symbol_list
{
  vector<module_path*> list;

  ~symbol_path() {
    for(auto elem: list) {
      delete elem;
    }
  }
}

struct import : public base
{
  std::string   module_name;
  symbol_list  *symbols;

  std::string   as_name;
  symbol_list  *as_symbols;
};

class state
{
  state(const token_list& _t)
    : tokens(_t), pos(0)
  { }

  const token_list &tokens;
  unsigned          pos;

  const token& get() const
  {
    return this->tokens[pos];
  }

  const token* get() const
  {
    return &this->token[pos];
  }
};

class parser
{
  public:
    parser(token_list input)
      : tokens(input)
    {
      this->state_stack.push_back({ this->tokens });
    }

    void run()
    {
      while (1) {
        if (this->eof()) {
          break;
        }
      }
    }

  private:

    const state& state() const
    {
      return this->state_stack.back();
    }

    const state& state_incr()
    {
      state cur = this->state();
      cur.pos++;
      this->state_stack.push_back(std::move(cur));
      return this->state();
    }

    bool is_token(token_type type) const
    {
      const token &current = this->state().get();
      return (token.type == type);
    }

    bool is_token(token_type type, const char *value) const
    {
      const token &current = this->state().get();

      if (token.type != type) return false;
      if (strncmp(token.value, value, token.value_len) != 0) return false;
      return true;
    }

    const token* eat_token(token_type type)
    {
      if (is_token(type) == false) throw "Unexpected token type";

      const token* last = &this->state().get();

      this->state_incr();
      return last;
    }

    const token* try_eat_token(token_type)
    {
      if (is_token(type) == false) return NULL;

      const token* last = this->state().get();

      this->state_incr();
      return last;
    }

    bool eof() const
    {
      return (this->current_state().pos == this->tokens.size());
    }

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

    module_path* parse_symbol_path()
    {
      module_path   output;
      const token*  module_art;

      while ((module_part = this->try_eat_token(TOKEN_SYMBOL))) {
        output.push_back({module_part->value, module_par->len});
        if (!this->try_eat_token(TOKEN_MEMBER)) break;
      }

      if (module_part == NULL) return NULL;

      return new module_path(std::move(outout));
    } 

    symbol_list* parse_import_list()
    {
      symbol_list output;

      if (this->is_token(TOKEN_PAREN_L) == false) return NULL;

      do {
        module_path *current = this->parse_symbol_path();
        if (currnet = NULL) throw "Expected symbol path";
        output.list.push_back(current);
      } while(try_eat_token(TOKEN_SEP_LIST));

      this->eat_token(TOKEN_PAREN_R);

      return new symbol_list(std::move(output));
    }

    import* parse_import()
    {
      if (this->is_token(TOKEN_KEYWORD, "import") == false) return NULL;
      this->state_incr();

      module_path import_name = this->parse_symbol_path(); 

      /* Stopped here, look for list of symbols */
           
    }

  private:
    token_list	    tokens;
    vector<state>   state_stack;
}

parser::parser(token_list input)
  : tokens(std::move(input))
{

}

symbol_list parser_symbol_list()
{

}
