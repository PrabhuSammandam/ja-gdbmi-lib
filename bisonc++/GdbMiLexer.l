%class-name GdiMiLexer
%no-lines

DIGIT       [0-9]
L           [a-zA-Z_]
T           [0-9a-zA-Z_-]
IDENTIFIER  {L}+{T}*

%%
"^"                     {  return GdbMiParserBase::CARROT; }
","                     {  return GdbMiParserBase::COMMA; }
"+"                     {  return GdbMiParserBase::ADD_OP; }
"*"                     {  return GdbMiParserBase::MULT_OP; }
"="                     {  return GdbMiParserBase::EQUAL_SIGN; }
"~"                     {  return GdbMiParserBase::TILDA; }
"@"                     {  return GdbMiParserBase::AT_SYMBOL; }
"&"                     {  return GdbMiParserBase::AMPERSAND; }
"["                     {  return GdbMiParserBase::OPEN_BRACKET; }
"]"                     {  return GdbMiParserBase::CLOSED_BRACKET; }
"{"                     {  return GdbMiParserBase::OPEN_BRACE; }
"}"                     {  return GdbMiParserBase::CLOSED_BRACE; }
"("                     {  return GdbMiParserBase::OPEN_PAREN; }
")"                     {  return GdbMiParserBase::CLOSED_PAREN; }

\n                      {  return GdbMiParserBase::NEWLINE; }
\r\n                    {  return GdbMiParserBase::NEWLINE; }
\r                      {  return GdbMiParserBase::NEWLINE; }

{DIGIT}+                {  return GdbMiParserBase::INTEGER_LITERAL; }

[ \t\v\f]               {}
{IDENTIFIER}            {  return GdbMiParserBase::STRING_LITERAL;    }
.                       {  return GdbMiParserBase::STRING_LITERAL;    }
\"(\\.|[^\\"])*\"       {  return GdbMiParserBase::CSTRING; }
