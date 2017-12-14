from distutils.core import setup, Extension
import sys, os, numpy

numpy_include = os.path.join(os.path.dirname(numpy.__file__), "core", "include", "numpy")
print(numpy_include)

dbr_lib_dir = 'e:\\Program Files (x86)\\Dynamsoft\Barcode Reader 5.2\\Components\\C_C++\\Lib'

module_dbr = Extension('dbr', sources=['dbr.c'], include_dirs=[
                       numpy_include], library_dirs=[dbr_lib_dir], libraries=['DBRx86'])

setup(name='DynamsoftBarcodeReader',
      version='5.2',
      description='Python barcode extension',
      ext_modules=[module_dbr])
