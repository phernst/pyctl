from distutils.version import LooseVersion
import os
import platform
import re
import subprocess
import sys

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.build_py import build_py

from create_cmakelists import create_cmakelists

class CMakeExtension(Extension): # pylint: disable=too-few-public-methods
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CopyCtlData(build_py):
    def run(self):
        self.copy_ctl_data()
        super().run()

    def copy_ctl_data(self):
        self.copy_tree('ctl_src/modules/src/ocl/cl_src', 'ctl/cl_src')
        self.copy_tree('ctl_src/database', 'ctl/database')


class CMakeBuild(build_ext):
    def run(self):
        create_cmakelists('ctl_src')

        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError as os_error:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions)) from os_error

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)


this_directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name='pyctl',
    version='0.1.3',
    author='Philipp Ernst',
    author_email='philipp.ernst@ovgu.de',
    url='https://github.com/phernst/pyctl',
    description='Python bindings for CTL',
    long_description=long_description,
    long_description_content_type='text/markdown',
    ext_modules=[CMakeExtension('pyctl')],
    cmdclass=dict(build_py=CopyCtlData, build_ext=CMakeBuild),
    packages=find_packages(),
    include_package_data=True,
    zip_safe=False,
    install_requires=[
        'importlib_resources;python_version<"3.7"',
        'PySide2>=5.15',
    ],
)
