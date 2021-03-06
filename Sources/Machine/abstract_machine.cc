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

#include "Machine/abstract_machine.hpp"

#include <mpi.h>
#include <nlohmann/json.hpp>

#include <fstream>

#include "Utils/exceptions.hpp"
#include "Utils/mpi_interface.hpp"
#include "Utils/random_utils.hpp"

namespace netket {

namespace {
inline unsigned ReallyRandomSeed() {
  std::random_device rd;
  return rd();
}
}  // namespace

void AbstractMachine::InitRandomPars(double sigma,
                                     nonstd::optional<unsigned> seed) {
  VectorType parameters(Npar());
  const auto comm = MPI_COMM_WORLD;
  constexpr auto root = 0;
  int rank;
  MPI_Comm_rank(comm, &rank);
  if (rank == root) {
    default_random_engine generator{seed.has_value() ? *seed
                                                     : ReallyRandomSeed()};
    std::generate(parameters.data(), parameters.data() + parameters.size(),
                  [&generator, sigma]() {
                    std::normal_distribution<double> dist{0.0, sigma};
                    return Complex{dist(generator), dist(generator)};
                  });
  }
  auto status = MPI_Bcast(parameters.data(), parameters.size(),
                          MPI_DOUBLE_COMPLEX, root, comm);
  if (status != MPI_SUCCESS) throw MPIError{status, "MPI_Bcast"};
  SetParameters(parameters);
}

void AbstractMachine::LogVal(Eigen::Ref<const RowMatrix<double>> v,
                             Eigen::Ref<VectorType> out,
                             const any & /*unused*/) {
  CheckShape(__FUNCTION__, "v", {v.rows(), v.cols()},
             {std::ignore, Nvisible()});
  CheckShape(__FUNCTION__, "out", out.size(), v.rows());
  for (auto i = Index{0}; i < v.rows(); ++i) {
    out(i) = LogValSingle(v.row(i));
  }
}

AbstractMachine::VectorType AbstractMachine::LogVal(
    Eigen::Ref<const RowMatrix<double>> v, const any &cache) {
  VectorType out(v.rows());
  LogVal(v, out, cache);
  return out;
}

void AbstractMachine::DerLog(Eigen::Ref<const RowMatrix<double>> v,
                             Eigen::Ref<RowMatrix<Complex>> out,
                             const any & /*cache*/) {
  CheckShape(__FUNCTION__, "v", {v.rows(), v.cols()},
             {std::ignore, Nvisible()});
  CheckShape(__FUNCTION__, "out", {out.rows(), out.cols()}, {v.rows(), Npar()});
  for (auto i = Index{0}; i < v.rows(); ++i) {
    out.row(i) = DerLogSingle(v.row(i));
  }
}

RowMatrix<Complex> AbstractMachine::DerLog(
    Eigen::Ref<const RowMatrix<double>> v, const any &cache) {
  RowMatrix<Complex> out(v.rows(), Npar());
  DerLog(v, out, cache);
  return out;
}

AbstractMachine::VectorType AbstractMachine::DerLogChanged(
    VisibleConstType v, const std::vector<int> &tochange,
    const std::vector<double> &newconf) {
  VisibleType vp(v);
  hilbert_->UpdateConf(vp, tochange, newconf);
  return DerLogSingle(vp);
}

AbstractMachine::VectorType AbstractMachine::LogValDiff(
    VisibleConstType v, const std::vector<std::vector<int>> &tochange,
    const std::vector<std::vector<double>> &newconf) {
  RowMatrix<double> input(static_cast<Index>(tochange.size()), v.size());
  input = v.transpose().colwise().replicate(input.rows());
  for (auto i = Index{0}; i < input.rows(); ++i) {
    GetHilbert().UpdateConf(input.row(i), tochange[static_cast<size_t>(i)],
                            newconf[static_cast<size_t>(i)]);
  }
  auto x = AbstractMachine::LogVal(input, any{});
  x.array() -= LogValSingle(v, any{});
  return x;
}

Complex AbstractMachine::LogValDiff(VisibleConstType v,
                                    const std::vector<int> &tochange,
                                    const std::vector<double> &newconf,
                                    const any & /*unused*/) {
  return LogValDiff(v, {tochange}, {newconf})(0);
}

}  // namespace netket
