import os

listDevice = [1,2,3,4,5,6,7,8,9,10]
listId = [2,18,3,5,8,7,10,9,1,12,84,13]

newListDevice = []
removeList = []

a = 0
i = 0

#on supprime les devices qui ne sont plus attribué
for device in listDevice:
    for id in listId:
        if device == id:
            newListDevice.append(id)
            a = 0
            break
        a = a + 1
    if a == len(listId):
        removeList.append(device)
    a = 0
for remove in removeList:
    listDevice.remove(remove)

#il faut ajouter ceux en plus
for id in listId:
    for device in listDevice:
        if id == device:
            a = 0
            break
        a = a + 1
    if a == len(listDevice):
        listDevice.append(id)
    a = 0
    

os.system("pause")