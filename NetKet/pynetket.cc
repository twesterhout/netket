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

#ifndef NETKET_PYNETKET_CC
#define NETKET_PYNETKET_CC

#include <mpi.h>
#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <complex>
#include <vector>
#include "netket.hpp"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<std::complex<double>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::vector<int>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::vector<double>>);

namespace netket {

PYBIND11_MODULE(pynetket, m) {
  py::bind_vector<std::vector<int>>(m, "VectorInt");
  py::bind_vector<std::vector<std::vector<int>>>(m, "VectorVectorInt");
  py::bind_vector<std::vector<double>>(m, "VectorDouble");
  py::bind_vector<std::vector<std::vector<double>>>(m, "VectorVectorDouble");
  py::bind_vector<std::vector<std::complex<double>>>(m, "VectorComplexDouble");

  py::class_<AbstractGraph>(m, "_AbstractGraph")
      .def("Nsites", &AbstractGraph::Nsites)
      .def("AdjacencyList", &AbstractGraph::AdjacencyList)
      .def("SymmetryTable", &AbstractGraph::SymmetryTable)
      .def("EdgeColors", &AbstractGraph::EdgeColors)
      .def("IsBipartite", &AbstractGraph::IsBipartite)
      .def("IsConnected", &AbstractGraph::IsConnected)
      .def("Distances", &AbstractGraph::Distances)
      .def("AllDistances", &AbstractGraph::AllDistances);

  py::class_<Hypercube, AbstractGraph>(m, "Hypercube")
      .def(py::init<int, int, bool>())
      .def(py::init<int, int, bool, std::vector<std::vector<int>>>())
      .def("Nsites", &Hypercube::Nsites)
      .def("AdjacencyList", &Hypercube::AdjacencyList)
      .def("SymmetryTable", &Hypercube::SymmetryTable)
      .def("EdgeColors", &Hypercube::EdgeColors)
      .def("IsBipartite", &Hypercube::IsBipartite)
      .def("IsConnected", &Hypercube::IsConnected)
      .def("Distances", &Hypercube::Distances)
      .def("AllDistances", &Hypercube::AllDistances);

#if 0 // Only temporary
  py::class_<Hilbert>(m, "Hilbert")
      .def(py::init<py::kwargs>())
      .def(py::init<const Graph &, py::kwargs>())
      .def("IsDiscrete", &Hilbert::IsDiscrete)
      .def("LocalSize", &Hilbert::LocalSize)
      .def("Size", &Hilbert::Size)
      .def("LocalStates", &Hilbert::LocalStates)
      .def("UpdateConf", &Hilbert::UpdateConf);

  py::class_<Hamiltonian>(m, "Hamiltonian")
      .def(py::init<const Hilbert &, py::kwargs>())
      .def("FindConn", &Hamiltonian::FindConn)
      .def("ForEachConn", &Hamiltonian::ForEachConn)
      .def("GetHilbert", &Hamiltonian::GetHilbert);

  py::class_<SparseMatrixWrapper<Hamiltonian>>(m, "SparseHamiltonianWrapper")
      .def(py::init<const Hamiltonian &>())
      .def("GetMatrix", &SparseMatrixWrapper<Hamiltonian>::GetMatrix);
#endif
}

}  // namespace netket

#endif
