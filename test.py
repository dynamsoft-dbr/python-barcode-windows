import os.path
import dbr
import cv2

def initLicense(license):
    dbr.initLicense(license)

def decodeFile(fileName):
    formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000 # 1D, QRCODE, PDF417, DataMatrix
    results = dbr.decodeFile(fileName, formats)
    
    for result in results:
        print("barcode format: " + result[0])
        print("barcode value: " + result[1])

def decodeBuffer(image):
    formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000 # 1D, QRCODE, PDF417, DataMatrix
    results = dbr.decodeBuffer(image, formats)
    
    for result in results:
        print("barcode format: " + result[0])
        print("barcode value: " + result[1])

if __name__ == "__main__":
    barcode_image = input("Enter the barcode file: ")
    if not os.path.isfile(barcode_image):
        print("It is not a valid file.")
    else:
        initLicense("t0068MgAAAD7z+U5pnqewjyssJP/njbxww+pxzd89JQ3nSODVo9GUADZq/0xTQx+OR7Fr8BT3A2kq/srsIxVrMcg6p5AR3T0=")
        decodeFile(barcode_image)
        image = cv2.imread(barcode_image, 1)
        decodeBuffer(image)