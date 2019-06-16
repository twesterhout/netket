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

import matplotlib.pyplot as plt
import json

plt.ion()

# In this example we plot the energy variance

while True:
    plt.clf()
    plt.ylabel("EnergyVariance")
    plt.xlabel("Iteration #")

    iters = []
    energy = []
    sigma = []
    evar = []
    evarsig = []

    data = json.load(open("test.log"))
    for iteration in data["Output"]:
        iters.append(iteration["Iteration"])
        energy.append(iteration["Energy"]["Mean"])
        sigma.append(iteration["Energy"]["Sigma"])
        evar.append(iteration["EnergyVariance"]["Mean"])
        evarsig.append(iteration["EnergyVariance"]["Sigma"])

    plt.semilogy()
    plt.errorbar(iters, evar, yerr=evarsig, color="red")

    plt.legend(frameon=False)
    plt.pause(1)

plt.ioff()
plt.show()
