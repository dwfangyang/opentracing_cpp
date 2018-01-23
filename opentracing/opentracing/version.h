#ifndef OPENTRACING_VERSION_H
#define OPENTRACING_VERSION_H

#define OPENTRACING_VERSION "1.3.0"

// clang-format off
#define BEGIN_OPENTRACING_ABI_NAMESPACE \
  inline namespace v1 {
#define END_OPENTRACING_ABI_NAMESPACE \
  }  // namespace v1
// clang-format on

#endif // OPENTRACING_VERSION_H
