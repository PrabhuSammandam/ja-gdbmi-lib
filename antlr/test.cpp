#include <antlr4-runtime/antlr4-runtime.h>
#include <antlr4-runtime/CommonTokenStream.h>
#include "GdbMiV1Parser.h"
#include "GdbMiV1Lexer.h"
#include <iostream>
#include <ostream>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "GdbMiV1BaseVisitor.h"
#include "GdbVisitor.h"
#include "GdbMiResp.h"

using namespace std;
using namespace antlr4;

auto readFileIntoString3(const string &path) -> string {
   struct stat sb {};
   string      res;

   FILE *input_file = fopen(path.c_str(), "r");
   if (input_file == nullptr) {
      perror("fopen");
   }

   stat(path.c_str(), &sb);
   res.resize(sb.st_size);
   fread(const_cast<char *>(res.data()), sb.st_size, 1, input_file);
   fclose(input_file);

   return res;
}

auto main(int argc, char *argv[]) -> int {
   //   if (argc < 2) {
   //      std::cout << "input file name" << std::endl;
   //      return 0;
   //   }
   //    std::string file = argv[1];
   std::string file = "/home/psammandam/disk/projects/gdbmi/i1.txt";
   std::string input;

   input = readFileIntoString3(file);

   std::cout << input << std::endl;

   ANTLRInputStream  i(input);
   GdbMiV1Lexer      lexer{&i};
   CommonTokenStream tokens(&lexer);
   GdbMiV1Parser     parser(&tokens);

   auto *tree = parser.output();
   std::cout << tree->toStringTree() << std::endl << std::flush;

   GdbVisitor v;
   auto *     resp = v.visitOutput(tree).as<GdbMiOutput *>();

   resp->print();
   return 0;
}
