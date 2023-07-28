# 2-Factor Key Fob Board

This repository contains all the necessary information and design files for a custom board designed to provide a 2-factor entry system for vehicles. The board incorporates an ESP8266 and an RFM69HW radio module, and its primary goal is to enhance vehicle security by jamming all key signals near the car. The jammer can be disabled by accessing the Access Point (AP) created by the ESP8266 and accessing the web server at the IP address 69.69.69.69, where there is a button on the web page to disable the jammer manually.
The 2-Factor Key Fob project aims to enhance vehicle security by adding a two-factor authentication system to the standard key fob. By utilizing an ESP8266 and an RFM69HW radio module, the key fob can jam all key signals near the car, preventing unauthorized access. To disable the jammer, users can connect to the ESP8266's Access Point and access the web server hosted at IP address 69.69.69.69, which allows the user to disable the jammer.

![WhatsApp Image 2023-07-28 at 23 09 05](https://github.com/luispl77/2factorkeyfob/assets/81360502/a04371ca-98d2-49d9-993f-6a3bc589fb11)


## Hardware

The "hardware" directory contains all the design files necessary for manufacturing the 2-Factor Key Fob board. These files were designed using JLCPCB's platform and can be directly uploaded for fabrication. The EasyEDA project can also be accessed here: https://oshwlab.com/maarnotto/jammer_usb_stick

## Firmware

The "firmware" directory contains the code required to run on the ESP8266 microcontroller. You can use the Arduino IDE or any other compatible development environment to upload the firmware to the board.

## Demo

https://www.youtube.com/watch?v=CixVvEEoNlQ

## Contact me

If you have any questions or feedback, please contact me on Discord: luispl#9021
