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

#ifndef NETKET_SOURCES_OPTIMIZER_PY_STOCHASTIC_RECONFIGURATION_HPP
#define NETKET_SOURCES_OPTIMIZER_PY_STOCHASTIC_RECONFIGURATION_HPP

#include <Python.h>
#include "Optimizer/stochastic_reconfiguration.hpp"

namespace netket {

void AddSR(PyObject* module);

}  // namespace netket

#endif  // NETKET_SOURCES_OPTIMIZER_PY_STOCHASTIC_RECONFIGURATION_HPP
