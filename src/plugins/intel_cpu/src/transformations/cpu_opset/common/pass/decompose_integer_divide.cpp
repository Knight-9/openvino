// Copyright (C) 2020-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "decompose_integer_divide.hpp"
#include <ngraph/opsets/opset1.hpp>
#include <ngraph/rt_info.hpp>

namespace ov {
namespace intel_cpu {

DecomposeIntegerDivide::DecomposeIntegerDivide() {
    register_matcher(std::make_shared<ngraph::pattern::Matcher>(ngraph::pattern::wrap_type<ngraph::opset1::Divide>(), "DecomposeIntegerDivide"),
         [](ngraph::pattern::Matcher& m) {
             auto divide = std::dynamic_pointer_cast<ngraph::opset1::Divide>(m.get_match_root());
             if (!divide) {
                 return false;
             }
             if (!divide->get_element_type().is_integral_number()) {
                 return false;
             }

             auto new_divide = std::make_shared<ngraph::opset1::Divide>(divide->input_value(0), divide->input_value(1));
             auto new_floor = std::make_shared<ngraph::opset1::Floor>(new_divide);
             new_floor->set_friendly_name(divide->get_friendly_name());
             ngraph::copy_runtime_info(divide, new_floor);
             ngraph::replace_node(divide, new_floor);
             return true;
         });
}

}   // namespace intel_cpu
}   // namespace ov
