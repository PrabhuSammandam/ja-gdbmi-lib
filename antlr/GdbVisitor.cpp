#include "GdbVisitor.h"
#include "GdbMiResp.h"
#include <vector>

auto GdbVisitor::visitResult_class(GdbMiV1Parser::Result_classContext *ctx) -> antlrcpp::Any {
   auto resultClass = ctx->getText();

   if (resultClass == "done") {
      return (int)GdbMiResultRecord::RESULT_CLASS_DONE;
   }
   if (resultClass == "running") {
      return (int)GdbMiResultRecord::RESULT_CLASS_RUNNING;
   }
   if (resultClass == "connected") {
      return (int)GdbMiResultRecord::RESULT_CLASS_CONNECTED;
   }
   if (resultClass == "error") {
      return (int)GdbMiResultRecord::RESULT_CLASS_ERROR;
   }
   if (resultClass == "exit") {
      return (int)GdbMiResultRecord::RESULT_CLASS_EXIT;
   }
   return 0;
}

auto GdbVisitor::visitSimple_string(GdbMiV1Parser::Simple_stringContext *ctx) -> antlrcpp::Any { return ctx->getText(); }

auto GdbVisitor::visitValue(GdbMiV1Parser::ValueContext *ctx) -> antlrcpp::Any {
   GdbMiResult *r = nullptr;

   if (ctx->simple_string() != nullptr) {
      r         = new GdbMiResult{GdbMiResult::VALUE_TYPE_CONST};
      r->_const = visitSimple_string(ctx->simple_string()).as<std::string>();
   } else if (ctx->list() != nullptr) {
      r             = new GdbMiResult{GdbMiResult::VALUE_TYPE_LIST};
      r->_childrens = visitList(ctx->list()).as<std::vector<GdbMiResult *>>();
   } else if (ctx->tuple() != nullptr) {
      r             = new GdbMiResult{GdbMiResult::VALUE_TYPE_TUPLE};
      r->_childrens = visitTuple(ctx->tuple()).as<std::vector<GdbMiResult *>>();
   }

   return r;
}

auto GdbVisitor::visitResult_record(GdbMiV1Parser::Result_recordContext *ctx) -> antlrcpp::Any {
   std::vector<GdbMiResult *> resultList;
   for (auto *r : ctx->result()) {
      auto *robj = visitResult(r).as<GdbMiResult *>();
      resultList.push_back(robj);
   }
   auto  resultClass = visitResult_class(ctx->result_class()).as<int>();
   auto  token       = ctx->TOKEN() != nullptr ? ctx->TOKEN()->getText() : std::string{""};
   auto *resp        = new GdbMiResultRecord{resultClass, token, resultList};

   return resp;
}

auto GdbVisitor::visitResult(GdbMiV1Parser::ResultContext *ctx) -> antlrcpp::Any {
   auto *r = visitValue(ctx->value()).as<GdbMiResult *>();

   r->_variable = ctx->VARIABLE()->getText();

   return r;
}

auto GdbVisitor::visitTuple(GdbMiV1Parser::TupleContext *ctx) -> antlrcpp::Any {
   std::vector<GdbMiResult *> resList;

   for (auto *r : ctx->result()) {
      auto *robj = visitResult(r).as<GdbMiResult *>();
      resList.push_back(robj);
   }

   return resList;
}

auto GdbVisitor::visitList(GdbMiV1Parser::ListContext *ctx) -> antlrcpp::Any {
   std::vector<GdbMiResult *> resList;

   if (!ctx->result().empty()) {
      for (auto *r : ctx->result()) {
         auto *robj = visitResult(r).as<GdbMiResult *>();
         resList.push_back(robj);
      }
   } else if (!ctx->value().empty()) {
      for (auto *r : ctx->value()) {
         auto *robj = visitValue(r).as<GdbMiResult *>();
         resList.push_back(robj);
      }
   }

   return resList;
}

