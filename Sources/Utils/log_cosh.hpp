// Copyright 2019 The Simons Foundation, Inc. - All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SOURCES_UTILS_LOG_COSH_HPP
#define SOURCES_UTILS_LOG_COSH_HPP

#include <Eigen/Core>

#include "common_types.hpp"

namespace netket {

namespace detail {
#ifdef NETKET_USE_SLEEF
Complex SumLogCosh_avx2(
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> input,
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> bias) noexcept;
Complex SumLogCosh_avx2(
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> input) noexcept;
#endif
Complex SumLogCosh_generic(
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> input,
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> bias) noexcept;
Complex SumLogCosh_generic(
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> input) noexcept;
}  // namespace detail

/// Returns `∑log(cosh(inputᵢ + biasᵢ))`
inline Complex SumLogCosh(
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> input,
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>> bias) noexcept {
#ifdef NETKET_USE_SLEEF
  if (__builtin_cpu_supports("avx2")) {
    return detail::SumLogCosh_avx2(input, bias);
  } else {
    return detail::SumLogCosh_generic(input, bias);
  }
#else
  return detail::SumLogCosh_generic(input, bias);
#endif
}

/// Returns `∑log(cosh(inputᵢ))`
inline Complex SumLogCosh(
    Eigen::Ref<const Eigen::Matrix<Complex, Eigen::Dynamic, 1>>
        input) noexcept {
#ifdef NETKET_USE_SLEEF
  if (__builtin_cpu_supports("avx2")) {
    return detail::SumLogCosh_avx2(input);
  } else {
    return detail::SumLogCosh_generic(input);
  }
#else
  return detail::SumLogCosh_generic(input);
#endif
}
}  // namespace netket

#endif  // SOURCES_UTILS_LOG_COSH_HPP
