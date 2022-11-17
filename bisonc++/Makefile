GdbMiParser : parse.cc main.cpp lex.cc
	g++ --std=c++17 -I .. -o GdbMiParser parse.cc main.cpp lex.cc ../GdbMiResp.cpp

parse.cc : GdbMiGrammer.g
	bisonc++  GdbMiGrammer.g
lex.cc : GdbMiLexer.l
	flexc++  GdbMiLexer.l
