import os.path
import dbr

# formats = {
#         "OneD": 0x3FF,
#         "CODE_39": 0x1,
#         "CODE_128": 0x2,
#         "CODE_93": 0x4,
#         "CODABAR": 0x8,
#         "ITF": 0x10,
#         "EAN_13": 0x20,
#         "EAN_8": 0x40,
#         "UPC_A": 0x80,
#         "UPC_E": 0x100,
#         "INDUSTRIAL_25": 0x200,
#         "PDF417": 0x2000000,
#         "DATAMATRIX": 0x8000000,
#         "QR_CODE": 0x4000000
#     }

def initLicense(license):
    dbr.initLicense(license)

def decodeFile(fileName):
    dbr.initLicense("t0260NQAAALGw+aCAePXdOS3p1xkqT5hesExKVpEe7NiIhkdlUz/Jvx8km3ItI0ykUcmeP67BYVlJ2PDW++bjSYmDLmyMgOmmvc0mdvhlSy500kqnLoBAL+TybcdAP42b5p5WehK9Gsmweqi+ydK6B0KaUNQMDJZ1DrnhDXZ209pfpJoVybPk/CMcDKXaF2oRLKEOYVscXTF6mbiWUnMP5lj4OdTvFa0eVRcE0q9BckiqYgUZLK4L6DVgRXWRL5nRPtvEtd+qZe6psu0JZ7HEPhsbodfAVH2G436z1QahLGJXdQCoQv8UQ/quGQP2wCWemfueeKJ4Y6WsvEvmkUpizbTOE3Njjaw=")
    formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000; # 1D, QRCODE, PDF417, DataMatrix
    results = dbr.decodeFile(fileName, formats)
    
    for result in results:
        print("barcode format: " + result[0])
        print("barcode value: " + result[1])

if __name__ == "__main__":
    barcode_image = input("Enter the barcode file: ");
    if not os.path.isfile(barcode_image):
        print "It is not a valid file."
    else:
        decodeFile(barcode_image);