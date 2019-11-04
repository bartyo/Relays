import os
import json
import time
import serial

#=====TO DO===============================#
#=Threading, one for BLE and one for HTTP=#
#=BLE implementation                     =#
#=POST request to check a device ID      =#
#=========================================#

class bluetooth:
    """"Class to define bluetooth connection"""
    _data = 0
    _port = "/dev/ttyAMA0"
    _baudrate = 9600
    _timeout = 2
    _light = 0
    _begin = "{"
    _end = "}"
    _endacquire = 0

    def __init__(self,port,baudrate,timeout):
        self._port = port
        self._baudrate = baudrate
        self._timeout = timeout
        return serial.Serial(self._port, self._baudrate, self._timeout)

    def serialize(self):

    def deserialize():

    def acquire():

    def get_data(self):
        return self._data

class http:
    """"Class to define http connection, define with :
    - her token
    - her data received
    """
    _data = 0
    _token = ""
    _id = ""
    _email = ""
    _password = ""
    _url = {
                'token':'https://projsante.herokuapp.com/api/auth',
                'devices':'https://projsante.herokuapp.com/api/relays/devices',
                'patient':'https://projsante.herokuapp.com/api/patients?_id=%s',
                'send_data':'https://projsante.herokuapp.com/api/relays/devices',
            }
    _header = {'content-type':'application/json','x-auth-token':''}
    _paramAuth = {'email':'','password':''}
    _paramData = 0
    _error = ""
    _timer = 0

    def __init__(self, token=""):
        self.add_headers(self.token)

    def __init__(self,email,password):
        self._email = email
        self.password = password
        self._paramAuth["email"] = email
        self._paramAuth["password"] = password

    def get_token(self):
        """get token authenticator from webapp"""
        self.set_timer(time.time())
        r = requests.post(url = self._url['token'], data = json.dumps(self._paramAuth), headers = self._header)
        token = json.loads(r.text)
        try:
            self._token = token["token"]
            self._header['x-auth-token'] = token
        except:
            self._error = "no token receive"

    def get_devices(self):
        self.set_timer(time.time())
        r = requests.get(url = _url['devices'], headers = self._header)
        return r

    def send_data(self):

    def is_device_exists(id):
        """Check if device is present into web database, use when device try to send data and its is is not stored localy"""
        r = requests.get(url = _url['patient']%(id,), headers = self._header)
        if r.status == 200:
            return 1
        else:
            return 0

    def add_headers_token(self, headers):
        self._header['x-auth-token'] = headers

    def get_token_dev(self):
        print(self._token)

    def get_error_dev(self):
        print(self._error)

    def set_timer(self, timer):
        self._timer = timer

    def get_timer(self):
        return self._timer




class Devices:
    """Classe définissant l'ensemble des devices caractérisée par :
    - son nombre"""
    _nbDevices = 0
    _listDevice = []
    def __init__(self):
        self._nbDevices += 1


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


def setup():
    # defining a params dict for the parameters to be sent to the API 
    #PARAMS = {'email':'test@test.com','password':'12345'}

    #HEADERS = {'content-type':'application/json'}
    token_r = http("test@test.com","12345")
    token_r.http_post(URL_AUTH)
    token_r.get_token()
    token_r.get_error()

    http_data = http(token_r.get_token())
    loop(http_data)
    # sending get request and saving the response as response object 
    #r = requests.post(url = URL_AUTH, data = json.dumps(PARAMS), headers = HEADERS) 
    
    # extracting data in json format 
    #data = r.text
    #global token
    #token = json.loads(data)
    #print(token["token"])
    global g_timer_relay
    g_timer_relay = time.time()

def loop(http_data):
    while 1:
        global g_timer_relay
        global g_sensor

        nn = input()
        print("nn : ",nn)
        #print(time.time()-g_timer_relay)
        if time.time()-http_data.get_timer() > 10:
            http_data.set_timer(time.time())
            dataJson = json.loads(http_data.http_get())

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


os.system("pause")
