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

import netket.exact as exact
import netket.hilbert as hs
import netket.operator as op
import netket.graph as gr
import math as ma
import numpy as np


def build_rotation(hi, basis):
    localop = op.LocalOperator(hi, 1.0)
    U_X = 1.0 / (ma.sqrt(2)) * np.asarray([[1.0, 1.0], [1.0, -1.0]])
    U_Y = 1.0 / (ma.sqrt(2)) * np.asarray([[1.0, -1j], [1.0, 1j]])

    N = hi.size

    assert len(basis) == hi.size

    for j in range(hi.size):
        if basis[j] == "X":
            localop *= op.LocalOperator(hi, U_X, [j])
        if basis[j] == "Y":
            localop *= op.LocalOperator(hi, U_Y, [j])
    return localop


def generate(N, n_basis=20, n_shots=1000, seed=1234):
    g = gr.Hypercube(length=N, n_dim=1, pbc=False)
    hi = hs.Spin(g, s=0.5)
    ha = op.Ising(hilbert=hi, h=1)
    res = exact.lanczos_ed(ha, first_n=1, compute_eigenvectors=True)

    psi = res.eigenvectors[0]

    rotations = []
    training_samples = []
    training_bases = []

    np.random.seed(seed)

    for m in range(n_basis):
        basis = np.random.choice(
            list("XYZ"), size=N, p=[1.0 / N, 1.0 / N, (N - 2.0) / N]
        )

        rotation = build_rotation(hi, basis)
        psir = rotation.to_sparse().dot(psi)

        rand_n = np.random.choice(
            hi.n_states, p=np.square(np.absolute(psir)), size=n_shots
        )

        for rn in rand_n:
            training_samples.append(hi.number_to_state(rn))
        training_bases += [m] * n_shots

        rotations.append(rotation)

    return hi, tuple(rotations), training_samples, training_bases, ha, psi
