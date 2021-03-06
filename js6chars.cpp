//#define _DEBUG

#include <algorithm>
#include <climits>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#ifdef _DEBUG
  #include <iostream>
  #include <typeinfo>
#endif

#include "js6chars.hpp"
std::string char_repr(char value, bool* cannot_use);
std::string str_repr(const char* str, bool* cannot_use);
std::string str_repr(std::string const& in, bool* cannot_use);

static std::string small_number_repr(int value)
{
  return value == 0
      ? "+[]"
      : value == 1
        ? "+!+[]"
        : value == 2
          ? "!+[]+!+[]"
          : "!+[]+" + number_repr(value -1);
}

static std::string number_repr_helper(int value)
{
  if (value < 10) return small_number_repr(value) + "+[]"; 
  return number_repr_helper(value/10) + "+(" + small_number_repr(value%10) + ")";
}

std::string number_repr(int value)
{
  if (value < 0)
  {
    return std::string("+(") + char_repr('-') + "+(" + number_repr_helper(-value) + "))";
  }
  return value < 10
      ? small_number_repr(value)
      : std::string("+(") + number_repr_helper(value) + ")";
}

namespace {
template <std::size_t N> std::string from_known(char value, const char (&real_str)[N], std::string&& generated)
{
  std::string shortest_match;
  char const* it = std::find(std::begin(real_str), std::end(real_str), value);
  for ( ; it != std::end(real_str) ; it = std::find(std::next(it), std::end(real_str), value))
  {
    std::string match = "(" + generated + "+[])" + "[" + number_repr((int)(it - real_str)) + "]";
    if (shortest_match.empty() || shortest_match.size() > match.size())
    {
      shortest_match = match;
    }
  }
  return shortest_match;
}
}

struct Generator { virtual std::string operator() (char, bool*) = 0; };
// 0123456789 should be omitted from require variable as trivally constructibles

