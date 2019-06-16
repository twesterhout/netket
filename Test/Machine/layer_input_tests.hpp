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

std::vector<netket::json> GetLayerInputs() {
  std::vector<netket::json> input_tests;
  netket::json pars;
  //
  // // FullyConnected layer
  pars = {{"Graph",
           {{"Name", "Hypercube"}, {"L", 20}, {"Dimension", 1}, {"Pbc", true}}},
          {"Machine",
           {{"Name", "FFNN"},
            {"Layers",
             {{{"Name", "FullyConnected"},
               {"Inputs", 20},
               {"Outputs", 40},
               {"Activation", "Lncosh"}}}}}},
          {"Hamiltonian", {{"Name", "Heisenberg"}}}};
  pars["Hilbert"]["Name"] = "Spin";
  pars["Hilbert"]["S"] = 0.5;
  input_tests.push_back(pars);

  // Convolutional layer
  pars = {{"Graph",
           {{"Name", "Hypercube"}, {"L", 20}, {"Dimension", 1}, {"Pbc", true}}},
          {"Machine",
           {{"Name", "FFNN"},
            {"Layers",
             {{{"Name", "Convolutional"},
               {"InputChannels", 1},
               {"OutputChannels", 2},
               {"Distance", 2},
               {"Activation", "Lncosh"}},
              {{"Name", "Convolutional"},
               {"InputChannels", 2},
               {"OutputChannels", 2},
               {"Distance", 2},
               {"Activation", "Lncosh"}}}}}},
          {"Hamiltonian", {{"Name", "Heisenberg"}}}};
  pars["Hilbert"]["Name"] = "Spin";
  pars["Hilbert"]["S"] = 0.5;
  input_tests.push_back(pars);

  // // Sum out layer
  pars = {{"Graph",
           {{"Name", "Hypercube"}, {"L", 20}, {"Dimension", 1}, {"Pbc", true}}},
          {"Machine",
           {{"Name", "FFNN"},
            {"Layers",
             {{{"Name", "Convolutional"},
               {"InputChannels", 1},
               {"OutputChannels", 2},
               {"Distance", 2},
               {"Activation", "Lncosh"}},
              {{"Name", "Sum"}, {"Inputs", 40}},
              {{"Name", "FullyConnected"},
               {"Inputs", 1},
               {"Outputs", 2},
               {"Activation", "Lncosh"}}}}}},
          {"Hamiltonian", {{"Name", "Heisenberg"}}}};
  pars["Hilbert"]["Name"] = "Spin";
  pars["Hilbert"]["S"] = 0.5;
  input_tests.push_back(pars);

  pars = {{"Graph",
           {{"Name", "Hypercube"}, {"L", 20}, {"Dimension", 1}, {"Pbc", true}}},
          {"Machine",
           {{"Name", "FFNN"},
            {"Layers",
             {{{"Name", "Convolutional"},
               {"InputChannels", 1},
               {"OutputChannels", 2},
               {"Distance", 2},
               {"Activation", "Lncosh"}}}}}},
          {"Hamiltonian", {{"Name", "Heisenberg"}}}};
  pars["Hilbert"]["Name"] = "Spin";
  pars["Hilbert"]["S"] = 0.5;
  input_tests.push_back(pars);

  return input_tests;
}
