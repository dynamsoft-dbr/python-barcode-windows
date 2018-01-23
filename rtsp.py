import cv2
import dbr
import time

def get_time():
    localtime = time.localtime()
    capturetime = time.strftime("%Y%m%d%H%M%S", localtime)
    return capturetime

def read_barcode():

    vc = cv2.VideoCapture("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_175k.mov");

    if vc.isOpened(): # try to get the first frame
        dbr.initLicense("t0260NQAAALGw+aCAePXdOS3p1xkqT5hesExKVpEe7NiIhkdlUz/Jvx8km3ItI0ykUcmeP67BYVlJ2PDW++bjSYmDLmyMgOmmvc0mdvhlSy500kqnLoBAL+TybcdAP42b5p5WehK9Gsmweqi+ydK6B0KaUNQMDJZ1DrnhDXZ209pfpJoVybPk/CMcDKXaF2oRLKEOYVscXTF6mbiWUnMP5lj4OdTvFa0eVRcE0q9BckiqYgUZLK4L6DVgRXWRL5nRPtvEtd+qZe6psu0JZ7HEPhsbodfAVH2G436z1QahLGJXdQCoQv8UQ/quGQP2wCWemfueeKJ4Y6WsvEvmkUpizbTOE3Njjaw=")
        rval, frame = vc.read()
    else:
        return
    
    windowName = "Barcode Reader"
    formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000; # 1D, QRCODE, PDF417, DataMatrix

    while True:
        cv2.imshow(windowName, frame)
        rval, frame = vc.read();
        results = dbr.decodeBuffer(frame, formats)
        if (len(results) > 0):
            print(get_time())
            print("Total count: " + str(len(results)))
            for result in results:
                print("Type: " + result[0])
                print("Value: " + result[1] + "\n")

        # 'ESC' for quit
        key = cv2.waitKey(20)
        if key == 27:
            dbr.destroy()
            break

    cv2.destroyWindow(windowName)

if __name__ == "__main__":
    print("OpenCV version: " + cv2.__version__)
    read_barcode()
