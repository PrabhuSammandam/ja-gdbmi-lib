/*
The bison whether c++ or c requires the yylex function. There are many forms of yylex with different arguments type.
First identify the prototype of the yylex function by running the bison and check in the output c or cpp file.

in generated code the yylex is not defined or declared as function. It is just the macro name. we need #define the yylex macro with real
function name only.

Say for example in the generated code the yylex was like 
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));

So we need to declare and define a function with arguments same as above yylex call.
            int my_yylex( JA::GdbMiParser::semantic_type * const lval, JA::GdbMiParser::location_type *location );

Then we need to tell the bison that yylex is replaced by our function name my_yylex like below
#undef yylex
#define yylex my_yylex

This is the first toughest part in using the bison. we can define the grammers and we will stuck on how to test and where to give input to test.
*/

%skeleton "lalr1.cc"
%require "3.2"
%language "c++"
%defines
%no-lines
%define api.value.type variant
%define api.namespace {JA}
%define api.parser.class {   GdbMiParser }
%define parse.error verbose
%define parse.trace
%locations
%define parse.lac full

%code requires { namespace JA { class GdbMiScanner; class GdbMiDriver; } }

%parse-param{JA::GdbMiScanner & scanner}
%parse-param{JA::GdbMiDriver & driver}

%code top {
#include "GdbMiResp.h"
#include "GdbMiScanner.hpp"
#include "GdbMiDriver.hpp"

#undef yylex
#define yylex scanner.yylex
}

%token OPEN_BRACE /* { */ CLOSED_BRACE /* } */ OPEN_PAREN /* ( */ CLOSED_PAREN /* ) */ OPEN_BRACKET /* [ */ CLOSED_BRACKET  /* ] */
%token PLUS          /* + */
%token ASTERISK         /* * */
%token EQUAL      /* = */
%token TILDE           /* ~ */
%token AT_SYMBOL       /* @ */
%token AMPERSAND       /* & */
%token NL              /* \n \r\n \r */
%token INTEGER_LITERAL /* A number 1234 */
%token STRING_LITERAL  /* A string literal */
%token CSTRING         /* "a string like \" this " */
%token COMMA           /* , */
%token CARET          /* ^ */

%type<std::string>                          STRING_LITERAL CSTRING INTEGER_LITERAL

%nterm<std::string>                         optionalToken token resultClass asyncClass variable
%nterm<GdbMiResult *>                       result value const tuple list
%nterm<std::vector<GdbMiResult *>>          resultList valueList
%nterm<GdbMiOutOfBandRecord *>              streamRecord asyncRecord outOfBandRecord logStreamRecord targetStreamRecord consoleStreamRecord
%nterm<GdbMiOutOfBandRecord *>              execAsyncRecord statusAsyncRecord notifyAsyncRecord
%nterm<std::vector<GdbMiOutOfBandRecord *>> outOfBandRecordList
%nterm<GdbMiResultRecord *>                 resultRecord optionalResultRecord
%nterm<GdbMiOutput *>                       output

%start output

%%
output                  : outOfBandRecordList optionalResultRecord OPEN_PAREN STRING_LITERAL CLOSED_PAREN NL { $$ = driver.setOutput($1, $2);};
optionalResultRecord    : %empty                                                     { $$ = nullptr; };
optionalResultRecord    : resultRecord                                               { $$ = $1; };
resultRecord            : optionalToken CARET resultClass resultList NL              { $$ = driver.createResultRecord($1, $3, $4); }
resultClass             : STRING_LITERAL                                             { $$ = $1; }        
outOfBandRecordList     : %empty                                                     { $$ = std::vector<GdbMiOutOfBandRecord *>{}; };
outOfBandRecordList     : outOfBandRecordList outOfBandRecord                        { $$ = std::move($1);   $$.push_back($2); };
outOfBandRecord         : asyncRecord | streamRecord                                 { $$ = $1; }
asyncRecord             : execAsyncRecord | statusAsyncRecord | notifyAsyncRecord    { $$ = $1; }
execAsyncRecord         : optionalToken ASTERISK    asyncClass resultList  NL        { $$ = driver.createExecAsyncOutput($1, $3, $4); };
statusAsyncRecord       : optionalToken PLUS     asyncClass resultList  NL           { $$ = driver.createStatusAsyncOutput($1, $3, $4); };
notifyAsyncRecord       : optionalToken EQUAL asyncClass resultList  NL              { $$ = driver.createNotifyAsyncOutput($1, $3, $4); };
asyncClass              : STRING_LITERAL                                             { $$ = $1; }        
result                  : variable  EQUAL value                                      { $$ = $3; $$->_variable = $1;};
variable                : STRING_LITERAL                                             { $$ = $1; }        
resultList              : %empty                                                     { $$ = std::vector<GdbMiResult *>{}; };
resultList              : resultList COMMA result                                    { $$ = std::move($1); $$.push_back($3);};
valueList               : %empty                                                     { $$ = std::vector<GdbMiResult *>{}; };
valueList               : valueList COMMA value                                      { $$ = std::move($1); $$.push_back($3);};
value                   : const | tuple | list                                       { $$ = $1; }
const                   : CSTRING                                                    { $$ = driver.createConst($1); };
tuple                   : OPEN_BRACE                      CLOSED_BRACE               { $$ = driver.createTuple(); };
tuple                   : OPEN_BRACE result resultList   CLOSED_BRACE                { $$ = driver.createTuple($2, $3); };
list                    : OPEN_BRACKET                    CLOSED_BRACKET             { $$ = driver.createList(); };
list                    : OPEN_BRACKET value valueList   CLOSED_BRACKET              { $$ = driver.createList($2, $3); };
list                    : OPEN_BRACKET result resultList CLOSED_BRACKET              { $$ = driver.createList($2, $3); };
streamRecord            : consoleStreamRecord | targetStreamRecord | logStreamRecord { $$ = $1; } 
consoleStreamRecord     : TILDE     CSTRING       NL                                 { $$ = driver.createConsoleStreamOutput($2); };
targetStreamRecord      : AT_SYMBOL CSTRING       NL                                 { $$ = driver.createTargetStreamOutput($2); };
logStreamRecord         : AMPERSAND CSTRING       NL                                 { $$ = driver.createLogStreamOutput($2); };
optionalToken           : %empty                                                     { $$ = std::string{}; }
optionalToken           : token                                                      { $$ = $1;};
token                   : INTEGER_LITERAL                                            { $$ = $1;};

%%
void JA::GdbMiParser::error(const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
