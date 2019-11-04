#-*- coding: utf-8 -*-
#
# Test du port série
import serial
import time

g_timer_relay = 0
g_timer_relay = time.time()
test_string = "Je teste le port serie 1 2 3 4 5"
debut = "{"
fin = "}"
begin_receip = 0
end_receip = 0
str_loopback = ""
printed = 0
#port_list = ["/dev/ttyAMA0", "/dev/ttyAMA0", "/dev/ttyS0", "/dev/ttyS0",]
port = "/dev/ttyAMA0"
serialPort = serial.Serial(port, 9600, timeout = 2)
while 1:
  try:
    #print "Port Serie ", port, " ouvert pour le test :"
    #bytes_sent = serialPort.write("Salut")
    #print "Envoye ", bytes_sent, " octets"
    loopback = serialPort.read()
    if loopback == debut:
      begin_receip = 1
      end_receip = 0
      printed = 0
         
    if loopback != "" and begin_receip == 1 and end_receip == 0:
      str_loopback = str_loopback + loopback
      
    if loopback == fin:
      begin_receip = 0
      end_receip = 1
      
    if begin_receip == 0 and end_receip == 1 and printed == 0:
      print(str_loopback)
      printed = 1
      str_loopback = ""
    #serialPort.close()
  except IOError:
    print "Erreur sur ", port, ""
