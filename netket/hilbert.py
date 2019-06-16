# Copyright 2019 The Simons Foundation, Inc. - All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from ._C_netket.hilbert import *


def Qubit(graph):
    """
    Constructs a new ``Qubit`` given a graph.

    Args:
        graph: Graph representation of sites.

    Examples:
        Simple qubit hilbert space.

        ```python
        >>> from netket.graph import Hypercube
        >>> from netket.hilbert import Qubit
        >>> g = Hypercube(length=10,n_dim=2,pbc=True)
        >>> hi = Qubit(graph=g)
        >>> print(hi.size)
        100
    """
    return CustomHilbert(graph, local_states=[0, 1])
