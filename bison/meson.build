project('gdbparser', 'c','cpp',
        default_options : ['cpp_std=c++11', 'buildtype=debug'])

srcs=files([
  'main.cpp',
])

root_inc        = include_directories(['.' , '..'])

flex = find_program('flex', required: false)
bison = find_program('bison', required: false)

if not flex.found()
  error('MESON_SKIP_TEST flex not found.')
endif

if not bison.found()
  error('MESON_SKIP_TEST bison not found.')
endif

gdb_mi_grammer_files = custom_target(
  'GdbMiGrammer',
  output : ['GdbMiParser.cpp', 'GdbMiParser.hpp', 'location.hh'],
  input : 'GdbMiGrammer.g',
  command : [bison, '@INPUT@', '--defines', '--output=@OUTPUT0@'],
)

gdb_mi_lexer_files = custom_target(
  'GdbMiLexer',
  output : ['GdbMiLexer.cpp'],
  input : 'GdbMiLexer.l',
  command : [flex ,'--outfile=@OUTPUT0@', '@INPUT@'],
)

gdb_mi_parser_lib = static_library('GdbMiParser',
                                     'GdbMiDriver.cpp',
                                     'GdbMiScanner.hpp',
                                     '../GdbMiResp.cpp',
                                     '../GdbMiResp.h',
                                     [gdb_mi_grammer_files],
                                     [gdb_mi_lexer_files],
                                     include_directories : [root_inc])

e = executable('gdbparser',
               [srcs],
               include_directories : [root_inc],
               link_with: [gdb_mi_parser_lib])

