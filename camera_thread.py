import cv2
import dbr
import time
import threading
import Queue

q = Queue.Queue(1)

class BarcodeReaderThread (threading.Thread):
    def __init__(self, name, isRunning):
        threading.Thread.__init__(self)
        self.name = name
        self.isRunning = isRunning

    def run(self):
        global q
        formats = 0x3FF | 0x2000000 | 0x8000000 | 0x4000000; # 1D, QRCODE, PDF417, DataMatrix

        while self.isRunning:
            # Get a frame
            frame = q.get(True)
            results = dbr.decodeBuffer(frame, formats)
            q.task_done()

            if (len(results) > 0):
                print(get_time())
                print("Total count: " + str(len(results)))
                for result in results:
                    print("Type: " + result[0])
                    print("Value: " + result[1] + "\n")

        print("Quit thread")

def get_time():
    localtime = time.localtime()
    capturetime = time.strftime("%Y%m%d%H%M%S", localtime)
    return capturetime

def read_barcode():
    vc = cv2.VideoCapture(0)

    if vc.isOpened(): # try to get the first frame
        dbr.initLicense("t0260NQAAALGw+aCAePXdOS3p1xkqT5hesExKVpEe7NiIhkdlUz/Jvx8km3ItI0ykUcmeP67BYVlJ2PDW++bjSYmDLmyMgOmmvc0mdvhlSy500kqnLoBAL+TybcdAP42b5p5WehK9Gsmweqi+ydK6B0KaUNQMDJZ1DrnhDXZ209pfpJoVybPk/CMcDKXaF2oRLKEOYVscXTF6mbiWUnMP5lj4OdTvFa0eVRcE0q9BckiqYgUZLK4L6DVgRXWRL5nRPtvEtd+qZe6psu0JZ7HEPhsbodfAVH2G436z1QahLGJXdQCoQv8UQ/quGQP2wCWemfueeKJ4Y6WsvEvmkUpizbTOE3Njjaw=")
        rval, frame = vc.read()
    else:
        return
    
    windowName = "Barcode Reader"

    # Create a thread for barcode detection
    barcodeReaderThread = BarcodeReaderThread("Barcode Reader Thread", True)
    barcodeReaderThread.start()

    global q
    while True:
        cv2.imshow(windowName, frame) # Render a frame on Window
        rval, frame = vc.read(); # Get a frame

        try:
            q.put_nowait(frame)
        except Queue.Full:
            try:
                q.get_nowait()
            except Queue.Empty:
                None

        # 'ESC' for quit
        key = cv2.waitKey(20)
        if key == 27:
            barcodeReaderThread.isRunning = False
            barcodeReaderThread.join()
            dbr.destroy()
            break

    cv2.destroyWindow(windowName)

if __name__ == "__main__":
    print "OpenCV version: " + cv2.__version__
    read_barcode()