auto GdbVisitor::visitOutput(GdbMiV1Parser::OutputContext *ctx) -> antlrcpp::Any {

   GdbMiResultRecord *resultRecord = nullptr;
   if (ctx->result_record() != nullptr) {
      resultRecord = visitResult_record(ctx->result_record()).as<GdbMiResultRecord *>();
   }

   std::vector<GdbMiOutOfBandRecord *> oobRecordList;

   for (auto *oo : ctx->out_of_band_record()) {
      auto *oobObj = visitOut_of_band_record(oo).as<GdbMiOutOfBandRecord *>();
      oobRecordList.push_back(oobObj);
   }

   auto *out = new GdbMiOutput{oobRecordList, resultRecord};
   return out;
}

auto GdbVisitor::visitOut_of_band_record(GdbMiV1Parser::Out_of_band_recordContext *ctx) -> antlrcpp::Any {
   GdbMiOutOfBandRecord *out          = nullptr;
   auto *                asyncRecord  = ctx->async_record();
   auto *                streamRecord = ctx->stream_record();

   if (asyncRecord != nullptr) {
      out        = visitAsync_record(asyncRecord).as<GdbMiOutOfBandRecord *>();
      out->_type = GdbMiOutOfBandRecord::OOB_RECORD_TYPE_ASYNC;
   } //
   else if (streamRecord != nullptr) {
      out        = visitStream_record(streamRecord).as<GdbMiOutOfBandRecord *>();
      out->_type = GdbMiOutOfBandRecord::OOB_RECORD_TYPE_STREAM;
   }

   return out;
}

auto GdbVisitor::visitStream_record(GdbMiV1Parser::Stream_recordContext *ctx) -> antlrcpp::Any {
   GdbMiOutOfBandRecord *out     = nullptr;
   auto *                console = ctx->console_stream_output();
   auto *                target  = ctx->target_stream_output();
   auto *                log     = ctx->log_stream_output();

   if (console != nullptr) {
      out = visitConsole_stream_output(console).as<GdbMiStreamRecord *>();
   } else if (target != nullptr) {
      out = visitTarget_stream_output(target).as<GdbMiStreamRecord *>();
   } else if (log != nullptr) {
      out = visitLog_stream_output(log).as<GdbMiStreamRecord *>();
   }
   return out;
}

auto GdbVisitor::visitConsole_stream_output(GdbMiV1Parser::Console_stream_outputContext *ctx) -> antlrcpp::Any {
   auto  data = ctx->C_STRING() != nullptr ? ctx->C_STRING()->getText() : "";
   auto *out  = new GdbMiStreamRecord{GdbMiStreamRecord::STREAM_TYPE_CONSOLE, data};

   return out;
}

auto GdbVisitor::visitTarget_stream_output(GdbMiV1Parser::Target_stream_outputContext *ctx) -> antlrcpp::Any {
   auto  data = ctx->C_STRING() != nullptr ? ctx->C_STRING()->getText() : "";
   auto *out  = new GdbMiStreamRecord{GdbMiStreamRecord::STREAM_TYPE_TARGET, data};

   return out;
}

auto GdbVisitor::visitLog_stream_output(GdbMiV1Parser::Log_stream_outputContext *ctx) -> antlrcpp::Any {
   auto  data = ctx->C_STRING() != nullptr ? ctx->C_STRING()->getText() : "";
   auto *out  = new GdbMiStreamRecord{GdbMiStreamRecord::STREAM_TYPE_LOG, data};

   return out;
}

auto GdbVisitor::visitAsync_record(GdbMiV1Parser::Async_recordContext *ctx) -> antlrcpp::Any {
   GdbMiOutOfBandRecord *out         = nullptr;
   auto *                execAsync   = ctx->exec_async_output();
   auto *                statusAsync = ctx->status_async_output();
   auto *                notifyAsync = ctx->notify_async_output();

   if (execAsync != nullptr) {
      out = visitExec_async_output(execAsync).as<GdbMiAsyncRecord *>();
   } //
   else if (statusAsync != nullptr) {
      out = visitStatus_async_output(statusAsync).as<GdbMiAsyncRecord *>();
   } //
   else if (notifyAsync != nullptr) {
      out = visitNotify_async_output(notifyAsync).as<GdbMiAsyncRecord *>();
   }

   return out;
}

