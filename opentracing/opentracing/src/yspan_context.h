#pragma once

#include <opentracing/span.h>
#include <opentracing/string_view.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include "ypropagation.h"

namespace YYOT {
class YSpanContext : public opentracing::SpanContext {
 public:
  YSpanContext() = default;

  YSpanContext(
      std::string trace_id, std::string span_id,
      std::unordered_map<std::string, std::string>&& baggage) noexcept;

  YSpanContext(const YSpanContext&) = delete;
  YSpanContext(YSpanContext&&) = delete;

  ~YSpanContext() override = default;

  YSpanContext& operator=(YSpanContext&) = delete;
  YSpanContext& operator=(YSpanContext&& other) noexcept;

  void set_baggage_item(opentracing::string_view key,
                        opentracing::string_view value) noexcept;

  std::string baggage_item(opentracing::string_view key) const;

  void ForeachBaggageItem(
      std::function<bool(const std::string& key, const std::string& value)> f)
      const override;

  template <class Carrier>
  opentracing::expected<void> Inject(
      const PropagationOptions& propagation_options, Carrier& writer) const {
    std::lock_guard<std::mutex> lock_guard{baggage_mutex_};
    return InjectSpanContext(propagation_options, writer, trace_id_, span_id_,
                             baggage_);
  }

  template <class Carrier>
  opentracing::expected<bool> Extract(
      const PropagationOptions& propagation_options, Carrier& reader) {
    std::lock_guard<std::mutex> lock_guard{baggage_mutex_};
    return ExtractSpanContext(propagation_options, reader, trace_id_, span_id_,
                              baggage_);
  }

  std::string trace_id() const noexcept { return trace_id_; }
  std::string span_id() const noexcept { return span_id_; }

 private:
  std::string trace_id_ = "";
  std::string span_id_ = "";

  mutable std::mutex baggage_mutex_;
  std::unordered_map<std::string, std::string> baggage_;
};
}  // namespace YYOT
