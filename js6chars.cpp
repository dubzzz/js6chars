//#define _DEBUG

#include <algorithm>
#include <climits>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#ifdef _DEBUG
  #include <iostream>
#endif

#include "js6chars.hpp"
std::string char_repr(char value, bool* cannot_use, char forbidden);
std::string str_repr(const char* str, bool* cannot_use, char forbidden);
std::string str_repr(std::string const& in, bool* cannot_use, char forbidden);

static std::string small_number_repr(int value)
{
  return value == 0
      ? "+[]"
      : value == 1
        ? "++[[]][+[]]"
        : "++[" + number_repr(value -1) + "][+[]]";
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

struct Generator { virtual std::string operator() (char, bool*, char) = 0; };
// 0123456789 should be omitted from require variable as trivally constructibles

struct NumberGenerator : Generator
{
  static constexpr const char* generate = "0123456789";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/, char /*forbidden*/) override { return number_repr(value-'0') + "+[]"; }
};
struct UndefinedGenerator : Generator
{
  static constexpr const char* generate = "undefi";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/, char /*forbidden*/) override { return from_known(value, "undefined", "[][+[]]"); }
};
struct NanGenerator : Generator
{
  static constexpr const char* generate = "Na";
  static constexpr const char* require = "u";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "NaN", "+" + char_repr('u', cannot_use, forbidden)); }
};
struct SmallerNanGenerator : Generator
{
  static constexpr const char* generate = "Na";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/, char forbidden) override { return forbidden != '!' ? from_known(value, "NaN", "+(![]+[])") : ""; }
};
struct PlusGenerator : Generator
{
  static constexpr const char* generate = "+";
  static constexpr const char* require = "e";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "1e+30", "+(" + str_repr("1e30", cannot_use, forbidden) + ")"); }
};
struct FindGenerator : Generator
{
  static constexpr const char* generate = "ucto ()";
  static constexpr const char* require = "find";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "function find()", "[][" + str_repr("find", cannot_use, forbidden) + "]"); }
};
struct TrueGenerator : Generator
{
  static constexpr const char* generate = "true";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/, char forbidden) override { return forbidden != '!' ? from_known(value, "true", "!![]") : ""; }
};
struct FalseGenerator : Generator
{
  static constexpr const char* generate = "false";
  static constexpr const char* require = "";
  std::string operator() (char value, bool* /*cannot_use*/, char forbidden) override { return forbidden != '!' ? from_known(value, "false", "![]") : ""; }
};
struct InfinityGenerator : Generator
{
  static constexpr const char* generate = "Infity";
  static constexpr const char* require = "e";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "Infinity", "+(" + str_repr("1e1000", cannot_use, forbidden) + ")"); }
};
struct SquareBracketGenerator : Generator
{
  static constexpr const char* generate = "[objct AayIa]";
  static constexpr const char* require = "entris";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "[object Array Iterator]", "[][" + str_repr("entries", cannot_use, forbidden) + "]"); }
};
struct ClassArrayGenerator : Generator
{
  static constexpr const char* generate = "fi Aay()";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "function Array()", "[][" + str_repr("constructor", cannot_use, forbidden) + "]"); }
};
struct ClassBooleanGenerator : Generator
{
  static constexpr const char* generate = "fi Blea()";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "function Boolean()", "(![])[" + str_repr("constructor", cannot_use, forbidden) + "]"); }
};
struct ClassNumberGenerator : Generator
{
  static constexpr const char* generate = "fi Nmbe()";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "function Number()", "(+[])[" + str_repr("constructor", cannot_use, forbidden) + "]"); }
};
struct ClassStringGenerator : Generator
{
  static constexpr const char* generate = "fi Sg()";
  static constexpr const char* require = "constru";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "function String()", "([]+[])[" + str_repr("constructor", cannot_use, forbidden) + "]"); }
};
struct ClassFunctionGenerator : Generator
{
  static constexpr const char* generate = " F()";
  static constexpr const char* require = "construfid";
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return from_known(value, "function Function()", "[][" + str_repr("find", cannot_use, forbidden) + "][" + str_repr("constructor", cannot_use, forbidden) + "]"); }
};
struct NumberInBaseGenerator : Generator
{
  static constexpr const char* generate = "abcdefhjklmpqsuvwxyz";
  static constexpr const char* require = "toSring";
  std::string operator() (char value, bool* cannot_use, char forbidden) override
  {
    int base = 11 + value -'a';
    if (base <= 30) { // max is 36
      base = ((int)(base/10) +1) * 10;
    }
    return "(" + number_repr(10 + value -'a') + ")[" + str_repr("toString", cannot_use, forbidden) + "](" + number_repr(base) + ")";
  }
};
struct CGenerator : Generator
{
  static constexpr const char* generate = "C";
  static constexpr const char* require = "findconstructorreturn atob(arguments[])N";
  std::string operator() (char, bool* cannot_use, char forbidden) override { return "[][" + str_repr("find", cannot_use, forbidden) + "][" + str_repr("constructor", cannot_use, forbidden) + "](" + str_repr("return atob(arguments[0])", cannot_use, forbidden) + ")(" + str_repr("20N", cannot_use, forbidden) + ")[" + number_repr(1) + "]"; }
};
struct AllGenerator : Generator
{
  static constexpr const char* require = "construfmChade";// and also !
  std::string operator() (char value, bool* cannot_use, char forbidden) override { return "([]+[])[" + str_repr("constructor", cannot_use, forbidden) + "][" + str_repr("fromCharCode", cannot_use, forbidden) + "](" + number_repr((int)value) + ")"; }
};

