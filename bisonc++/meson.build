project('GdbMiParser', 'c','cpp',
        default_options : ['cpp_std=c++11', 'buildtype=debug'])

srcs=files([
  'main.cpp',
])

root_inc        = include_directories(['.' , '..'])

flex = find_program('flexc++', required: false)
bison = find_program('bisonc++', required: false)

if not flex.found()
  error('MESON_SKIP_TEST flex not found.')
endif

if not bison.found()
  error('MESON_SKIP_TEST bison not found.')
endif

gdb_mi_grammer_files = custom_target(
  'GdbMiGrammer',
  output : ['parse.cc', 'GdbMiParser.h', 'GdbMiParser.ih', 'GdbMiParserbase.h'],
  input : 'GdbMiGrammer.g',
  command : [bison, '@INPUT@', '--filenames=@OUTPUT0@'],
)

gdb_mi_lexer_files = custom_target(
  'GdbMiLexer',
  output : ['lex.cc', 'GdbMiLexer.h', 'GdbMiLexer.ih', 'GdbMiLexerbase.h'],
  input : 'GdbMiLexer.l',
  command : [flex , '--baseclass-header=GdbMiLexerBase.h', '@INPUT@'],
)

gdb_mi_parser_lib = static_library('GdbMiParser',
                                     '../GdbMiResp.cpp',
                                     '../GdbMiResp.h',
                                     [gdb_mi_grammer_files],
                                     [gdb_mi_lexer_files],
                                     include_directories : [root_inc])

e = executable('GdbMiParser',
               [srcs],
               include_directories : [root_inc],
               link_with: [gdb_mi_parser_lib])

