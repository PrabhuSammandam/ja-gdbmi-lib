#pragma once
#include "GdbMiV1BaseVisitor.h"

class GdbVisitor : public GdbMiV1BaseVisitor {
 public:
   GdbVisitor() = default;

   auto visitResult_record(GdbMiV1Parser::Result_recordContext *ctx) -> antlrcpp::Any override;
   auto visitResult_class(GdbMiV1Parser::Result_classContext *ctx) -> antlrcpp::Any override;
   auto visitResult(GdbMiV1Parser::ResultContext *ctx) -> antlrcpp::Any override;
   auto visitValue(GdbMiV1Parser::ValueContext *ctx) -> antlrcpp::Any override;
   auto visitSimple_string(GdbMiV1Parser::Simple_stringContext *ctx) -> antlrcpp::Any override;
   auto visitList(GdbMiV1Parser::ListContext *ctx) -> antlrcpp::Any override;
   auto visitTuple(GdbMiV1Parser::TupleContext *ctx) -> antlrcpp::Any override;

   auto visitOutput(GdbMiV1Parser::OutputContext *ctx) -> antlrcpp::Any override;
   auto visitOut_of_band_record(GdbMiV1Parser::Out_of_band_recordContext *ctx) -> antlrcpp::Any override;
   auto visitAsync_record(GdbMiV1Parser::Async_recordContext *ctx) -> antlrcpp::Any override;
   auto visitExec_async_output(GdbMiV1Parser::Exec_async_outputContext *ctx) -> antlrcpp::Any override;
   auto visitStatus_async_output(GdbMiV1Parser::Status_async_outputContext *ctx) -> antlrcpp::Any override;
   auto visitNotify_async_output(GdbMiV1Parser::Notify_async_outputContext *ctx) -> antlrcpp::Any override;
   auto visitAsync_output(GdbMiV1Parser::Async_outputContext *ctx) -> antlrcpp::Any override;
   auto visitAsync_class(GdbMiV1Parser::Async_classContext *ctx) -> antlrcpp::Any override;
   auto visitStream_record(GdbMiV1Parser::Stream_recordContext *ctx) -> antlrcpp::Any override;
   auto visitConsole_stream_output(GdbMiV1Parser::Console_stream_outputContext *ctx) -> antlrcpp::Any override;
   auto visitTarget_stream_output(GdbMiV1Parser::Target_stream_outputContext *ctx) -> antlrcpp::Any override;
   auto visitLog_stream_output(GdbMiV1Parser::Log_stream_outputContext *ctx) -> antlrcpp::Any override;
};
