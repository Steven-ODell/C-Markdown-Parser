#pragma once 
#include <string>
#include <fstream>
#include <sstream>


inline std::string load_file() {
  std::ifstream file("/home/saoii/cpp_proj/mdparser1/inputfile.md");
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string source = buffer.str();
  return source;
}
