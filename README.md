# Building Python Extension with DBR 5.0

## Prerequisites
* [Dynamsoft Barcode Reader 5.0 for Windows][0]
* Python 2.7.0
* OpenCV 2.4.10
* Windows 10
* USB webcam

## How to Build the Extension
1. Install OpenCV-Python **cv2.pyd**:

    ```
    pip install opencv-python
    ```

2. Set Visual Studio environment:
    * Visual Studio 2010 (VS10): SET VS90COMNTOOLS=%VS100COMNTOOLS%
    * Visual Studio 2012 (VS11): SET VS90COMNTOOLS=%VS110COMNTOOLS%
    * Visual Studio 2013 (VS12): SET VS90COMNTOOLS=%VS120COMNTOOLS%
    * Visual Studio 2015 (VS14): SET VS90COMNTOOLS=%VS140COMNTOOLS%

    If you are using **Visual Studio 2015**, use the following command:

    ```
    SET VS90COMNTOOLS=%VS140COMNTOOLS%
    ```

3. Add SDK paths to **setup.py**:

    ```python
    from distutils.core import setup, Extension

    module_dbr = Extension('dbr',
                            sources = ['dbr.c'], 
                            include_dirs=["F:\\Python27\\Lib\\site-packages\\numpy\\core\\include\\numpy", 'e:\\Program Files (x86)\\Dynamsoft\\Barcode Reader 5.0\\Components\\C_C++\\Include'],
                            library_dirs=['e:\\Program Files (x86)\\Dynamsoft\Barcode Reader 5.0\\Components\\C_C++\\Lib'],
                            libraries=['DBRx86'])

    setup (name = 'DynamsoftBarcodeReader',
            version = '1.0',
            description = 'Python barcode extension',
            ext_modules = [module_dbr])

    ```

4. Build the Python extension

    ```
    python setup.py build install
    ```

4. Copy **Dynamsoft\Barcode Reader 5.0\Components\C_C++\Redist\DynamsoftBarcodeReaderx86.dll** to **Python27\Lib\site-packages**

## How to Run the app
1. Connect a USB webcam to your PC.
2. Run **camera.py** or **camera_thread.py**:
    ```
    python camera.py
    ```
    ![webcam barcode reader with OpenCV Python](http://www.codepool.biz/wp-content/uploads/2017/04/python-barcode-reader.PNG)


[0]:http://www.dynamsoft.com/Downloads/Dynamic-Barcode-Reader-Download.aspx
[1]:http://stackoverflow.com/questions/2817869/error-unable-to-find-vcvarsall-bat
