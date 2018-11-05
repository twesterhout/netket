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

#include "Graph/hypercube.hpp"

#include <sstream>
#include "Utils/exceptions.hpp"
#include "Utils/messages.hpp"
#include "Utils/next_variation.hpp"
#include "config.hpp"

namespace netket {

namespace {
static void CheckArgs(int const L, int const ndim, bool const pbc) {
  if (L <= 0) {
    std::ostringstream msg;
    msg << "Side length must be at least 1, but got " << L;
    throw InvalidInputError{msg.str()};
  }
  if (ndim <= 0) {
    std::ostringstream msg;
    msg << "Dimension must be at least 1, but got " << ndim;
    throw InvalidInputError{msg.str()};
  }
  if (pbc && L <= 2) {
    throw InvalidInputError(
        "L<=2 hypercubes cannot have periodic boundary conditions");
  }
}
}  // namespace

NETKET_EXPORT
Hypercube::Hypercube(int const L, int const ndim, bool const pbc)
    : L_{L},
      ndim_{ndim},
      pbc_{pbc},
      sites_{},
      coord2sites_{},
      adjlist_{},
      eclist_{},
      nsites_{0} {
  CheckArgs(L, ndim, pbc);
  Init(nullptr);
}

NETKET_EXPORT
Hypercube::Hypercube(int const L, int const ndim, bool const pbc,
                     std::vector<std::vector<int>> const &colorlist)
    : L_{L},
      ndim_{ndim},
      pbc_{pbc},
      sites_{},
      coord2sites_{},
      adjlist_{},
      eclist_{},
      nsites_{0} {
  CheckArgs(L, ndim, pbc);
  Init(&colorlist);
}

NETKET_EXPORT
void Hypercube::Init(std::vector<std::vector<int>> const *colorlist) {
  assert(L_ > 0 && "Bug! L_>=1 by construction.");
  assert(ndim_ >= 1 && "Bug! ndim_>=1 by construction.");
  GenerateLatticePoints();
  GenerateAdjacencyList();
  // If edge colors are specificied read them in, otherwise set them all to 0
  if (colorlist != nullptr) {
    EdgeColorsFromList(*colorlist, eclist_);
  } else {
    InfoMessage() << "No colors specified, edge colors set to 0 " << '\n';
    EdgeColorsFromAdj(adjlist_, eclist_);
  }
  InfoMessage() << "Hypercube created\n";
  InfoMessage() << "Dimension = " << ndim_ << '\n';
  InfoMessage() << "L = " << L_ << '\n';
  InfoMessage() << "Pbc = " << pbc_ << '\n';
}

NETKET_EXPORT
void Hypercube::GenerateLatticePoints() {
  std::vector<int> coord(ndim_, 0);

  nsites_ = 0;
  do {
    sites_.push_back(coord);
    coord2sites_[coord] = nsites_;
    nsites_++;
  } while (netket::next_variation(coord.begin(), coord.end(), L_ - 1));
}

NETKET_EXPORT
void Hypercube::GenerateAdjacencyList() {
  adjlist_.resize(nsites_);

  for (int i = 0; i < nsites_; i++) {
    std::vector<int> neigh(ndim_);
    std::vector<int> neigh2(ndim_);

    neigh = sites_[i];
    neigh2 = sites_[i];
    for (int d = 0; d < ndim_; d++) {
      if (pbc_) {
        neigh[d] = (sites_[i][d] + 1) % L_;
        neigh2[d] = ((sites_[i][d] - 1) % L_ + L_) % L_;
        int neigh_site = coord2sites_.at(neigh);
        int neigh_site2 = coord2sites_.at(neigh2);
        adjlist_[i].push_back(neigh_site);
        adjlist_[i].push_back(neigh_site2);
      } else {
        if ((sites_[i][d] + 1) < L_) {
          neigh[d] = (sites_[i][d] + 1);
          int neigh_site = coord2sites_.at(neigh);
          adjlist_[i].push_back(neigh_site);
          adjlist_[neigh_site].push_back(i);
        }
      }

      neigh[d] = sites_[i][d];
      neigh2[d] = sites_[i][d];
    }
  }
}

// Returns a list of permuted sites equivalent with respect to
// translation symmetry
NETKET_EXPORT
std::vector<std::vector<int>> Hypercube::SymmetryTable() const {
  if (!pbc_) {
    throw InvalidInputError(
        "Cannot generate translation symmetries "
        "in the hypercube without PBC");
  }

  std::vector<std::vector<int>> permtable;

  std::vector<int> transl_sites(nsites_);
  std::vector<int> ts(ndim_);

  for (int i = 0; i < nsites_; i++) {
    for (int p = 0; p < nsites_; p++) {
      for (int d = 0; d < ndim_; d++) {
        ts[d] = (sites_[i][d] + sites_[p][d]) % L_;
      }
      transl_sites[p] = coord2sites_.at(ts);
    }
    permtable.push_back(transl_sites);
  }
  return permtable;
}

}  // namespace netket