auto GdbVisitor::visitExec_async_output(GdbMiV1Parser::Exec_async_outputContext *ctx) -> antlrcpp::Any {
   auto *out   = visitAsync_output(ctx->async_output()).as<GdbMiAsyncRecord *>();
   out->_token = ctx->TOKEN() != nullptr ? ctx->TOKEN()->getText() : "";
   out->asyncType(GdbMiAsyncRecord::ASYNC_TYPE_EXEC);

   return out;
}

auto GdbVisitor::visitStatus_async_output(GdbMiV1Parser::Status_async_outputContext *ctx) -> antlrcpp::Any {
   auto *out   = visitAsync_output(ctx->async_output()).as<GdbMiAsyncRecord *>();
   out->_token = ctx->TOKEN() != nullptr ? ctx->TOKEN()->getText() : "";
   out->asyncType(GdbMiAsyncRecord::ASYNC_TYPE_STATUS);

   return out;
}

auto GdbVisitor::visitNotify_async_output(GdbMiV1Parser::Notify_async_outputContext *ctx) -> antlrcpp::Any {
   auto *out   = visitAsync_output(ctx->async_output()).as<GdbMiAsyncRecord *>();
   out->_token = ctx->TOKEN() != nullptr ? ctx->TOKEN()->getText() : "";
   out->asyncType(GdbMiAsyncRecord::ASYNC_TYPE_NOTIFY);

   return out;
}

auto GdbVisitor::visitAsync_output(GdbMiV1Parser::Async_outputContext *ctx) -> antlrcpp::Any {
   auto *out = new GdbMiAsyncRecord{};

   out->_asyncClass = visitAsync_class(ctx->async_class()).as<int>();

   for (auto *r : ctx->result()) {
      auto *robj = visitResult(r).as<GdbMiResult *>();
      out->_childrens.push_back(robj);
   }

   return out;
}

auto GdbVisitor::visitAsync_class(GdbMiV1Parser::Async_classContext *ctx) -> antlrcpp::Any {
   auto text = ctx->getText();

   int rc = -1;

   if ("stopped" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_STOPPED;
   } else if ("running" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_RUNNING;
   } else if ("thread-group-added" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_GROUP_ADDED;
   } else if ("thread-group-removed" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_GROUP_REMOVED;
   } else if ("thread-group-started" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_GROUP_STARTED;
   } else if ("thread-group-exited" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_GROUP_EXITED;
   } else if ("thread-created" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_CREATED;
   } else if ("thread-exited" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_EXITED;
   } else if ("thread-selected" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_THREAD_SELECTED;
   } else if ("library-loaded" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_LIBRARY_LOADED;
   } else if ("library-unloaded" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_LIBRARY_UNLOADED;
   } else if ("traceframe-changed" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_TRACEFRAME_CHANGED;
   } else if ("tsv-created" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_TSV_CREATED;
   } else if ("tsv-modified" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_TSV_MODIFIED;
   } else if ("tsv-deleted" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_TSV_DELETED;
   } else if ("breakpoint-created" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_BREAKPOINT_CREATED;
   } else if ("breakpoint-modified" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_BREAKPOINT_MODIFIED;
   } else if ("breakpoint-deleted" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_BREAKPOINT_DELETED;
   } else if ("record-started" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_RECORD_STARTED;
   } else if ("record-stopped" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_RECORD_STOPPED;
   } else if ("cmd-param-changed" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_CMD_PARAM_CHANGED;
   } else if ("memory-changed" == text) {
      rc = GdbMiAsyncRecord::ASYNC_CLASS_MEMORY_CHANGED;
   }
   return rc;
}
