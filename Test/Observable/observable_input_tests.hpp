// Copyright 2018-2019 The Simons Foundation, Inc. - All Rights Reserved.
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

#include <fstream>
#include <string>
#include <vector>
#include "Utils/json_utils.hpp"

std::vector<netket::json> GetObservableInputs() {
  std::vector<netket::json> input_tests;
  netket::json pars;

  std::vector<std::vector<double>> sx = {{0, 1}, {1, 0}};
  std::vector<std::vector<double>> szsz = {
      {1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, -1, 0}, {0, 0, 0, 1}};
  Complex Iu(0, 1);
  std::vector<std::vector<Complex>> sy = {{0, Iu}, {-Iu, 0}};

  pars.clear();
  pars["Hilbert"]["QuantumNumbers"] = {1, -1};
  pars["Hilbert"]["Size"] = 10;

  netket::json parsobs;
  parsobs["Name"] = "Observable_1";
  parsobs["Operators"] = {sx, szsz, szsz, sx,   sy, sy,
                          sy, szsz, sx,   szsz, sy, szsz};
  parsobs["ActingOn"] = {{0}, {0, 1}, {1, 0}, {1},    {2}, {3},
                         {4}, {4, 5}, {5},    {6, 8}, {9}, {7, 0}};

  pars["Observables"].push_back(parsobs);

  input_tests.push_back(pars);
  return input_tests;
}
