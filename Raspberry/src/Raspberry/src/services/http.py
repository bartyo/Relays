import requests
import json
import time

class Http:
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

    def __init__(self,email,password):
        self._email = email
        self.password = password
        self._paramAuth["email"] = email
        self._paramAuth["password"] = password
        self.get_token()

    def get_token(self):
        """get token authenticator from webapp"""
        self.set_timer(time.time())
        r = requests.post(url = self._url['token'], data = json.dumps(self._paramAuth), headers = self._header)
        token = json.loads(r.text)
        try:
            self._token = token["token"]
            self._header['x-auth-token'] = self._token
        except:
            self._error = "no token receive"

    def get_devices(self):
        self.set_timer(time.time())
        r = requests.get(url = self._url['devices'], headers = self._header)
        return json.loads(r.text)

    def send_data(self):
        a=1

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