#include "yspan_context.h"

namespace YYOT {
//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
YSpanContext::YSpanContext(
    std::string trace_id, std::string span_id,
    std::unordered_map<std::string, std::string>&& baggage) noexcept
    : trace_id_{trace_id}, span_id_{span_id}, baggage_{std::move(baggage)} {}

//------------------------------------------------------------------------------
// operator=
//------------------------------------------------------------------------------
YSpanContext& YSpanContext::operator=(
    YSpanContext&& other) noexcept {
  trace_id_ = other.trace_id_;
  span_id_ = other.span_id_;
  baggage_ = std::move(other.baggage_);
  return *this;
}

//------------------------------------------------------------------------------
// set_baggage_item
//------------------------------------------------------------------------------
void YSpanContext::set_baggage_item(
    opentracing::string_view key, opentracing::string_view value) noexcept try {
  std::lock_guard<std::mutex> lock_guard{baggage_mutex_};
  baggage_.emplace(key, value);
} catch (const std::exception&) {
  // Drop baggage item upon error.
}

//------------------------------------------------------------------------------
// baggage_item
//------------------------------------------------------------------------------
std::string YSpanContext::baggage_item(
    opentracing::string_view key) const {
  std::lock_guard<std::mutex> lock_guard{baggage_mutex_};
  auto lookup = baggage_.find(key);
  if (lookup != baggage_.end()) {
    return lookup->second;
  }
  return {};
}

//------------------------------------------------------------------------------
// ForeachBaggageItem
//------------------------------------------------------------------------------
void YSpanContext::ForeachBaggageItem(
    std::function<bool(const std::string& key, const std::string& value)> f)
    const {
  std::lock_guard<std::mutex> lock_guard{baggage_mutex_};
  for (const auto& baggage_item : baggage_) {
    if (!f(baggage_item.first, baggage_item.second)) {
      return;
    }
  }
}
}  // namespace YYOT
