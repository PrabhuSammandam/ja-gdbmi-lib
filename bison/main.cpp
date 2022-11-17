#include <cstdio>
#include <sstream>
#include <string>
#include <cassert>
#include <cstring>
#include <array>
#include <iostream>
#include <streambuf>
#include <sys/stat.h>
#include <FlexLexer.h>
#include "GdbMiDriver.hpp"
#include "GdbMiParser.hpp"

auto readFileIntoString3(const std::string &path) -> std::string {
   struct stat sb {};

   std::string res;

   FILE *input_file = fopen(path.c_str(), "r");

   if (input_file == nullptr) {
      perror("fopen");
   }

   stat(path.c_str(), &sb);
   res.resize(sb.st_size);
   fread(const_cast<char *>(res.data()), sb.st_size, 1, input_file);
   fclose(input_file);

   return (res);
}

auto main() -> int {
   std::string file = "/home/psammandam/disk/projects/gdbmi/i1.txt";
   std::string input;

   input = readFileIntoString3(file);

   JA::GdbMiDriver d{};

   auto *out = d.parse(input);

   if (out != nullptr) {
      out->print();
      delete out;
   }

   return (0);
}