struct NumberGenerator : Generator
{
  static constexpr const char* generate = "0123456789";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return number_repr(value-'0') + "+[]"; }
};
struct UndefinedGenerator : Generator
{
  static constexpr const char* generate = "undefi";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return from_known(value, "undefined", "[][[]]"); }
};
struct CommaGenerator : Generator
{
  static constexpr const char* generate = ",";
  static constexpr const char* require = "conat";
  std::string operator() (char /*value*/, bool* cannot_use) override {
    std::string data = str_repr("concat", cannot_use);
    return data.empty() ? "" : "([[]][" + data + "](+[])+[])[+[]]"; }
};
struct NanGenerator : Generator
{
  static constexpr const char* generate = "Na";
  static constexpr const char* require = "u";
  std::string operator() (char value, bool* cannot_use) override { return from_known(value, "NaN", "+" + char_repr('u', cannot_use)); }
};
struct SmallerNanGenerator : Generator
{
  static constexpr const char* generate = "Na";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return from_known(value, "NaN", "+(![]+[])"); }
};
struct SmallerNanBisGenerator : Generator
{
  static constexpr const char* generate = "Na";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return from_known(value, "NaN", "+[][+[]]"); }
};
struct SmallerNanTerGenerator : Generator
{
  static constexpr const char* generate = "Na";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return from_known(value, "NaN", "+[![]]"); }
};
struct PlusGenerator : Generator
{
  static constexpr const char* generate = "+";
  static constexpr const char* require = "e";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("1e30", cannot_use);
    return data.empty() ? "" : from_known(value, "1e+30", "+(" + data + ")"); }
};
struct MinusGenerator : Generator
{
  static constexpr const char* generate = "-";
  static constexpr const char* require = "indexOf";
  std::string operator() (char /*value*/, bool* cannot_use) override {
    std::string data = str_repr("indexOf", cannot_use);
    return data.empty() ? "" : "([][" + data + "]([])+[])[+[]]"; }
};
struct FindGenerator : Generator
{
  static constexpr const char* generate = "ucto ()";
  static constexpr const char* require = "find";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("find", cannot_use);
    return data.empty() ? "" : from_known(value, "function find()", "[][" + data + "]"); }
};
struct TrueGenerator : Generator
{
  static constexpr const char* generate = "true";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return from_known(value, "true", "!![]"); }
};
struct FalseGenerator : Generator
{
  static constexpr const char* generate = "false";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/) override { return from_known(value, "false", "![]"); }
};
struct InfinityGenerator : Generator
{
  static constexpr const char* generate = "Infity";
  static constexpr const char* require = "e";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("1e1000", cannot_use);
    return data.empty() ? "" : from_known(value, "Infinity", "+(" + data + ")"); }
};
struct SquareBracketGenerator : Generator
{
  static constexpr const char* generate = "[objct AayIa]";
  static constexpr const char* require = "entris";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("entries", cannot_use);
    return data.empty() ? "" : from_known(value, "[object Array Iterator]", "[][" + data + "]()"); }
};
struct ClosedCurlyBracedGenerator : Generator
{//console.log(([].find+[]).split([]+[]).sort().reverse()[0]);
  static constexpr const char* generate = "}";
  static constexpr const char* require = "findspltrev";
  std::string operator() (char /*value*/, bool* cannot_use) override {
    std::string data = str_repr("find", cannot_use);
    std::string split_method = str_repr("split", cannot_use);
    std::string reverse_method = str_repr("reverse", cannot_use);
    return data.empty() ? "" : "([][" + data + "]+[])[" + split_method + "]([]+[])[" + reverse_method + "]()[+[]]"; }
};
struct ClassArrayGenerator : Generator
{
  static constexpr const char* generate = "fi Aay(){";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("constructor", cannot_use);
    return data.empty() ? "" : from_known(value, "function Array() {", "[][" + data + "]"); }
};
struct ClassBooleanGenerator : Generator
{
  static constexpr const char* generate = "fi Blea(){";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("constructor", cannot_use);
    return data.empty() ? "" : from_known(value, "function Boolean() {", "(![])[" + data + "]"); }
};
struct ClassNumberGenerator : Generator
{
  static constexpr const char* generate = "fi Nmbe(){";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("constructor", cannot_use);
    return data.empty() ? "" : from_known(value, "function Number() {", "(+[])[" + data + "]"); }
};
struct ClassStringGenerator : Generator
{
  static constexpr const char* generate = "fi Sg(){";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use) override {
    std::string data = str_repr("constructor", cannot_use);
    return data.empty() ? "" : from_known(value, "function String() {", "([]+[])[" + data + "]"); }
};
struct ClassFunctionGenerator : Generator
{
  static constexpr const char* generate = " F(){";
  static constexpr const char* require = "construfid";
  std::string operator() (char value, bool* cannot_use) override {
    std::string method_label = str_repr("find", cannot_use);
    std::string constructor = str_repr("constructor", cannot_use);
    return method_label.empty() || constructor.empty() ? "" : from_known(value, "function Function() {", "[][" + method_label + "][" + constructor + "]"); }
};
struct NumberInBaseGenerator : Generator
{
  static constexpr const char* generate = "abcdefhjklmpqsuvwxyz";
  static constexpr const char* require = "toSring";
  std::string operator() (char value, bool* cannot_use) override
  {
    int base = 11 + value -'a';
    if (base <= 30) { // max is 36
      base = ((int)(base/10) +1) * 10;
    }
    std::string data = str_repr("toString", cannot_use);
    return data.empty() ? "" : "(" + number_repr(10 + value -'a') + ")[" + data + "](" + number_repr(base) + ")";
  }
};
struct CGenerator : Generator
{
  static constexpr const char* generate = "C";
  static constexpr const char* require = "findconstructorreturn atobN";
  std::string operator() (char, bool* cannot_use) override {
    std::string method_label = str_repr("find", cannot_use);
    std::string constructor = str_repr("constructor", cannot_use);
    std::string running_code = str_repr("return atob", cannot_use);
    std::string parameter = str_repr("20N", cannot_use);
    return method_label.empty() || constructor.empty() || running_code.empty() || parameter.empty()
        ? ""
        : "[][" + method_label + "][" + constructor + "](" + running_code + ")()(" + parameter + ")[" + number_repr(1) + "]"; }
};
struct AllGenerator : Generator
{
  static constexpr const char* require = "construfmChade";// and also !
  std::string operator() (char value, bool* cannot_use) override {
    std::string constructor = str_repr("constructor", cannot_use);
    std::string method_charcode = str_repr("fromCharCode", cannot_use);
    return constructor.empty() || method_charcode.empty()
        ? ""
        : "([]+[])[" + constructor + "][" + method_charcode + "](" + number_repr((int)value) + ")"; }
};

