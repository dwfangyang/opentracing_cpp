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
    // A YTracer is a implementation of Tracer
    //
    std::shared_ptr<Tracer> MakeYTracer() noexcept;
    END_OPENTRACING_ABI_NAMESPACE
}  // namespace opentracing

#endif /* yyop_hpp */
