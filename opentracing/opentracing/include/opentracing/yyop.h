//
//  yyop.hpp
//  opentracing
//
//  Created by 方阳 on 2018/1/22.
//  Copyright © 2018年 yy. All rights reserved.
//

#ifndef yyop_hpp
#define yyop_hpp

#include <opentracing/tracer.h>
#include <opentracing/version.h>
#include <memory>

namespace opentracing {
    BEGIN_OPENTRACING_ABI_NAMESPACE
    // A NoopTracer is a trivial, minimum overhead implementation of Tracer
    // for which all operations are no-ops.
    //
    // The primary use of this implementation is in libraries, such as RPC
    // frameworks, that make tracing an optional feature controlled by the
    // end user. A no-op implementation allows said libraries to use it
    // as the default Tracer and to write instrumentation that does
    // not need to keep checking if the tracer instance is nil.
    //
    // For the same reason, the NoopTracer is the default "global" tracer
    // (see Tracer::Global and Tracer::InitGlobal functions).
    //
    // WARNING: NoopTracer does not support baggage propagation.
    std::shared_ptr<Tracer> MakeYYopTracer() noexcept;
    END_OPENTRACING_ABI_NAMESPACE
}  // namespace opentracing

#endif /* yyop_hpp */
