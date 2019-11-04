import serial

class bluetooth:
    """"Class to define bluetooth connection"""
    _data = 0
    _port = "/dev/ttyAMA0"
    _baudrate = 9600
    _timeout = 2
    _light = 0
    _begin = "{"
    _end = "}"
    _receip = {'begin':0,'end':0}
    _endacquire = 0
    _serial = 0

    def __init__(self):
        self._serial = serial.Serial(self._port, self._baudrate, self._timeout)
        return _serial


    def serialize(self):
        self._data = json.dumps(self._data)

    def acquire():
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
        return self._data

    def get_endacquire(self):
        return self._endacquire