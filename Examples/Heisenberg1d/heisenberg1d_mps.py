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
import json

pars = {}
N = 20

# defining the lattice
pars["Graph"] = {"Name": "Hypercube", "L": N, "Dimension": 1, "Pbc": True}

# defining the hamiltonian
pars["Hamiltonian"] = {"Name": "Heisenberg", "TotalSz": 0}

# defining the wave function
pars["Machine"] = {
    "Name": "MPSperiodic",
    "BondDim": 5,
    "SymmetryPeriod": 1,
    "SigmaRand": 0.01,
    "Diagonal": False,
}

# defining the sampler
# here we use Metropolis sampling with single spin flips
pars["Sampler"] = {"Name": "MetropolisHamiltonian"}

# defining the Optimizer
# here we use the Stochastic Gradient Descent
pars["Optimizer"] = {"Name": "Sgd", "LearningRate": 0.02}

pars["GroundState"] = {
    "Method": "Sr",
    "Nsamples": 1.0e3,
    "NiterOpt": 500,
    "Diagshift": 0.1,
    "UseIterative": True,
    "OutputFile": "test",
}

json_file = "MPS_heisenberg1d_N%d.json" % N
with open(json_file, "w") as outfile:
    json.dump(pars, outfile)

print("\nGenerated Json input file: ", json_file)
print("\nNow you have two options to run NetKet: ")
print("\n1) Serial mode: netket " + json_file)
print("\n2) Parallel mode: mpirun -n N_proc netket " + json_file)
