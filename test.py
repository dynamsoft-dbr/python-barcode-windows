import os
import dbr
import cv2

def loadSettings(setting_file):
    dbr.loadSettings(setting_file)

def initLicense(license):
    dbr.initLicense(license)

def decodeFile(fileName):
    formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000 # 1D, QRCODE, PDF417, DataMatrix
    results = dbr.decodeFile(fileName, formats, 'CUSTOM')
    
    for result in results:
        print("barcode format: " + result[0])
        print("barcode value: " + result[1])

def decodeBuffer(image):
    formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000 # 1D, QRCODE, PDF417, DataMatrix
    results = dbr.decodeBuffer(image, formats, 'CUSTOM')
    
    for result in results:
        print("barcode format: " + result[0])
        print("barcode value: " + result[1])

if __name__ == "__main__":
    import sys
    if sys.version_info < (3, 0):
        barcode_image = raw_input("Enter the barcode file: ")
    else:
        barcode_image = input("Enter the barcode file: ")
        
    if not os.path.isfile(barcode_image):
        print("It is not a valid file.")
    else:
        initLicense("t0068MgAAAGhyhnDz09eHhPRZ5MRe3ak0VO/TZmolcIp39SoQ8LtsgZhzAj5khTx+OFsXgz+HIz+MNaC7VA69MM1N26gNlZA=")
        setting_file = os.path.join(os.getcwd(), 'templates', 'default.settings.json')
        loadSettings(setting_file)
        decodeFile(barcode_image)
        image = cv2.imread(barcode_image, 1)
        decodeBuffer(image)