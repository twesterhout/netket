// Copyright 2018 The Simons Foundation, Inc. - All Rights Reserved.
// Copyright 2018 Tom Westerhout
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

#ifndef NETKET_PYTHONHELPER_HPP
#define NETKET_PYTHONHELPER_HPP

#include <pybind11/cast.h>
#include <pybind11/pytypes.h>
#include "exceptions.hpp"
#include "kwargs.hpp"

namespace netket {

namespace detail {
template <>
struct ParameterTraits<pybind11::dict> {
  using Parameters = pybind11::dict;

  static bool FieldExists(Parameters const& parameters,
                          std::string const& field) {
    return parameters.contains(pybind11::cast(field));
  }

  template <class T>
  static T GetField(Parameters const& parameters, std::string const& field) {
    return parameters[pybind11::cast(field)].cast<T>();
  }

  static pybind11::dict GetPart(Parameters const& parameters,
                                std::string const& field) {
    return parameters[pybind11::cast(field)];
  }
};

template <>
struct ParameterTraits<pybind11::kwargs>
    : public ParameterTraits<pybind11::dict> {};
}  // namespace detail

}  // namespace netket

#endif
