#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "js6chars.hpp"

static void display_help()
{
  std::cout << "------ HELP ------" << std::endl;
  std::cout << std::endl;
  std::cout << "js6chars makes it possible to convert JavaScript data into" <<std::endl;
  std::cout << "their equivalent versions using only a 6-character subset: []+()!" << std::endl;
  std::cout << std::endl;
  std::cout << "js6chars syntax:" << std::endl;
  std::cout << "-c <character> : string of one character" << std::endl;
  std::cout << "-n <number>    : number" << std::endl;
  std::cout << "-s <script>    : script" << std::endl;
  std::cout << "-t <text>      : string" << std::endl;
}

static void display_unknown_parameter(std::string const& param)
{
  std::cout << "Unknown parameter: " << param << std::endl;
  std::cout << std::endl;
  display_help();
}

int main(int argc, char** argv)
{
  std::vector<std::string> params;
  std::copy(argv+1, argv+argc, std::back_inserter(params));
  
  if (params.size() == 2)
  {
    if (params[0].size() != 2 || params[0][0] != '-')
    {
      display_unknown_parameter(params[0]);
    }
    else
    {
      switch (params[0][1])
      {
        case 'c':
          if (params[1].size() != 1) { std::cout << "-c must be followed by one unisue character" << std::endl; break; }
          std::cout << char_repr(params[1][0]) << std::endl;
          break;
        case 'n': std::cout << number_repr(std::stoi(params[1])) << std::endl; break;
        case 's': std::cout << run_command(params[1]) << std::endl; break;
        case 't': std::cout << str_repr(params[1]) << std::endl; break;
        default : display_unknown_parameter(params[0]); break;
      }
    }
  }
  else
  {
    display_help();
  }
  return 0;
}

