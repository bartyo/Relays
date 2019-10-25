import os
import json
import requests
import time


class Devices:
    """Classe définissant l'ensemble des devices caractérisée par :
    - son nombre"""
    _nbDevices = 0
    _listDevice = []
    def __init__(self):
        self._nbDevices +=1


class Device:
    """Classe définissant un device caractérisée par :
    - son id
    - sa satO2
    - son gainO2
    - son pulse
    - sa gainPulse
    - son status
    - ses données brutes du BLE """
    _id = 0
    _satO2 = 0
    _gainO2 = 0
    _pulseO2 = 0
    _gainPulseO2 = 0
    _status = 0
    _data = [[0],[0]]
    
    def __init__(self,id): # Notre méthode constructeur
        """Pour l'instant, on ne va définir qu'un seul attribut"""
        self._id = id
        self._satO2 = 0
        self._gainO2 = 0
        self._pulse = 0
        self._gainPulse = 0
        self._status = 1
        self._data = 0

    def set_id(self,id):
        self._id = id
    def set_satO2(self,satO2):
        self._satO2 = satO2
    def set_gainO2(self,gainO2):
        self._gainO2 = gainO2
    def set_pulse(self,pulse):
        self._pulse = pulse
    def set_gainPulse(self,gainPulse):
        self._gainPulse = gainPulse
    def set_status(self,status):
        self._status = status
    def set_data(self,data):
        self._data = data
    def get_id(self):
        return self._id
    def equalId(self,id):
        if id == self._id:
            return 1


URL_AUTH = "https://projsante.herokuapp.com/api/auth"
URL_RELAYS = "https://projsante.herokuapp.com/api/relays/devices"
g_timer_relay = 0
g_sensor = []

def main():
    setup()
    while 1:
        loop()


def setup():
    # defining a params dict for the parameters to be sent to the API 
    PARAMS = {'email':'test@test.com','password':'12345'}

    HEADERS = {'content-type':'application/json'}
    
    # sending get request and saving the response as response object 
    r = requests.post(url = URL_AUTH, data = json.dumps(PARAMS), headers = HEADERS) 
    
    # extracting data in json format 
    data = r.text
    global token
    token = json.loads(data)
    print(token["token"])
    global g_timer_relay
    g_timer_relay = time.time()

def loop():
    global g_timer_relay
    global g_sensor
    #print(time.time()-g_timer_relay)
    if time.time()-g_timer_relay > 10:
        g_timer_relay = time.time()
        HEADERS_RELAYS = {'x-auth-token':token["token"]}
        r = requests.get(url = URL_RELAYS, headers = HEADERS_RELAYS)
        data = r.text
        dataJson = json.loads(data)
        #print(range(dataJson))
        for elem in range(len(dataJson)):

            #print(json.loads(elem)["id"])
            is_device_exists(dataJson[elem]["id"])
            #print(dataJson[elem]["id"])
        display_sensors()

def is_device_exists(id):
    global g_sensor
    a=0
    for obj in g_sensor:
        if obj.get_id() == id:
            a = a + 1
        else:
            a = a + 0
    if a == 0:
        add_device(id)

def add_device(id):
    global g_sensor
    g_sensor.append(Device(id))

def is_device_enabled(ids):
    #Comparer tout g_sensor avec la réponse json
    a = 0
    for obj in g_sensor:
        for json in range(len(ids)):
            if ids[json]["id"] == obj.get_id():
                a = a + 1
            else:
                a = a + 0
            if a == 0:
                obj.set_status(0)
                #remove g_sensor

def display_sensors():
    print("#============================#")
    for obj in g_sensor:
        print(obj.get_id())
    print("#============================#")


main()


print("C'est loupie et loupie les plus belles")
os.system("pause")
