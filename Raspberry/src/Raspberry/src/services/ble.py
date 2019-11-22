import serial
import json

class Bluetooth:
    """"Class to define bluetooth connection"""
    _data = ""
    _port = "/dev/ttyAMA0"
    _baudrate = 9600
    _timeout = 2
    _light = 0
    _begin = "{"
    _end = "}"
    _receip = {'begin':0,'end':0}
    _endacquire = 0
    _serial = ""

    def __init__(self):
        self._serial = serial.Serial(self._port, self._baudrate, timeout = self._timeout)

    def get_serial(self):
        return self._serial

    def serialize(self):
        try:
            self._data = self._data.replace("\r\n","")
            self._data = json.dumps(self._data)
        except:
            self._data = json.dumps(self._data)

    def acquire(self):
        r = self._serial.read()
        if r == self._begin:
            self._receip['begin'] = 1
            self._receip['end'] = 0
            self._endacquire = 0

        if r != "" and self._receip['begin'] == 1 and self._receip['end'] == 0:
            self._data = self._data + r

        if r == self._end:
            self._receip['begin'] = 1
            self._receip['end'] = 0
            self._endacquire = 1

    def get_data(self):
        self._endacquire = 0
        data = self._data
        self._data = ""
        return data

    def get_endacquire(self):
        return self._endacquire

    def send_data(self, data):
        self._serial.write(data)