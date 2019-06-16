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

from ._C_netket.variational import *

import itertools


def _Vmc_iter(self, n_iter=None, step_size=1):
    """
    iter(self: Vmc, n_iter: int=None, step_size: int=1) -> int

    Returns a generator which advances the VMC optimization, yielding
    after every step_size steps up to n_iter.

    Args:
        n_iter (int=None): The number of steps or None, for no limit.
        step_size (int=1): The number of steps the simulation is advanced.

    Yields:
        int: The current step.
    """
    self.reset()
    for i in itertools.count(step=step_size):
        if n_iter and i >= n_iter:
            return
        self.advance(step_size)
        yield i


Vmc.iter = _Vmc_iter
