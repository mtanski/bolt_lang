typedef vector<std::string> symbol_list;
typedef vector<std::string> module_path;

struct base
{

};

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
      return (token.type == type);;
    }

    bool is_token(token_type type, const char *value) const
    {
      const token &current = this->state().get();

      if (token.type != type) return false;
      if (strncmp(token.value, value, token.value_len) != 0) return false;
      return true;
    }

    const token& eat_token(token_type type)
    {
      if (is_token(type) == false) throw "Unexpected token type";

      const token& last = this->state();

      this->statr_incr();
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

    module_path parse_symbol_path()
    {
      module_path result;

      while (this->eof() == false) {
        const token& module_part = this->eat_token(TOKEN_SYMBOL);
        result.push_back({ module_part.value, module_part.len });

        if (this->is_token(TOKEN_MEMBER) == false) break;
        this->state_incr();
      }

      return result;
    } 

    parse_import_list()
    {

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
