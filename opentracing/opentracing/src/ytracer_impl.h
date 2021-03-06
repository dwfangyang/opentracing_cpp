#pragma once

#include <memory>
#include <vector>
#include <string>
//#include "logger.h"
#include "ypropagation.h"
//#include "recorder.h"
#include "ytracer.h"

namespace YYOT {
class YTracerImpl
    : public YTracer,
      public std::enable_shared_from_this<YTracerImpl> {
 public:
  YTracerImpl(const PropagationOptions& propagation_options/*,
                      std::unique_ptr<Recorder>&& recorder*/) noexcept;

//  LightStepTracerImpl(std::shared_ptr<Logger> logger,
//                      const PropagationOptions& propgation_options,
//                      std::unique_ptr<Recorder>&& recorder) noexcept;

  std::unique_ptr<opentracing::Span> StartSpanWithOptions(
      opentracing::string_view operation_name,
      const opentracing::StartSpanOptions& options) const noexcept override;

//  opentracing::expected<void> Inject(
//      const opentracing::SpanContext& span_context,
//      std::ostream& writer) const override;

  opentracing::expected<void> Inject(
      const opentracing::SpanContext& span_context,
      const opentracing::TextMapWriter& writer) const override;

  opentracing::expected<void> Inject(
      const opentracing::SpanContext& span_context,
      const opentracing::HTTPHeadersWriter& writer) const override;

//  opentracing::expected<std::unique_ptr<opentracing::SpanContext>> Extract(
//      std::istream& reader) const override;

  opentracing::expected<std::unique_ptr<opentracing::SpanContext>> Extract(
      const opentracing::TextMapReader& reader) const override;

  opentracing::expected<std::unique_ptr<opentracing::SpanContext>> Extract(
      const opentracing::HTTPHeadersReader& reader) const override;

  bool Flush() noexcept override;

  void Close() noexcept override;
  
  void enqueueSpanJson(std::string spanjson);
          
  void setTraceid(std::string);

 private:
//  std::shared_ptr<Logger> logger_;
  PropagationOptions propagation_options_;
//  std::unique_ptr<Recorder> recorder_;
  std::vector<std::string> spanjsons_;
  std::string                traceid_;
};
}  // namespace lightstep
