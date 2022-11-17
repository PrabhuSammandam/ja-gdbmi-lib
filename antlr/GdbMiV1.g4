grammar GdbMiV1;

output                   : out_of_band_record*  result_record?  '(gdb)'  NL;

result_record            : TOKEN? '^' result_class ( ',' result )* NL;
result_class             : 'done' | 'running' | 'connected' | 'error' | 'exit';

out_of_band_record       : async_record | stream_record;

async_record             : exec_async_output | status_async_output | notify_async_output;

exec_async_output        : TOKEN? '*' async_output NL;
status_async_output      : TOKEN? '+' async_output NL;
notify_async_output      : TOKEN? '=' async_output NL;

async_output             : async_class ( ',' result )*;

async_class              : VARIABLE;

stream_record            : console_stream_output | target_stream_output | log_stream_output;
console_stream_output    : '~' C_STRING NL;
target_stream_output     : '@' C_STRING NL;
log_stream_output        : '&' C_STRING NL;

result                   : VARIABLE '=' value;
value                    : simple_string | tuple | list;
simple_string            : C_STRING;
tuple                    : '{}' | '{' result ( ',' result )* '}';
list                     : '[]' | '[' value ( ',' value )* ']' | '[' result ( ',' result )* ']';

C_STRING : '"' (~('"' | '\\' | '\r' | '\n') | '\\' ('"' | '\\'))* '"';
VARIABLE : [a-zA-Z_-]+;        
NL       : '\n' | '\r\n';
TOKEN    : [0-9]+;

