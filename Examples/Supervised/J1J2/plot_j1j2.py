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

import numpy as np
import matplotlib.pyplot as plt
import json

plt.ion()

while True:
    plt.clf()

    iters = []
    log_overlap = []
    mse = []
    mse_log = []

    data = json.load(open("output.log"))
    for iteration in data["Output"]:
        iters.append(iteration["Iteration"])
        log_overlap.append(iteration["log_overlap"])
        mse.append(iteration["mse"])
        mse_log.append(iteration["mse_log"])

    overlap = np.exp(-np.array(log_overlap))

    J2 = 0.4
    plt.subplot(2, 1, 1)
    plt.title(r"$J_1 J_2$ model, $J_2=" + str(J2) + "$")
    plt.ylabel("Overlap = F")
    plt.xlabel("Iteration #")

    plt.plot(iters, overlap)
    plt.axhline(
        y=1, xmin=0, xmax=iters[-1], linewidth=2, color="k", label="max accuracy = 1"
    )

    plt.legend(frameon=False)

    plt.subplot(2, 1, 2)
    plt.ylabel("Overlap Error = 1-F")
    plt.xlabel("Iteration #")
    plt.semilogy(iters, 1.0 - overlap)

    plt.pause(1)

    plt.show()
