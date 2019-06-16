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

import os
import platform
import re
import shlex
import subprocess
import sys

from distutils import log
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


# Poor man's command-line options parsing
def steal_cmake_flags(args):
    """
    Extracts CMake-related arguments from ``args``. ``args`` is a list of
    strings usually equal to ``sys.argv``. All arguments of the form
    ``--cmake-args=...`` are extracted (i.e. removed from ``args``!) and
    accumulated. If there are no arguments of the specified form,
    ``NETKET_CMAKE_FLAGS`` environment variable is used instead.
    """
    _ARG_PREFIX = "--cmake-args="

    def _unquote(x):
        m = re.match(r"'(.*)'", x)
        if m:
            return m.group(1)
        m = re.match(r'"(.*)"', x)
        if m:
            return m.group(1)
        return x

    stolen_args = [x for x in args if x.startswith(_ARG_PREFIX)]
    for x in stolen_args:
        args.remove(x)

    if len(stolen_args) > 0:
        cmake_args = sum(
            (shlex.split(_unquote(x[len(_ARG_PREFIX) :])) for x in stolen_args), []
        )
    else:
        try:
            cmake_args = shlex.split(os.environ["NETKET_CMAKE_FLAGS"])
        except KeyError:
            cmake_args = []
    return cmake_args


"""
A list of arguments to be passed to the configuration step of CMake.
"""
_CMAKE_FLAGS = steal_cmake_flags(sys.argv)


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


def _have_ninja():
    """
    Returns `True` if the [ninja](https://ninja-build.org/) build system is
    available on the system.
    """
    with open(os.devnull, "wb") as devnull:
        try:
            subprocess.check_call("ninja --version".split(), stdout=devnull)
        except OSError:
            return False
        else:
            return True


def _generator_specified(args):
    """
    Returns `True` if `-G` flag was given to CMake.
    """
    for _ in filter(lambda f: f.startswith("-G"), args):
        return True
    return False


class CMakeBuild(build_ext):
    """
    We extend setuptools to support building extensions with CMake. An extension
    is built with CMake if it inherits from ``CMakeExtension``.
    """

    def build_extension(self, ext):
        if isinstance(ext, CMakeExtension):  # Building with CMake
            cwd = os.getcwd()
            # Create a directory for building out-of-source
            if not os.path.exists(self.build_temp):
                os.makedirs(self.build_temp)
            # lib_dir is the directory, where the shared libraries will be
            # stored (it will probably be different from the build_temp
            # directory so that setuptools find the libraries)
            lib_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
            if not os.path.exists(lib_dir):
                os.makedirs(lib_dir)
            # Options to pass to CMake during configuration
            cmake_args = _CMAKE_FLAGS
            cmake_args.append(
                "-DNETKET_PYTHON_VERSION={}.{}.{}".format(*sys.version_info[:3])
            )
            cmake_args.append("-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(lib_dir))
            if not _generator_specified(cmake_args) and _have_ninja():
                cmake_args.append("-GNinja")

            def _decode(x):
                if sys.version_info >= (3, 0):
                    return x.decode()
                else:
                    return x

            # Building
            os.chdir(self.build_temp)
            try:
                # Configuration step
                output = subprocess.check_output(
                    ["cmake", ext.sourcedir] + cmake_args, stderr=subprocess.STDOUT
                )
                if self.distribution.verbose:
                    log.info(_decode(output))
                if not self.distribution.dry_run:
                    # Build step
                    output = subprocess.check_output(
                        ["cmake", "--build", "."], stderr=subprocess.STDOUT
                    )
                    if self.distribution.verbose:
                        log.info(_decode(output))
            except subprocess.CalledProcessError as e:
                if hasattr(ext, "optional"):
                    if not ext.optional:
                        self.warn(_decode(e.output))
                        raise
                    self.warn(
                        'building extension "{}" failed:\n{}'.format(
                            ext.name, _decode(e.output)
                        )
                    )
                else:
                    self.warn(_decode(e.output))
                    raise
            os.chdir(cwd)
        else:  # Fall back to the default method
            if sys.version_info >= (3, 0):
                super().build_extension(ext)
            else:
                super(build_ext, self).build_extension(ext)


setup(
    name="netket",
    version="2.0b3",
    author="Giuseppe Carleo et al.",
    url="http://github.com/netket/netket",
    author_email="netket@netket.org",
    license="Apache 2.0",
    packages=["netket"],
    ext_modules=[CMakeExtension("netket._C_netket")],
    long_description="""NetKet is an open - source project delivering cutting - edge
         methods for the study of many - body quantum systems with artificial
         neural networks and machine learning techniques.""",
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)
