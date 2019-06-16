# Copyright 2018-2019 The Simons Foundation, Inc. - All Rights Reserved.
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

from __future__ import print_function
import netket as nk
import sys

SEED = 3141592

# Constructing a 1d lattice
g = nk.graph.Hypercube(length=20, n_dim=1)

# Hilbert space of spins from given graph
hi = nk.hilbert.Spin(s=0.5, graph=g)

# Hamiltonian
ha = nk.operator.Ising(h=1.0, hilbert=hi)

# Machine
ma = nk.machine.RbmSpin(hilbert=hi, alpha=1)
ma.init_random_parameters(seed=SEED, sigma=0.01)

# Sampler
sa = nk.sampler.MetropolisLocal(machine=ma)
sa.seed(SEED)

# Optimizer
op = nk.optimizer.Sgd(learning_rate=0.01)

# Variational Monte Carlo
vmc = nk.variational.Vmc(
    hamiltonian=ha,
    sampler=sa,
    optimizer=op,
    n_samples=1000,
    diag_shift=0.0,
    method="Sr",
)

mpi_rank = nk.MPI.rank()

for step in vmc.iter(300):
    obs = vmc.get_observable_stats()
    if mpi_rank == 0:
        print("step={}".format(step))
        print("acceptance={}".format(list(sa.acceptance)))
        print("observables={}\n".format(obs))
        # Print output to the console immediately
        sys.stdout.flush()
        # Save current parameters to file
        ma.save("test.wf")

    comm.barrier()
