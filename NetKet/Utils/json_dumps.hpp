// Copyright 2018 The Simons Foundation, Inc. - All Rights Reserved.
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

#ifndef NETKET_JSON_DUMPS_HPP
#define NETKET_JSON_DUMPS_HPP

#include <complex>
#include <vector>

#include <Eigen/Dense>
#include <json.hpp>
#include "exceptions.hpp"

namespace Eigen {

template <class T>
void to_json(nlohmann::json &js, const Matrix<T, Eigen::Dynamic, 1> &v) {
  std::vector<T> temp(v.size());
  for (std::size_t i = 0; i < std::size_t(v.size()); i++) {
    temp[i] = v(i);
  }
  js = nlohmann::json(temp);
}

template <class T>
void from_json(const nlohmann::json &js, Matrix<T, Eigen::Dynamic, 1> &v) {
  std::vector<T> temp = js.get<std::vector<T>>();
  v.resize(temp.size());
  for (std::size_t i = 0; i < temp.size(); i++) {
    v(i) = temp[i];
  }
}

template <class T>
void to_json(nlohmann::json &js,
             const Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &v) {
  std::vector<std::vector<T>> temp(v.rows());
  for (std::size_t i = 0; i < std::size_t(v.rows()); i++) {
    temp[i].resize(v.cols());
    for (std::size_t j = 0; j < std::size_t(v.cols()); j++) {
      temp[i][j] = v(i, j);
    }
  }
  js = nlohmann::json(temp);
}

template <class T>
void from_json(const nlohmann::json &js,
               Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &v) {
  std::vector<std::vector<T>> temp = js.get<std::vector<std::vector<T>>>();

  if (temp[0].size() == 0) {
    throw netket::InvalidInputError(
        "Error while loading Eigen Matrix from Json");
  }

  v.resize(temp.size(), temp[0].size());
  for (std::size_t i = 0; i < temp.size(); i++) {
    for (std::size_t j = 0; j < temp[i].size(); j++) {
      if (temp[i].size() != temp[0].size()) {
        throw netket::InvalidInputError(
            "Error while loading Eigen Matrix from Json");
      }
      v(i, j) = temp[i][j];
    }
  }
}

}  // namespace Eigen

namespace nlohmann {
template <typename T>
struct adl_serializer<std::complex<T>> {
  static void to_json(json &js, const std::complex<T> &p) {
    js = json{p.real(), p.imag()};
  }

  static void from_json(const json &js, std::complex<T> &p) {
    if (js.is_array()) {
      p = std::complex<T>{js.at(0).get<T>(), js.at(1).get<T>()};
    } else {
      p = std::complex<T>{js.get<T>(), T{0}};
    }
  }
};
}  // namespace nlohmann

#endif
