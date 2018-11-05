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

#ifndef NETKET_CUSTOM_GRAPH_HPP
#define NETKET_CUSTOM_GRAPH_HPP

#include <nonstd/optional.hpp>
#include "Graph/abstract_graph.hpp"
#include "Utils/exceptions.hpp"
#include "Utils/kwargs.hpp"
#include "Utils/memory_utils.hpp"
#include "config.hpp"

namespace netket {

/**
    Class for user-defined graphs
    The list of edges and nodes is read from a json input file.
*/
class NETKET_EXPORT CustomGraph : public AbstractGraph {
  // adjacency list
  std::vector<std::vector<int>> adjlist_;

  ColorMap eclist_;

  int nsites_;

  std::vector<std::vector<int>> automorphisms_;

  bool isbipartite_;
  bool is_connected_;

 private:
  void CheckGraph() const;
  bool ComputeConnected() const;

  void Init(nonstd::optional<std::vector<std::vector<int>>> adjacency_list,
            nonstd::optional<std::vector<std::vector<int>>> automorphisms,
            bool is_bipartite, std::vector<std::vector<int>> const *colorlist);

 public:
  CustomGraph(int size, bool is_bipartite = false);

  CustomGraph(int size, std::vector<std::vector<int>> automorphisms,
              bool is_bipartite = false);

  CustomGraph(std::vector<std::vector<int>> adjlist, bool is_bipartite = false,
              std::vector<std::vector<int>> const *colorlist = nullptr);

  CustomGraph(std::vector<std::vector<int>> adjacency_list,
              std::vector<std::vector<int>> automorphisms,
              bool is_bipartite = false,
              std::vector<std::vector<int>> const *colorlist = nullptr);

  // Returns a list of permuted sites constituting an automorphism of the
  // graph
  std::vector<std::vector<int>> SymmetryTable() const override {
    return automorphisms_;
  }

  int Nsites() const override { return nsites_; }

  std::vector<std::vector<int>> AdjacencyList() const override {
    return adjlist_;
  }

  bool IsBipartite() const override { return isbipartite_; }

  bool IsConnected() const override { return is_connected_; }

  // Returns map of the edge and its respective color
  const ColorMap &EdgeColors() const override { return eclist_; }
};

// TODO(twesterhout): Move me somewhere :)
inline std::vector<std::vector<int>> AdjacencyListFromEdges(
    const std::vector<std::vector<int>> &edges) {
  std::vector<std::vector<int>> adjlist;
  auto nsites = 0;

  for (auto const &edge : edges) {
    if (edge.size() != 2) {
      throw InvalidInputError(
          "The edge list is invalid (edges need "
          "to connect exactly two sites)");
    }
    if (edge[0] < 0 || edge[1] < 0) {
      throw InvalidInputError("The edge list is invalid");
    }

    nsites = std::max(std::max(edge[0], edge[1]), nsites);
  }

  nsites++;
  adjlist.resize(static_cast<std::size_t>(nsites));

  for (auto const &edge : edges) {
    adjlist[edge[0]].push_back(edge[1]);
    adjlist[edge[1]].push_back(edge[0]);
  }
  return adjlist;
}

// TODO(twesterhout): Move me to json_helpers.hpp or something and make into a
// normal function (i.e. not a template) :)
template <class Parameters>
auto make_custom_graph(const Parameters &parameters)
    -> std::unique_ptr<CustomGraph> {
  auto const number_xor_options =
      static_cast<int>(FieldExists(parameters, "AdjacencyList")) +
      static_cast<int>(FieldExists(parameters, "Edges")) +
      static_cast<int>(FieldExists(parameters, "Size"));
  if (number_xor_options > 1) {
    throw InvalidInputError(
        "Custom graph specification may contain only one of 'AdjacencyList', "
        "'Edges', and 'Size'");
  } else if (number_xor_options < 1) {
    throw InvalidInputError(
        "Custom graph specification must contain one of 'AdjacencyList', "
        "'Edges', and 'Size'");
  }

  // Is bipartite?
  auto const is_bipartite =
      FieldOrDefaultVal<bool>(parameters, "IsBipartite", false);

  // Colors
  std::vector<std::vector<int>> colorlist;
  std::vector<std::vector<int>> const *colorlist_ptr = nullptr;
  if (FieldExists(parameters, "EdgeColors")) {
    colorlist = FieldVal<std::vector<std::vector<int>>>(parameters,
                                                        "EdgeColors", "Graph");
    colorlist_ptr = &colorlist;
  }

  // Adjacency list
  std::vector<std::vector<int>> adjlist;
  if (FieldExists(parameters, "AdjacencyList")) {
    adjlist = FieldVal<std::vector<std::vector<int>>>(parameters,
                                                      "AdjacencyList", "Graph");
  } else if (FieldExists(parameters, "Edges")) {
    auto const edges =
        FieldVal<std::vector<std::vector<int>>>(parameters, "Edges", "Graph");
    adjlist = AdjacencyListFromEdges(edges);
  } else {
    auto const size = FieldVal<int>(parameters, "Size");
    if (size < 1) {
      std::ostringstream msg;
      msg << "Size must be at least 1, but got " << size;
      throw InvalidInputError(msg.str());
    }
    adjlist.resize(static_cast<std::size_t>(size));
  }

  // Automorphisms
  if (FieldExists(parameters, "Automorphisms")) {
    auto automorphisms = FieldVal<std::vector<std::vector<int>>>(
        parameters, "Automorphisms", "Graph");
    return make_unique<CustomGraph>(std::move(adjlist),
                                    std::move(automorphisms), is_bipartite,
                                    colorlist_ptr);
  }
  return make_unique<CustomGraph>(std::move(adjlist), is_bipartite,
                                  colorlist_ptr);
}

}  // namespace netket

#endif