template <class Gen> void push_dependency(auto&& tree, char c)
{
  constexpr const char* str = Gen::require;
#ifdef _DEBUG
  std::cout << "  (" << ((int)c) << ")'" << c << "' requires \"" << str << "\"" << std::endl;
#endif 
  tree[c - CHAR_MIN].emplace_back(str, std::make_unique<Gen>());
}
template <class Gen> void push_dependencies(auto&& tree)
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
  push_dependencies<NanGenerator>(tree);
  push_dependencies<SmallerNanGenerator>(tree);
  push_dependencies<PlusGenerator>(tree);
  push_dependencies<FindGenerator>(tree);
  push_dependencies<TrueGenerator>(tree);
  push_dependencies<FalseGenerator>(tree);
  push_dependencies<InfinityGenerator>(tree);
  push_dependencies<SquareBracketGenerator>(tree);
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

std::string char_repr(char value, bool* cannot_use, char forbidden)
{
  static const auto tree = build_dependency_tree();
#ifdef _DEBUG
  std::cout << "Generating (" << ((int)value) << ")'" << value << "'";
  if (forbidden) std::cout << " with forbidden (" << ((int)forbidden) << ")'" << forbidden << "'";
  std::cout << std::endl;
#endif
  auto const& choices = tree[value - CHAR_MIN];
#ifdef _DEBUG
  std::cout << "  # " << choices.size() << " candidates" << std::endl;
#endif

  // Keep only generators that can be used (do no require stuff being computed)
  std::vector<Generator*> generators;
  for (auto const& choice : choices)
  {
    std::string const& require = choice.first;
#ifdef _DEBUG
    std::cout << "  candidate with require: " << require << std::endl;
#endif
    if (std::find_if(require.begin(), require.end(), [cannot_use](auto c) { return cannot_use[c - CHAR_MIN]; }) == require.end())
    {
      generators.push_back(choice.second.get());
    }
  }
#ifdef _DEBUG
  std::cout << "  # " << generators.size() << " selected" << std::endl;
#endif
  
  // Find the best match
  
  cannot_use[value - CHAR_MIN] = true;
  
  std::string best_match;
  for (auto gen : generators)
  {
    std::string match = gen->operator()(value, cannot_use, forbidden);
#ifdef _DEBUG
    std::cout << "  :(" << ((int)value) << ")'" << value << "': " << match << std::endl;
#endif
    if (best_match.empty() || best_match.size() > match.size())
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
  return char_repr(value, tab, '\0');
}

std::string str_repr(const char* str, bool* cannot_use, char forbidden)
{
  std::string out {};
  for (auto it = str ; *it ; ++it)
  {
    if (cannot_use[*it - CHAR_MIN]) { return out; }
  }
  for (auto it = str ; *it ; ++it)
  {
    if (it != str)
    {
      out += '+';
    }
    out += '(';
    auto forchar = char_repr(*it, cannot_use, forbidden);
    if (forchar.empty()) { return ""; }
    out += forchar;
    out += ')';
  }
  return out;
}
std::string str_repr(std::string const& in, bool* cannot_use, char forbidden)
{
  return str_repr(in.c_str(), cannot_use, forbidden);
}
std::string str_repr(const char* str)
{
  bool tab[CHAR_MAX-CHAR_MIN] = { 0 };
  std::fill(std::begin(tab), std::end(tab), false);
  return str_repr(str, tab, '\0');
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

