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

#include "Graph/custom_graph.hpp"

#include <array>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <vector>
#include "Utils/messages.hpp"
#include "config.hpp"

namespace netket {

namespace {
auto DefaultAutomorphisms(std::size_t const size)
    -> std::vector<std::vector<int>> {
  std::vector<int> identity(size);
  std::iota(std::begin(identity), std::end(identity), 0);
  return std::vector<std::vector<int>>{std::move(identity)};
}
}  // namespace

NETKET_EXPORT
CustomGraph::CustomGraph(int const size, bool const is_bipartite)
    : adjlist_{},
      eclist_{},
      nsites_{size},
      automorphisms_{},
      isbipartite_{},
      is_connected_{} {
  Init(nonstd::nullopt /*adjacency list*/, nonstd::nullopt /*automorphisms*/,
       is_bipartite, nullptr);
}

NETKET_EXPORT
CustomGraph::CustomGraph(int const size,
                         std::vector<std::vector<int>> automorphisms,
                         bool const is_bipartite)
    : adjlist_{},
      eclist_{},
      nsites_{size},
      automorphisms_{},
      isbipartite_{},
      is_connected_{} {
  Init(nonstd::nullopt /*adjacency list*/, std::move(automorphisms),
       is_bipartite, nullptr);
}

NETKET_EXPORT
CustomGraph::CustomGraph(std::vector<std::vector<int>> adjlist,
                         bool const is_bipartite,
                         std::vector<std::vector<int>> const *colorlist)
    : adjlist_{},
      eclist_{},
      nsites_{},
      automorphisms_{},
      isbipartite_{},
      is_connected_{} {
  Init(std::move(adjlist), nonstd::nullopt /*automorphisms*/, is_bipartite,
       colorlist);
}

NETKET_EXPORT
CustomGraph::CustomGraph(std::vector<std::vector<int>> adjlist,
                         std::vector<std::vector<int>> automorphisms,
                         bool const is_bipartite,
                         std::vector<std::vector<int>> const *colorlist)
    : adjlist_{},
      eclist_{},
      nsites_{},
      automorphisms_{},
      isbipartite_{},
      is_connected_{} {
  Init(std::move(adjlist), std::move(automorphisms), is_bipartite, colorlist);
}

NETKET_EXPORT
void CustomGraph::Init(
    nonstd::optional<std::vector<std::vector<int>>> adjacency_list,
    nonstd::optional<std::vector<std::vector<int>>> automorphisms,
    bool const is_bipartite, std::vector<std::vector<int>> const *colorlist) {
  // First of all, the adjacency list
  if (adjacency_list.has_value()) {
    adjlist_ = std::move(*adjacency_list);
    nsites_ = static_cast<int>(adjlist_.size());
  } else {
    if (nsites_ < 1) {
      std::ostringstream msg;
      msg << "Size must be at least 1, but got " << nsites_;
      throw InvalidInputError(msg.str());
    }
    adjlist_.resize(static_cast<std::size_t>(nsites_));
  }
  // Given the adjacency list, we can determine whether the graph is connected
  is_connected_ = ComputeConnected();
  // Automorphisms
  if (automorphisms.has_value()) {
    automorphisms_ = std::move(*automorphisms);
  } else {
    automorphisms_ = DefaultAutomorphisms(static_cast<std::size_t>(nsites_));
  }
  // Is bipartite?
  isbipartite_ = is_bipartite;
  // Colors
  if (colorlist == nullptr) {
    EdgeColorsFromAdj(adjlist_, eclist_);
    InfoMessage() << "No colors specified, edge colors set to 0 " << std::endl;
  } else {
    EdgeColorsFromList(*colorlist, eclist_);
  }
  // Some sanity checks
  CheckGraph();
  // Done
  InfoMessage() << "Graph created\n"
                << "Number of nodes = " << nsites_ << '\n';
}

NETKET_EXPORT
void CustomGraph::CheckGraph() const {
  for (int i = 0; i < nsites_; i++) {
    for (auto s : adjlist_[i]) {
      // Checking if the referenced nodes are within the expected range
      if (s >= nsites_ || s < 0) {
        throw InvalidInputError("The graph is invalid");
      }
      // Checking if the adjacency list is symmetric
      // i.e. if site s is declared neihgbor of site i
      // when site i is declared neighbor of site s
      if (std::count(adjlist_[s].begin(), adjlist_[s].end(), i) != 1) {
        throw InvalidInputError("The graph adjacencylist is not symmetric");
      }
    }
  }
  for (std::size_t i = 0; i < automorphisms_.size(); i++) {
    if (int(automorphisms_[i].size()) != nsites_) {
      throw InvalidInputError("The automorphism list is invalid");
    }
  }
}

NETKET_EXPORT
bool CustomGraph::ComputeConnected() const {
  const int start = 0;  // arbitrary node
  int nvisited = 0;
  BreadthFirstSearch(start, [&nvisited](int, int) { ++nvisited; });
  return nvisited == Nsites();
}

}  // namespace netket
