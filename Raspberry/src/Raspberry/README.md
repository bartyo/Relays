# Raspberry

## Concept
Raspberry communicate with devices through BLE and with webapp through HTTP and WIFI.
## Functionnal specifications
BLE is managed with HM-10 module.


## Material
* Raspberry pi 3B+ for processing and HTTP
* HM-10 bluetooth4.0 for BLE

## Raspberry setup
On raspberry UART is used by embeded bluetooth. In this application we need UART to dialog with HM-10.
![GPIO](https://www.framboise314.fr/wp-content/uploads/2017/02/port_serie_GPIO.png)

To disabled embeded bluetooth you have to be in superuser mode and add `dtoverlay = pi3-disable-bt` into /boot/config.txt and delete `console=serial0,115200` into /boot/cmdline.txt. Then reboot the raspberry.

Next, you can check these modifications with command `ls -l /dev`, you should get
![serial0->ttyAMA0](https://www.framboise314.fr/wp-content/uploads/2017/02/port_serie_install_02.png)

## Software

### Libraries used
* json for JSON format
* time for timers
* requests for HTTP protocols
* serial for BLE through UART


### Classes
In this project there are 5 classes
* Http which manages http protocol through wifi
* Bluetooth which manages BLE with HM-10 via UART
* Devices which is the list of all devices
* Device which represent each device paired
* Process which process data received from devices before they are sent to webapp