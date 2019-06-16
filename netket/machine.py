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

from ._C_netket.machine import *


def MPSPeriodicDiagonal(hilbert, bond_dim, symperiod=-1):
    return MPSPeriodic(hilbert, bond_dim, diag=True, symperiod=symperiod)


class CxxMachine(Machine):
    def __init__(self, hilbert):
        super(CxxMachine, self).__init__(hilbert)

    def log_val(self, v):
        r"""Returns the logarithm of the wave function for a visible
        configuration `v`.

        Subclasses should implement this function.

        Args:
            x: 1D vector of `float` of size `self.n_visible`.
        """
        raise NotImplementedError

    def der_log(self, v):
        r"""Returns the gradient of the logarithm of the wave function for
        a visible configuration `v`.

        Subclasses should implement this function.

        Args:
            x: 1D vector of `float` of size `self.n_visible`.
        """
        raise NotImplementedError

    def save(self, filename):
        r"""Saves machine's state to file.

        Subclasses should implement this function.

        *Note:* the sequence of saving and loading, -- ``m.save(filename);
        m.load(filename)``, -- should be an identity.

        Args:
            filename: name of the file.
	"""
        raise NotImplementedError

    def load(self, filename):
        r"""Loads machine's state from file.

        *Note:* the sequence of saving and loading, -- ``m.save(filename);
        m.load(filename)``, -- should be an identity.

        Subclasses should implement this function.

        Args:
            filename: name of the file.
	"""
        raise NotImplementedError

    def _is_holomorphic(self):
        r"""Returns whether the wave function is holomorphic.
        """
        raise NotImplementedError

    def _number_parameters(self):
        r"""Returns the number of variational parameters in the machine.

        Subclasses should implement this function, but to actually get the
        number of parameters, use `self.n_par` attribute.
        """
        raise NotImplementedError

    def _number_visible(self):
        r"""Returns the number of "visible units" in the machine.

        Subclasses should implement this function, but to actually get the
        dimension of input vector, use `self.n_visible` attribute.
        """
        raise NotImplementedError

    def _get_parameters(self):
        r"""Returns the variational parameters as a 1D complex vector.

        Subclasses should implement this function, but to actually access
        the parameters, use `self.parameters` attribute.
        """
        raise NotImplementedError

    def _set_parameters(self, p):
        r"""Sets the variational parameters.

        Subclasses should implement this function, but to actually access
        the parameters, use `self.parameters` attribute.

        Args:
            p: New variational parameters as a 1D complex vector.
        """
        raise NotImplementedError

