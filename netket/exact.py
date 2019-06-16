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

from ._C_netket.exact import *

import itertools


def _ImagTimePropagation_iter(self, dt, n_iter=None):
    """
    iter(self: ImagTimePropagation, dt: float, n_iter: int=None) -> int

    Returns a generator which advances the time evolution by dt,
    yielding after every step.

    Args:
        dt (float): The size of the time step.
        n_iter (int=None): The number of steps or None, for no limit.

    Yields:
        int: The current step.
    """
    for i in itertools.count():
        if n_iter and i >= n_iter:
            return
        self.advance(dt)
        yield i


ImagTimePropagation.iter = _ImagTimePropagation_iter
