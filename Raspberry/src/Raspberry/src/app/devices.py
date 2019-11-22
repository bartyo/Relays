class Devices:
    """Classe définissant l'ensemble des devices caractérisée par :
    - son nombre
    - list of device"""
    _nbDevices = 0
    _listDevice = []
    def __init__(self):
        self._nbDevices += 1

    def add_device(self,id):
        a=0
        for elem in self._listDevice:
            if elem == id:#id existe dans la liste
                a=a+1

        if a==0:
            dev = Device(id) 
            self._listDevice.append(dev)

    def upload_list(self, listId): 
        a = 0
        i = 0
        removeList = []
        #on supprime les devices qui ne sont plus attribué
        for device in self._listDevice:
            for id in listId:
                #print("id : ",id," : device", device)
                if device.get_id() == id:
                    a = 0
                    break
                a = a + 1
            if a == len(listId):
                removeList.append(Device(id))
            a = 0
        for remove in removeList:
            self._listDevice.remove(remove)

        #il faut ajouter ceux en plus
        for id in listId:
            for device in self._listDevice:
                if id == device:
                    a = 0
                    break
                a = a + 1
            if a == len(self._listDevice):
                self._listDevice.append(Device(id))
            a = 0

            
    def display_list(self):
        for elem in self._listDevice:
            print(elem.get_id())
        


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