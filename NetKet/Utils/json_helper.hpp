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

#ifndef NETKET_JSONHELPER_HPP
#define NETKET_JSONHELPER_HPP

#include <fstream>
#include <sstream>

#include <json.hpp>
#include "exceptions.hpp"
#include "kwargs.hpp"

namespace netket {

using json = nlohmann::json;

json ReadJsonFromFile(std::string filename) {
  json pars;

  std::ifstream filein(filename);
  if (filein.is_open()) {
    filein >> pars;
  } else {
    std::stringstream s;
    s << "Cannot read Json from file: " << filename;
    throw InvalidInputError(s.str());
  }
  return pars;
}

namespace detail {
template <>
struct ParameterTraits<json> {
  using Parameters = json;

  static bool FieldExists(Parameters const& parameters,
                          std::string const& field) {
    return parameters.count(field) > 0;
  }

  template <class T>
  static T GetField(Parameters const& parameters, std::string const& field) {
    return parameters[field].get<T>();
  }

  static json GetPart(Parameters const& parameters, std::string const& field) {
    return parameters[field];
  }
};
}  // namespace detail

}  // namespace netket

#endif
