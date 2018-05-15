# Building Python Extension with DBR for Windows

## Prerequisites
* [Dynamsoft Barcode Reader 6.0 for Windows][0]
* Python 2/3
* OpenCV-Python

    ```
    pip install opencv-python
    ```
* NumPy

    ```
    pip install numpy
    ```
* Visual Studio 2015

    ```
    SET VS90COMNTOOLS=%VS140COMNTOOLS%
    ```
* Windows 10
* USB webcam

## How to Build the Extension
1. Edit **setup.py**. Replace the **dbr_lib_dir** and **dbr_dll** with yours:

    ```python
    from distutils.core import setup, Extension
    import sys, os, numpy

    from distutils.command.install import install

    numpy_include = os.path.join(os.path.dirname(numpy.__file__), "core", "include", "numpy")
    print(numpy_include)

    dbr_lib_dir = r'e:\Program Files (x86)\Dynamsoft\Barcode Reader 6.0\Components\C_C++\Lib'
    dbr_dll = r'e:\Program Files (x86)\Dynamsoft\Barcode Reader 6.0\Components\C_C++\Redist\x86\DynamsoftBarcodeReaderx86.dll'

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
        version='6.0',
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
    ```

2. Build the Python extension

    ```
    python setup.py build install
    python3 setup.py build install
    ```

## How to Run the app
**A simple command line app**

```
python test.py
```

**Camera**
1. Connect a USB webcam to your PC.
2. Run **camera.py** or **camera_thread.py**:

    ```
    python camera.py
    ```
    
    ![webcam barcode reader with OpenCV Python](http://www.codepool.biz/wp-content/uploads/2017/04/python-barcode-reader.png)

## Blog
* [Things to Do with DBR 6.0 and Python Barcode Extension](http://www.codepool.biz/dynamsoft-barcode-python-extension-6-0.html)
* [How to Port C/C++ Barcode Extension to Python 3](http://www.codepool.biz/cc-barcode-extension-python-3.html)
* [Building Python Barcode Extension with DBR 5.0 on Windows][2]

[0]:http://www.dynamsoft.com/Downloads/Dynamic-Barcode-Reader-Download.aspx
[1]:http://stackoverflow.com/questions/2817869/error-unable-to-find-vcvarsall-bat
[2]:http://www.codepool.biz/python-barcode-extension-dbr-windows.html
