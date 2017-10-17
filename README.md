# Building Python Extension with DBR for Windows

## Prerequisites
* [Dynamsoft Barcode Reader 5.2 for Windows][0]. Copy **Dynamsoft\Barcode Reader 5.2\Components\C_C++\Redist\DynamsoftBarcodeReaderx86.dll** to **Python27\Lib\site-packages** / **Python35\Lib\site-packages**
* Python 2.7.0 / Python 3.5.0
* OpenCV 3.3.0

    ```
    pip install opencv-python
    ```
* NumPy 1.11.2

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
1. Edit **setup.py**. Replace the include and lib paths with yours:

    ```python
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


    ```

2. Build the Python extension

    ```
    python setup.py build install
    python3 setup.py build install
    ```

## How to Run the app
1. Connect a USB webcam to your PC.
2. Run **camera.py** or **camera_thread.py**:
    ```
    python camera.py
    ```
    ![webcam barcode reader with OpenCV Python](http://www.codepool.biz/wp-content/uploads/2017/04/python-barcode-reader.PNG)

## Blog
[Building Python Barcode Extension with DBR 5.0 on Windows][2]

[0]:http://www.dynamsoft.com/Downloads/Dynamic-Barcode-Reader-Download.aspx
[1]:http://stackoverflow.com/questions/2817869/error-unable-to-find-vcvarsall-bat
[2]:http://www.codepool.biz/python-barcode-extension-dbr-windows.html
