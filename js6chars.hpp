#include <string>

// OUTPUT is a Number

// Convert a positive int to its JavaScript Number representation
// using only characters []+
std::string number_repr(int value);

// OUTPUT is a String

// Convert any character into its JavaScript String (of one character) equivalent
// using only characters []+()!
// (!) does not work with char<0
std::string char_repr(char value);
// Convert any cstring to its JavaScript String equivalent
std::string str_repr(const char* str);
std::string str_repr(std::string const& in);

// OUTPUT is a Script

// Convert any cstring to its JavaScript script equivalent
// code can be copy-pasted into a JavaScript REPL and it will behave the same as str itself
std::string run_command(const char* str);
std::string run_command(std::string const& in);

