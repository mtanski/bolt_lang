/* bolt */
#include "tokenizer.hpp"
#include "parser.hpp"


const token* parser::eat_token(token_type type, const char* value)
{
  if (is_token(type) == false) throw bad_token(this->state_get(), type);
  if (is_token(type, value) == false) throw bad_token_value(this->state_get(), type, value);

  const token* last = this->state_get().get();

  this->state_incr();
  return last;
}


void parser_bolt::run()
{
  std::vector<t_base *> output;

  while (!this->eof()) {
    struct t_base *elem;

    if (!((elem = this->parse_import()) ||
          (elem = this->parse_struct()) ||
          (elem = this->parse_object())
       ))
    {
      throw bad_statement(this->state_get());
    }

    output.push_back(elem);
  }
}

t_symbol* parser_bolt::parse_symbol()
{
  const token* t;

  if ((t = this->try_eat_token(TOKEN_SYMBOL)))
    return new t_symbol(t->value, t->len);

  return NULL;
}

t_module_path* parser_bolt::parse_symbol_path()
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

t_attr* parser_bolt::parse_attrs()
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

t_symbol_list* parser_bolt::parse_import_list()
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

t_import* parser_bolt::parse_import()
{
  t_import output;

  if (!this->try_eat_token(TOKEN_KEYWORD, "import")) return NULL;

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

t_container* parser_bolt::parse_struct()
{
  t_struct output;
  
  if (!(output.name = this->parse_symbol())) return NULL;
  if (!this->try_eat_token(TOKEN_SEP_TYPE)) return NULL;
  if (!this->try_eat_token(TOKEN_KEYWORD, "struct")) return NULL;
  

  return new t_struct(std::move(output));
}

t_container* parser_bolt::parse_object()
{
  t_object output;
  
  if (!(output.name = this->parse_symbol())) return NULL;
  if (!this->try_eat_token(TOKEN_SEP_TYPE)) return NULL;
  if (!this->try_eat_token(TOKEN_KEYWORD, "object")) return NULL;
  
  
  return new t_object(std::move(output));
}

