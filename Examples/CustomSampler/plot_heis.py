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

import numpy as np
import matplotlib.pyplot as plt
import json

plt.ion()


exact = -35.6175461195


while True:
    plt.clf()
    plt.ylabel("Energy")
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

    plt.errorbar(iters, energy, yerr=sigma, color="red")
    plt.axhline(y=exact, xmin=0, xmax=iters[-1], linewidth=2, color="k", label="Exact")
    nres = len(iters)
    if nres > 100:
        fitx = iters[-100:-1]
        fity = energy[-100:-1]
        z = np.polyfit(fitx, fity, deg=0)
        p = np.poly1d(z)

        plt.plot(fitx, p(fitx))
        if nres > 2000:
            plt.xlim([nres - 2000, nres])
            maxval = np.max(energy[-2000:-1])
            plt.ylim([exact - (np.abs(exact) * 0.01), maxval + np.abs(maxval) * 0.01])
        error = (z[0] - exact) / -exact
        plt.gca().text(
            0.95,
            0.8,
            "Relative Error : " + "{:.2e}".format(error),
            verticalalignment="bottom",
            horizontalalignment="right",
            color="green",
            fontsize=15,
            transform=plt.gca().transAxes,
        )

    plt.legend(frameon=False)
    plt.pause(1)


plt.show()
