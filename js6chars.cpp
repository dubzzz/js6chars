#include <algorithm>
#include <cstddef>
#include <string>

#include "js6chars.hpp"

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
template <std::size_t N, class Gen> bool from_known_str(std::string& in, const char (&real_str)[N], Gen&& offuscated_gen, char value)
{
  char const* it = std::find(std::begin(real_str), std::end(real_str), value);
  if (it != std::end(real_str))
  {
    in = "(";
    in += offuscated_gen();
    in += "+[])";
    in += "[" + number_repr((int)(it - real_str)) + "]";
    return true;
  }
  return false;
}
}

std::string char_repr(char value)
{
  if (value >= '0' && value <= '9')
  {
    return number_repr(value-'0') + "+[]";
  }

  std::string out {};

  // undefined === [][0]
  if (from_known_str(out, "undefined", [](){ return std::string("[][+[]]"); }, value))
  {//
    return out;
  }

  // NaN === +("u")
  if (from_known_str(out, "NaN", [](){ return std::string("+(") + char_repr('u') + ")"; }, value))
  {
    return out;
  }

  // 1e+308 === 1e308
  if (from_known_str(out, "1e+308", [](){ return std::string("+(") + str_repr(std::string("1e308")) + ")"; }, value))
  {
    return out;
  }

  // function find() { [native code] } === []["find"]
  if (from_known_str(out, "function find()", [](){ return std::string("[][") + str_repr("find") + "]"; }, value))
  {
    return out;
  }

  // true === !![]
  if (from_known_str(out, "true", [](){ return std::string("!![]"); }, value))
  {
    return out;
  }

  // false === ![]
  if (from_known_str(out, "false", [](){ return std::string("![]"); }, value))
  {
    return out;
  }
  
  // [object Array Iterator] === []["entries"]()
  if (from_known_str(out, "[object Array Iterator]", [](){ return std::string("[][") + str_repr("entries") + "]()"; }, value))
  {
    return out;
  }

  // function Number() { [native code] } === (0).constructor === (0)["constructor"]
  if (from_known_str(out, "function Number()", [](){ return std::string("(+[])[") + str_repr("constructor") + "]"; }, value))
  {
    return out;
  }

  // function Array() { [native code] } === (0).constructor === (0)["constructor"]
  if (from_known_str(out, "function Array()", [](){ return std::string("[][") + str_repr("constructor") + "]"; }, value))
  {
    return out;
  }

  // function String() { [native code] } === (0).constructor === (0)["constructor"]
  if (from_known_str(out, "function String()", [](){ return std::string("([]+[])[") + str_repr("constructor") + "]"; }, value))
  {
    return out;
  }

  // function Function() { [native code] } === (0).constructor.constructor === (0)["constructor"]["constructor"]
  if (from_known_str(out, "function Function()", [](){ return std::string("[][") + str_repr("constructor") + "][" + str_repr("constructor") + "]"; }, value))
  {
    return out;
  }

  // function Boolean() { [native code] } === (0).constructor === (0)["constructor"]
  if (from_known_str(out, "function Boolean()", [](){ return std::string("(![])[") + str_repr("constructor") + "]"; }, value))
  {
    return out;
  }

  // "a" === 10["toString"](11)
  // "b" === 11["toString"](12)...
  if (value >= 'a' && value <= 'z')
  {
    return "(" + number_repr(10 + value -'a') + ")[" + str_repr("toString") + "](" + number_repr(11 + value -'a') + ")";
  }

  // "C" == window["atob"]("20N")[1]
  // "C" === Function("return atob(\"20N\")[1]")()
  if (value == 'C')
  {
    return run_command("return atob(" + str_repr("20N") + ")[1]");
  }

  // ([]+[])["constructor"]["fromCharCode"](\d+)
  return std::string("([]+[])[")
      + str_repr("constructor") + "]["
      + str_repr("fromCharCode") + "](" + number_repr((int)value) + ")";
}

std::string str_repr(const char* str)
{
  std::string out {};
  for (auto it = str ; *it ; ++it)
  {
    if (it != str)
    {
      out += '+';
    }
    out += '(';
    out += char_repr(*it);
    out += ')';
  }
  return out;
}
std::string str_repr(std::string const& in)
{
  return str_repr(in.c_str());
}

std::string run_command(const char* str)
{
  return std::string("[][") + str_repr("constructor") + "][" + str_repr("constructor") + "](" + str_repr(str) + ")()";
}
std::string run_command(std::string const& in)
{
  return run_command(in.c_str());
}

