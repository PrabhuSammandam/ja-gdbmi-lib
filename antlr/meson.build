project('GdbMiParser', 'c','cpp',
        default_options : ['cpp_std=c++11', 'buildtype=debug'])

antlr = find_program('antlr4')

srcs=files([
  'test.cpp',
  'GdbVisitor.cpp',
  '../GdbMiResp.cpp',
  '../GdbMiResp.h'
])

root_inc        = include_directories(['.','..'])
antlr_inc        = include_directories('/usr/local/include/antlr4-runtime')
local_inc        = include_directories('/usr/local/include')

ccomp = meson.get_compiler('c')
m_dep = ccomp.find_library('antlr4-runtime', required : false)

parser_input = files('GdbMiV1.g4')
parser_cxx_outputs = ['GdbMiV1Lexer.h',
                      'GdbMiV1Lexer.cpp',
                      'GdbMiV1Parser.h',
                      'GdbMiV1Parser.cpp',
                      'GdbMiV1BaseVisitor.h',
                      'GdbMiV1BaseVisitor.cpp',
                      'GdbMiV1Visitor.h',
                      'GdbMiV1Visitor.cpp']
parser_outputs = ['GdbMiV1Lexer.interp', 'Lexer.tokens']
parser_outputs += parser_cxx_outputs

parser = custom_target('gen-parser',
        input: parser_input,
        output: parser_outputs,
        command: [antlr,
                '@INPUT@',
                '-o', '@OUTDIR@',
                '-no-listener',
                '-Dlanguage=Cpp',
                '-visitor',
                '-Xexact-output-dir'])

parser_lib = static_library('GdbMiParser', parser, include_directories: [antlr_inc], dependencies: [m_dep])
parser_dep = declare_dependency(sources: [parser[2], parser[4], parser[6], parser[8]], link_with: [parser_lib])

executable('GdbMiParser',
           srcs,
           include_directories : [root_inc, antlr_inc, local_inc],
           dependencies: [m_dep, parser_dep],
           link_with : []
           )
