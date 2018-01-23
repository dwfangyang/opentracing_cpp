//
//  ypropagation.hpp
//  opentracing
//
//  Created by 方阳 on 2018/1/23.
//  Copyright © 2018年 yy. All rights reserved.
//

#ifndef ypropagation_hpp
#define ypropagation_hpp

#include <stdio.h>
#include <opentracing/propagation.h>
#include <unordered_map>

namespace lightstep {
    struct PropagationOptions {
        bool use_single_key = false;
    };
    
//    opentracing::expected<void> InjectSpanContext(
//                                                  const PropagationOptions& propagation_options, std::ostream& carrier,
//                                                  uint64_t trace_id, uint64_t span_id,
//                                                  const std::unordered_map<std::string, std::string>& baggage);
    
    opentracing::expected<void> InjectSpanContext(
                                                  const PropagationOptions& propagation_options,
                                                  const opentracing::TextMapWriter& carrier, uint64_t trace_id,
                                                  uint64_t span_id,
                                                  const std::unordered_map<std::string, std::string>& baggage);
    
//    opentracing::expected<bool> ExtractSpanContext(
//                                                   const PropagationOptions& propagation_options, std::istream& carrier,
//                                                   uint64_t& trace_id, uint64_t& span_id,
//                                                   std::unordered_map<std::string, std::string>& baggage);
    
    opentracing::expected<bool> ExtractSpanContext(
                                                   const PropagationOptions& propagation_options,
                                                   const opentracing::TextMapReader& carrier, uint64_t& trace_id,
                                                   uint64_t& span_id, std::unordered_map<std::string, std::string>& baggage);
    
//    opentracing::expected<bool> ExtractSpanContext(
//                                                   const PropagationOptions& propagation_options,
//                                                   const opentracing::HTTPHeadersReader& carrier, uint64_t& trace_id,
//                                                   uint64_t& span_id, std::unordered_map<std::string, std::string>& baggage);
}  // namespace lightstep
#endif /* ypropagation_hpp */
