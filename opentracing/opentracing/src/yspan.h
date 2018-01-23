#pragma once

#include <opentracing/span.h>
#include <opentracing/tracer.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <opentracing/util.h>
//#include "lightstep-tracer-common/collector.pb.h"
#include "yspan_context.h"
//#include "logger.h"
//#include "recorder.h"

namespace YYOT {
//Reference
class Reference{
public:
    uint64_t traceid;
    uint64_t spanid;
    opentracing::SpanReferenceType type;
};
//Log
class LogItem{
public:
    opentracing::SystemTime timestamp;
    std::vector<std::pair<opentracing::string_view, opentracing::Value>> fields;
};
    
class YSpan : public opentracing::Span {
 public:
  YSpan(std::shared_ptr<const opentracing::Tracer>&& tracer,
                /*Logger& logger, Recorder& recorder,*/
                opentracing::string_view operation_name,
                const opentracing::StartSpanOptions& options);

  YSpan(const YSpan&) = delete;
  YSpan(YSpan&&) = delete;
  YSpan& operator=(const YSpan&) = delete;
  YSpan& operator=(YSpan&&) = delete;

  ~YSpan() override;

  void FinishWithOptions(
      const opentracing::FinishSpanOptions& options) noexcept override;

  void SetOperationName(opentracing::string_view name) noexcept override;

  void SetTag(opentracing::string_view key,
              const opentracing::Value& value) noexcept override;

  void SetBaggageItem(opentracing::string_view restricted_key,
                      opentracing::string_view value) noexcept override;

  std::string BaggageItem(opentracing::string_view restricted_key) const
      noexcept override;

  void Log(std::initializer_list<
           std::pair<opentracing::string_view, opentracing::Value>>
               fields) noexcept override;

  const opentracing::SpanContext& context() const noexcept override {
    return span_context_;
  }
  const opentracing::Tracer& tracer() const noexcept override {
    return *tracer_;
  }

 private:
  // Fields set in StartSpan() are not protected by a mutex.
  std::shared_ptr<const opentracing::Tracer> tracer_;
//  Logger& logger_;
//  Recorder& recorder_;
  std::vector<Reference> references_;
  std::chrono::system_clock::time_point start_timestamp_;
  std::chrono::steady_clock::time_point start_steady_;
  std::chrono::steady_clock::time_point finish_steady_;
  YSpanContext span_context_;

  std::atomic<bool> is_finished_{false};

  // Mutex protects tags_, logs_, and operation_name_.
  std::mutex mutex_;
  std::string operation_name_;
  std::unordered_map<std::string, opentracing::Value> tags_;
  std::vector<LogItem> logs_;
};
}  // namespace lightstep
