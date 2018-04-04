from distutils.core import setup, Extension
import sys, os, numpy

from distutils.command.install import install

numpy_include = os.path.join(os.path.dirname(numpy.__file__), "core", "include", "numpy")
print(numpy_include)

dbr_lib_dir = 'e:\Program Files (x86)\Dynamsoft\Barcode Reader 5.2\Components\C_C++\Lib'
dbr_dll = 'e:\Program Files (x86)\Dynamsoft\Barcode Reader 5.2\Components\C_C++\Redist\DynamsoftBarcodeReaderx86.dll'

module_dbr = Extension('dbr', sources=['dbr.c'], include_dirs=[
                       numpy_include], library_dirs=[dbr_lib_dir], libraries=['DBRx86'])

class CustomInstall(install):
      def run(self):
          install.run(self)
          import shutil
          from distutils.sysconfig import get_python_lib
          src = dbr_dll
          dst = get_python_lib()
          shutil.copy2(src, dst)

setup(name='dbr',
      version='5.2',
      description='Python barcode extension',
      author='Xiao Ling',
      author_email='xiao@dynamsoft.com',
      url='https://www.dynamsoft.com/Products/Dynamic-Barcode-Reader.aspx',
      license = 'https://www.dynamsoft.com/Products/barcode-reader-license-agreement.aspx',
      ext_modules=[module_dbr],
      long_description='Dynamsoft Barcode Reader is a software development toolkit which enables barcode recognition of Code 39, Code 129, QR Code, DataMatrix, PDF417.',
      platforms=['Windows'],
      cmdclass={'install': CustomInstall}
)
