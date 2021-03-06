#pragma once

//#include <lightstep/metrics_observer.h>
//#include <lightstep/transporter.h>
#include <opentracing/tracer.h>
#include <opentracing/value.h>
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

namespace YYOT {

const std::string& CollectorServiceFullName();

const std::string& CollectorMethodName();

// Follows log level ordering defined in spdlog.
//  See https://github.com/gabime/spdlog/blob/master/include/spdlog/common.h
enum class LogLevel { debug = 1, info = 2, warn = 3, error = 4, off = 6 };

// DynamicConfigurationValue is used for configuration values that can
// be either fixed or changed at runtime. To specify a fixed value, just assign
// a constant
//    DynamicConfigurationValue<int> value = 123;
// for a dynamic value, use a functor
//    DynamicConfigurationValue<int> value = [] {
//        int value = /* look up dynamically */
//        return value;
//     };
template <class T>
class DynamicConfigurationValue {
 public:
  DynamicConfigurationValue(std::function<T()> value_functor)
      : value_functor_{std::move(value_functor)} {}

  DynamicConfigurationValue(const T& value) {
    value_functor_ = [value] { return value; };
  }

  T value() const { return value_functor_(); }

 private:
  std::function<T()> value_functor_;
};

struct YTracerOptions {
  // `component_name` is the human-readable identity of the instrumented
  // process. I.e., if one drew a block diagram of the distributed system,
  // the component_name would be the name inside the box that includes this
  // process.
  std::string component_name;

  // `access_token` is the unique API key for your YYOT project
  std::string access_token;

  // The host, port, and whether or not to use plaintext communication for the
  // collector. Ignored if a custom transporter is used.
//  std::string collector_host = "collector-grpc.lightstep.com";
//  uint32_t collector_port = 443;
//  bool collector_plaintext = false;

  // `tags` are arbitrary key-value pairs that apply to all spans generated by
  // this Tracer.
  std::unordered_map<std::string, opentracing::Value> tags;

  // Set `verbose` to true to enable more text logging.
  bool verbose = false;

  // Set `use_single_key_propagation` to Inject/Extract span context as a single
  // key in TextMap and HTTPHeaders carriers.
  bool use_single_key_propagation = false;

  // Set `logger_sink` to a custom function to override where logging is
  // printed; otherwise, it defaults to stderr.
  std::function<void(LogLevel, opentracing::string_view)> logger_sink;

  // `max_buffered_spans` is the maximum number of spans that will be buffered
  // before sending them to a collector.
  DynamicConfigurationValue<size_t> max_buffered_spans = 2000;

  // If `use_thread` is true, then the tracer will internally manage a thread to
  // regularly send reports to the collector; otherwise, if false,
  // YTracer::Flush must be manually invoked to send reports.
  //
  // Note: If `use_thread` is false, then the tracer doesn't do internal
  // locking so as to be non-blocking; so if spans are started and finished
  // concurrently, then the user is responsible for synchronization.
  bool use_thread = true;

  // `reporting_period` is the maximum duration of time between sending spans
  // to a collector.  If zero, the default will be used; and ignored if
  // `use_thread` is false.
  std::chrono::steady_clock::duration reporting_period =
      std::chrono::milliseconds{500};

  // `report_timeout` is the timeout to use when sending a reports to the
  // collector. Ignored if a custom transport is used.
  std::chrono::system_clock::duration report_timeout = std::chrono::seconds{5};

  // `transporter` customizes how spans are sent when flushed. If null, then a
  // default transporter is used.
  //
  // If `use_thread` is true, `transporter` should be derived from
  // AsyncTransporter; otherwise, it must be derived from SyncTransporter.
//  std::unique_ptr<Transporter> transporter;

  // `metrics_observer` can be optionally provided to track YYOT tracer
  // events. See MetricsObserver.
//  std::unique_ptr<MetricsObserver> metrics_observer;
};

// The YTracer interface can be used by custom carriers that need more
// direct access to a span context's data so as to propagate more efficiently.
class YTracer : public opentracing::Tracer {
 public:
  opentracing::expected<std::array<std::string, 2>> GetTraceSpanIds(
      const opentracing::SpanContext& span_context) const noexcept;

  opentracing::expected<std::unique_ptr<opentracing::SpanContext>>
  MakeSpanContext(std::string trace_id, std::string span_id,
                  std::unordered_map<std::string, std::string>&& baggage) const
      noexcept;

  virtual bool Flush() noexcept = 0;
};

// Returns a std::shared_ptr to a LightStepTracer or nullptr on failure.
std::shared_ptr<YTracer> MakeYTracer(
    YTracerOptions&& options) noexcept;
}  // namespace YYOT
