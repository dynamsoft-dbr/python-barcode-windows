from distutils.core import setup, Extension
import sys

dbr_include_dir = 'e:\\Program Files (x86)\\Dynamsoft\\Barcode Reader 5.2\\Components\\C_C++\\Include'
dbr_lib_dir = 'e:\\Program Files (x86)\\Dynamsoft\Barcode Reader 5.2\\Components\\C_C++\\Lib'


numpy_include_dir = None
if sys.version_info[0] == 2 and sys.version_info[1] == 7:
    numpy_include_dir = "F:\\Python27\\Lib\\site-packages\\numpy-1.11.2-py2.7-win32.egg\\numpy\\core\\include\\numpy"
else:
    numpy_include_dir = "F:\\Python35\\Lib\\site-packages\\numpy-1.11.2-py3.5-win32.egg\\numpy\\core\\include\\numpy"

print(sys.version_info)
print(numpy_include_dir)
module_dbr = Extension('dbr', sources=['dbr.c'], include_dirs=[
                       numpy_include_dir, dbr_include_dir], library_dirs=[dbr_lib_dir], libraries=['DBRx86'])

setup(name='DynamsoftBarcodeReader',
      version='1.0',
      description='Python barcode extension',
      ext_modules=[module_dbr])
