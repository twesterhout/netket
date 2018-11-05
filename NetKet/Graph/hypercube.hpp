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

#ifndef NETKET_HYPERCUBE_HPP
#define NETKET_HYPERCUBE_HPP

#include <map>
#include <vector>
#include "Graph/abstract_graph.hpp"

namespace netket {

class Hypercube : public AbstractGraph {
  // edge of the hypercube
  const int L_;

  // number of dimensions
  const int ndim_;

  // whether to use periodic boundary conditions
  const bool pbc_;

  // contains sites coordinates
  std::vector<std::vector<int>> sites_;

  // maps coordinates to site number
  std::map<std::vector<int>, int> coord2sites_;

  // adjacency list
  std::vector<std::vector<int>> adjlist_;

  // Edge colors
  ColorMap eclist_;

  int nsites_;

 public:
  explicit Hypercube(int const L, int const ndim, bool const pbc);

  explicit Hypercube(int const L, int const ndim, bool const pbc,
                     std::vector<std::vector<int>> const &colorlist);

  std::vector<std::vector<int>> SymmetryTable() const override;

  int Nsites() const override { return nsites_; }

  int Length() const { return L_; }

  int Ndim() const { return ndim_; }

  std::vector<std::vector<int>> Sites() const { return sites_; }

  std::vector<int> SiteCoord(int i) const { return sites_[i]; }

  std::vector<std::vector<int>> AdjacencyList() const override {
    return adjlist_;
  }

  std::map<std::vector<int>, int> Coord2Site() const { return coord2sites_; }

  int Coord2Site(const std::vector<int> &coord) const {
    return coord2sites_.at(coord);
  }

  bool IsBipartite() const override { return true; }

  bool IsConnected() const override { return true; }

  // Returns map of the edge and its respective color
  const ColorMap &EdgeColors() const override { return eclist_; }

 private:
  void Init(std::vector<std::vector<int>> const*);
  void GenerateLatticePoints();
  void GenerateAdjacencyList();

#if 0
  template <class Ptype>
  Hypercube(const Ptype &pars)
      : L_(FieldVal<int>(pars, "L", "Graph")),
        ndim_(FieldVal<int>(pars, "Dimension", "Graph")),
        pbc_(FieldOrDefaultVal(pars, "Pbc", true)) {
    if (pbc_ && L_ <= 2) {
      throw InvalidInputError(
          "L<=2 hypercubes cannot have periodic boundary conditions");
    }
    Init(pars);
  }

  template <class Ptype>
  void Init(const Ptype &pars) {
    assert(L_ > 0);
    assert(ndim_ >= 1);
    GenerateLatticePoints();
    GenerateAdjacencyList();

    // If edge colors are specificied read them in, otherwise set them all to
    // 0
    if (FieldExists(pars, "EdgeColors")) {
      std::vector<std::vector<int>> colorlist =
          FieldVal<std::vector<std::vector<int>>>(pars, "EdgeColors", "Graph");
      EdgeColorsFromList(colorlist, eclist_);
    } else {
      InfoMessage() << "No colors specified, edge colors set to 0 "
                    << std::endl;
      EdgeColorsFromAdj(adjlist_, eclist_);
    }

    InfoMessage() << "Hypercube created " << std::endl;
    InfoMessage() << "Dimension = " << ndim_ << std::endl;
    InfoMessage() << "L = " << L_ << std::endl;
    InfoMessage() << "Pbc = " << pbc_ << std::endl;
  }
#endif
};

}  // namespace netket
#endif