template <class Gen, class Tree> void push_dependency(Tree&& tree, char c)
{
  constexpr const char* str = Gen::require;
#ifdef _DEBUG
  std::cout << "  (" << ((int)c) << ")'" << c << "' requires \"" << str << "\"" << std::endl;
#endif 
  tree[c - CHAR_MIN].emplace_back(str, std::make_unique<Gen>());
}
template <class Gen, class Tree> void push_dependencies(Tree&& tree)
{
  for (auto it = Gen::generate ; !!*it ; ++it) { push_dependency<Gen>(tree, *it); }
}

auto build_dependency_tree()
{
  // for each char we declare the list of all the available Generator able to build it
  // give nthe definition of required entries
  // eg.:
  //   '[' -> [ (find), call [].find ]
  std::vector< std::vector< std::pair<std::string, std::unique_ptr<Generator>> > > tree(256);

#ifdef _DEBUG
  std::cout << "Start build_dependency_tree" << std::endl;
#endif 
  push_dependencies<NumberGenerator>(tree);
  push_dependencies<UndefinedGenerator>(tree);
  push_dependencies<CommaGenerator>(tree);
  push_dependencies<NanGenerator>(tree);
  push_dependencies<SmallerNanGenerator>(tree);
  push_dependencies<SmallerNanBisGenerator>(tree);
  push_dependencies<PlusGenerator>(tree);
  push_dependencies<MinusGenerator>(tree);
  push_dependencies<FindGenerator>(tree);
  push_dependencies<TrueGenerator>(tree);
  push_dependencies<FalseGenerator>(tree);
  push_dependencies<InfinityGenerator>(tree);
  push_dependencies<SquareBracketGenerator>(tree);
  push_dependencies<ClosedCurlyBracedGenerator>(tree);
  push_dependencies<ClassArrayGenerator>(tree);
  push_dependencies<ClassBooleanGenerator>(tree);
  push_dependencies<ClassNumberGenerator>(tree);
  push_dependencies<ClassStringGenerator>(tree);
  push_dependencies<ClassFunctionGenerator>(tree);
  push_dependencies<NumberInBaseGenerator>(tree);
  push_dependencies<CGenerator>(tree);
  push_dependencies<NumberInBaseGenerator>(tree);
  push_dependencies<ClassNumberGenerator>(tree);
  push_dependencies<ClassStringGenerator>(tree);
  push_dependencies<ClassFunctionGenerator>(tree);
  push_dependencies<NumberInBaseGenerator>(tree);
  push_dependencies<CGenerator>(tree); 
  for (int c = (int)CHAR_MIN ; c <= (int)CHAR_MAX ; ++c)
  {
    unsigned idx = c - CHAR_MIN;
    if (tree[idx].empty())
    {
      push_dependency<AllGenerator>(tree, (char)c);
    }
  }
#ifdef _DEBUG
  std::cout << "End build_dependency_tree" << std::endl;
#endif 
  
  return tree;
}

