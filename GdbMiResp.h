#pragma once
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// clang-format off

#define DECLARE_DEFAULT_CLASS(__x__) __x__(__x__ &other)  = delete;\
    __x__(__x__ &&other) = delete;\
    auto operator=(__x__ &&other) -> __x__ & = delete;\
    auto operator=(__x__ &other) -> __x__ & = delete;
// clang-format on

class GdbMiResult {

 public:
   enum { VALUE_TYPE_CONST, VALUE_TYPE_TUPLE, VALUE_TYPE_LIST };

   explicit GdbMiResult(int type);
   ~GdbMiResult() { clear(); }
   DECLARE_DEFAULT_CLASS(GdbMiResult);

   void clear();
   void print(int count);
   void print() { print(0); }

   int                        _type = VALUE_TYPE_CONST;
   std::string                _const;
   std::string                _variable;
   std::vector<GdbMiResult *> _childrens;
};

class GdbMiOutput {
 public:
   enum { GDB_MI_OUTPUT_OOB_RECORD, GDB_MI_OUTPUT_RESULT_RECORD, GDB_MI_OUTPUT_PROMPT };

   explicit GdbMiOutput(int type);
   virtual ~GdbMiOutput();
   DECLARE_DEFAULT_CLASS(GdbMiOutput);
   auto isOobRecord() const -> bool { return _type == GDB_MI_OUTPUT_OOB_RECORD; }
   auto isResultRecord() const -> bool { return _type == GDB_MI_OUTPUT_RESULT_RECORD; }
   auto isPrompt() const -> bool { return _type == GDB_MI_OUTPUT_PROMPT; }

   virtual void print();

 private:
   int _type = -1;
};

class GdbMiResultRecord : public GdbMiOutput {

 public:
   enum { RESULT_CLASS_DONE, RESULT_CLASS_RUNNING, RESULT_CLASS_CONNECTED, RESULT_CLASS_ERROR, RESULT_CLASS_EXIT };

   GdbMiResultRecord(int resultClass, std::string &token, const std::vector<GdbMiResult *> &resultList);
   ~GdbMiResultRecord() override;
   DECLARE_DEFAULT_CLASS(GdbMiResultRecord);

   void                       print() override;
   static auto                getResultClass(const std::string &resultStr) -> int;
   std::string                _token;
   int                        _resultClass = RESULT_CLASS_DONE;
   std::vector<GdbMiResult *> _childrens;
};

class GdbMiOutOfBandRecord : public GdbMiOutput {
 public:
   enum { OOB_RECORD_TYPE_ASYNC, OOB_RECORD_TYPE_STREAM };

   explicit GdbMiOutOfBandRecord(int type);
   virtual ~GdbMiOutOfBandRecord();
   DECLARE_DEFAULT_CLASS(GdbMiOutOfBandRecord);

   auto isAsyncRecord() const -> bool { return _type == OOB_RECORD_TYPE_ASYNC; }
   void print() override;
   int  _type = -1;
};

class GdbMiAsyncRecord : public GdbMiOutOfBandRecord {
 public:
   // clang-format off
   enum {
       ASYNC_TYPE_EXEC,
       ASYNC_TYPE_NOTIFY,
       ASYNC_TYPE_STATUS
   };
   enum {
       ASYNC_CLASS_UNKNOWN = -1,
       ASYNC_CLASS_RUNNING = 0,
       ASYNC_CLASS_STOPPED,
       ASYNC_CLASS_THREAD_GROUP_ADDED,
       ASYNC_CLASS_THREAD_GROUP_REMOVED,
       ASYNC_CLASS_THREAD_GROUP_STARTED,
       ASYNC_CLASS_THREAD_GROUP_EXITED,
       ASYNC_CLASS_THREAD_CREATED,
       ASYNC_CLASS_THREAD_EXITED,
       ASYNC_CLASS_THREAD_SELECTED,
       ASYNC_CLASS_LIBRARY_LOADED,
       ASYNC_CLASS_LIBRARY_UNLOADED,
       ASYNC_CLASS_TRACEFRAME_CHANGED,
       ASYNC_CLASS_TSV_CREATED,
       ASYNC_CLASS_TSV_DELETED,
       ASYNC_CLASS_TSV_MODIFIED,
       ASYNC_CLASS_BREAKPOINT_CREATED,
       ASYNC_CLASS_BREAKPOINT_MODIFIED,
       ASYNC_CLASS_BREAKPOINT_DELETED,
       ASYNC_CLASS_RECORD_STARTED,
       ASYNC_CLASS_RECORD_STOPPED,
       ASYNC_CLASS_CMD_PARAM_CHANGED,
       ASYNC_CLASS_MEMORY_CHANGED
   };
   // clang-format on

   GdbMiAsyncRecord();
   GdbMiAsyncRecord(int asyncType, std::string &token, int asyncClass, const std::vector<GdbMiResult *> &resultList);
   ~GdbMiAsyncRecord() override;
   DECLARE_DEFAULT_CLASS(GdbMiAsyncRecord);

   static auto getAsyncClass(const std::string &asyncStr) -> int;
   auto        asyncType() const -> int;
   auto        asyncType(int type) -> GdbMiAsyncRecord &;
   auto        asyncClass() const -> int;
   auto        asyncClass(int asyncClass) -> GdbMiAsyncRecord &;
   auto        token(std::string &token) -> GdbMiAsyncRecord &;
   auto        token() const -> std::string;

   void print() override;

   std::string                _token;
   int                        _asyncClass = ASYNC_CLASS_UNKNOWN;
   std::vector<GdbMiResult *> _childrens;

 private:
   int _asyncType = -1;
};

class GdbMiStreamRecord : public GdbMiOutOfBandRecord {
 public:
   enum { STREAM_TYPE_CONSOLE, STREAM_TYPE_TARGET, STREAM_TYPE_LOG };

   explicit GdbMiStreamRecord(int streamType);
   GdbMiStreamRecord(int streamType, std::string &data);
   ~GdbMiStreamRecord() override = default;

   DECLARE_DEFAULT_CLASS(GdbMiStreamRecord);

   void print() override;

   int         _streamType = -1;
   std::string _data;
};
