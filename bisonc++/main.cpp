#include "GdiMiLexer.h"
#include <cstdio>
#include <sstream>
#include <string>
#include <cassert>
#include <cstring>
#include <array>
#include <iostream>
#include <streambuf>
#include <sys/stat.h>
#include "GdbMiParser.h"

auto readFileIntoString3( const std::string &path ) -> std::string
{
   struct stat sb {};

   std::string res;

   FILE *      input_file = fopen( path.c_str(), "r" );

   if( input_file == nullptr )
   {
      perror( "fopen" );
   }

   stat( path.c_str(), &sb );
   res.resize( sb.st_size );
   fread( const_cast<char *>( res.data() ), sb.st_size, 1, input_file );
   fclose( input_file );

   return ( res );
}

auto main() -> int
{
   // char buf[1024];
   // std::snprintf(&buf[0], 1024, "%s=[\"%s\"]", "test", "a");
   // std::string input = "& test";
   // std::string input = &buf[0];

   std::string file = "/home/psammandam/disk/projects/gdbmi/i2.txt";
   std::string input;

   input = readFileIntoString3( file );
   std::cout << input << std::endl;

   GdbMiParser p{};

   auto *output = p.parse(input);

   if( output != nullptr )
   {
      output->print();
   }

   return ( 0 );
}
