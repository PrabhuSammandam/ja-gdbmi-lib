%baseclass-preinclude "GdbMiResp.h"
%class-name GdbMiParser
%error-verbose
%no-lines
%scanner-token-function d_scanner->lex()

%polymorphic INT             : int ;
             STRING          : std::string;
             OOB_RECORD      : GdbMiOutOfBandRecord*;
             OOB_RECORD_LIST : std::vector<GdbMiOutOfBandRecord *>;
             RESULT_RECORD   : GdbMiResultRecord*;
             RESULT          : GdbMiResult*;
             RESULT_LIST     : std::vector<GdbMiResult *>;
             OUTPUT          : GdbMiOutput*;

%token INTEGER_LITERAL
%token OPEN_BRACE     /* { */
%token CLOSED_BRACE   /* } */
%token OPEN_PAREN     /* ( */
%token CLOSED_PAREN   /* ) */
%token ADD_OP         /* + */
%token MULT_OP        /* * */
%token EQUAL_SIGN     /* = */
%token TILDA          /* ~ */
%token AT_SYMBOL      /* @ */
%token AMPERSAND      /* & */
%token OPEN_BRACKET   /* [ */
%token CLOSED_BRACKET /* ] */
%token NEWLINE        /* \n \r\n \r */
%token STRING_LITERAL /* A string literal */
%token CSTRING        /* "a string like \" this " */
%token COMMA          /* , */
%token CARROT         /* ^ */

%type <OOB_RECORD>       log_stream_output target_stream_output console_stream_output stream_record exec_async_output
%type <OOB_RECORD>       status_async_output notify_async_output async_record out_of_band_record
%type <RESULT>           const list value tuple result
%type <RESULT_LIST>      value_result_list result_list value_list
%type <STRING>           result_class async_class variable token optional_token cstring
%type <OOB_RECORD_LIST>  out_of_band_record_list
%type <RESULT_RECORD>    result_record optional_result_record
%type <OUTPUT>           output

%start output

%%

output : out_of_band_record_list optional_result_record OPEN_PAREN variable CLOSED_PAREN NEWLINE { $$ = setOutput($1, $2); };

optional_result_record  :                                                                  { $$ = (GdbMiResultRecord*)nullptr; };
optional_result_record  : result_record                                                    { $$ = $1; };
result_record           : optional_token CARROT result_class result_list NEWLINE           { $$ = createResultRecord($3, $1, $4); };
out_of_band_record_list :                                                                  { $$ = std::vector<GdbMiOutOfBandRecord *>{};};
out_of_band_record_list : out_of_band_record_list out_of_band_record                       { $1.push_back($2); $$ = std::move($1); };
out_of_band_record      : async_record | stream_record                                     { $$ =  $1;};
async_record            : exec_async_output | status_async_output | notify_async_output    { $$ = $1;};
exec_async_output       : optional_token MULT_OP async_class result_list NEWLINE           { $$ = createExecAsyncOutput($1, $3, $4);};
status_async_output     : optional_token ADD_OP async_class result_list NEWLINE            { $$ = createStatusAsyncOutput($1, $3, $4);};
notify_async_output     : optional_token EQUAL_SIGN async_class result_list NEWLINE        { $$ = createNotifyAsyncOutput($1, $3, $4);};
result_list             :                                                                  { $$ = std::vector<GdbMiResult *>{}; };
result_list             : result_list COMMA result                                         { $1.push_back($3); $$ = std::move($1); };
result_class            : STRING_LITERAL                                                   { $$ = d_scanner->matched();};
async_class             : STRING_LITERAL                                                   { $$ = d_scanner->matched();};
result                  : variable EQUAL_SIGN value                                        { $$ = $3;   $$->_variable = $1; };
variable                : STRING_LITERAL                                                   { $$ = std::string{d_scanner->matched()};};
value                   : const | tuple | list                                             { $$ = $1;};
const                   : cstring                                                          { $$ = createConst($1);};
value_list              : value                                                            { $$ = std::vector<GdbMiResult *>{};   $$.push_back($1);};
value_list              : value_list COMMA value                                           { $1.push_back($3); $$ = $1;};
value_result_list       : result                                                           { $$ = std::vector<GdbMiResult *>{}; $$.push_back($1);};
value_result_list       :  value_result_list COMMA result                                  { $1.push_back($3);   $$ = $1;};
tuple                   : OPEN_BRACE CLOSED_BRACE                                          { $$ = createTuple(); };
tuple                   : OPEN_BRACE value_result_list CLOSED_BRACE                        { $$ = createTuple($2); };
list                    : OPEN_BRACKET CLOSED_BRACKET                                      { $$ = createList();};
list                    : OPEN_BRACKET value_list CLOSED_BRACKET                           { $$ = createList($2);};
list                    : OPEN_BRACKET value_result_list CLOSED_BRACKET                    { $$ = createList($2);};
stream_record           : console_stream_output | target_stream_output | log_stream_output { $$ = $1;};
console_stream_output   : TILDA cstring NEWLINE                                            { $$ = createConsoleStreamOutput($2);};
target_stream_output    : AT_SYMBOL cstring NEWLINE                                        { $$ = createTargetStreamOutput($2);};
log_stream_output       : AMPERSAND cstring NEWLINE                                        { $$ = createLogStreamOutput($2);};
cstring                 : CSTRING                                                          { $$ = d_scanner->matched(); };
optional_token          :                                                                  { $$ = std::string{}; } | token { $$ = $1; };
token                   : INTEGER_LITERAL                                                  { $$ = d_scanner->matched(); };