std::string char_repr(char value, bool* cannot_use)
{
  static const auto tree = build_dependency_tree();
#ifdef _DEBUG
  unsigned iter_id = std::count(cannot_use, cannot_use+256, true);
  for (unsigned i {} ; i != iter_id ; ++i) { std::cout << "    "; }
  std::cout << "Generating (" << ((int)value) << ")'" << value << "'" << std::endl;
#endif
  auto const& choices = tree[value - CHAR_MIN];
#ifdef _DEBUG
  for (unsigned i {} ; i != iter_id ; ++i) { std::cout << "    "; }
  std::cout << "  # " << choices.size() << " candidates" << std::endl;
#endif

  // Keep only generators that can be used (do no require stuff being computed)
  std::vector<Generator*> generators;
  for (auto const& choice : choices)
  {
    std::string const& require = choice.first;
    if (std::find_if(require.begin(), require.end(), [cannot_use](auto c) { return cannot_use[c - CHAR_MIN]; }) == require.end())
    {
      generators.push_back(choice.second.get());
    }
  }
#ifdef _DEBUG
  for (unsigned i {} ; i != iter_id ; ++i) { std::cout << "    "; }
  std::cout << "  # " << generators.size() << " selected" << std::endl;
#endif
  
  // Find the best match
  
  cannot_use[value - CHAR_MIN] = true;
  
  std::string best_match;
  for (auto gen : generators)
  {
#ifdef _DEBUG
    for (unsigned i {} ; i != iter_id ; ++i) { std::cout << "    "; }
    std::cout << "  :using generator <" << typeid(*gen).name() << ">" << std::endl;
#endif
    std::string match = gen->operator()(value, cannot_use);
#ifdef _DEBUG
    for (unsigned i {} ; i != iter_id ; ++i) { std::cout << "    "; }
    std::cout << "  :(" << ((int)value) << ")'" << value << "': <" << typeid(*gen).name() << "> : " << match << std::endl;
#endif
    if (best_match.empty() || (best_match.size() > match.size() && ! match.empty()))
    {
      best_match = match;
    }
  }
  
  cannot_use[value - CHAR_MIN] = false;
  
  return best_match;
}
std::string char_repr(char value)
{
  bool tab[CHAR_MAX-CHAR_MIN] = { 0 };
  std::fill(std::begin(tab), std::end(tab), false);
  return char_repr(value, tab);
}

std::string str_repr(const char* str, bool* cannot_use)
{
  std::string out {};
  for (auto it = str ; *it ; ++it)
  {
    if (cannot_use[*it - CHAR_MIN]) { return out; }
  }
  for (auto it = str ; *it ; ++it)
  {
    auto forchar = char_repr(*it, cannot_use);
    if (forchar.empty()) { return ""; }
    
    if (it != str)
    {
      out += '+';
      if (forchar[forchar.size() -3] == '+' && forchar[forchar.size() -2] == '[' && forchar[forchar.size() -1] == ']')
      {
        forchar = forchar.substr(0, forchar.size() -3);
      }
      if (forchar[0] == '+' || (forchar.size() >= 2 && forchar[0] == '!' && forchar[1] == '+'))
      {
        out += '(';
        out += forchar;
        out += ')';
      }
      else
      {
        out += forchar;
      }
    }
    else
    {
      out += forchar;
    }
  }
  return out;
}
std::string str_repr(std::string const& in, bool* cannot_use)
{
  return str_repr(in.c_str(), cannot_use);
}
std::string str_repr(const char* str)
{
  bool tab[CHAR_MAX-CHAR_MIN] = { 0 };
  std::fill(std::begin(tab), std::end(tab), false);
  return str_repr(str, tab);
}
std::string str_repr(std::string const& in) { return str_repr(in.c_str()); }

std::string run_command(const char* str)
{
  return std::string("[][") + str_repr("constructor") + "][" + str_repr("constructor") + "](" + str_repr(str) + ")()";
}
std::string run_command(std::string const& in)
{
  return run_command(in.c_str());
}

