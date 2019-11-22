""""Scheduleur"""
import time
import json
import os
#from services import ble
from services import http
from app import devices

def main():
    objects = setup()
    objHttp = objects[0]
    objDevices = objects[1]
    timer = time.time()
    loop(objHttp, objDevices, timer)

def setup():
    objHttp = http.Http("test@test.com","12345")
    #objBle = ble.bluetooth()
    objDevices = devices.Devices()
    

    return objHttp, objDevices


def loop(objHttp, objDevices, timer):
    while 1:
        if time.time()-timer > 5:
            print(-timer + time.time())
            timer = time.time()
            
            device = objHttp.get_devices()
            objDevices.upload_list(device)
            print("*************")
            objDevices.display_list()

main()

#os.system("pause")