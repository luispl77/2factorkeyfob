# 2-Factor Key Fob Board

This repository contains all the necessary information and design files for a custom board designed to provide a 2-factor entry system for vehicles. The board incorporates an ESP8266 and an RFM69HW radio module, and its primary goal is to enhance vehicle security by jamming all key signals near the car. The jammer can be disabled by accessing the Access Point (AP) created by the ESP8266 and accessing the web server at the IP address 69.69.69.69, where there is a button on the web page to disable the jammer manually.
The 2-Factor Key Fob project aims to enhance vehicle security by adding a two-factor authentication system to the standard key fob. By utilizing an ESP8266 and an RFM69HW radio module, the key fob can jam all key signals near the car, preventing unauthorized access. To disable the jammer, users can connect to the ESP8266's Access Point and access the web server hosted at IP address 69.69.69.69, which allows the user to disable the jammer.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/a04371ca-98d2-49d9-993f-6a3bc589fb11)

## Demo

https://www.youtube.com/watch?v=CixVvEEoNlQ

## Hardware
![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/5efef5d2-6ad4-40b3-922d-d6b00389254c)
![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/c95a549a-fbde-4cf2-8d7e-cea3a4633279)

The "hardware" directory contains all the design files necessary for manufacturing the 2-Factor Key Fob board. These files were designed using JLCPCB's platform and can be directly uploaded for fabrication. The EasyEDA project can also be accessed here: https://oshwlab.com/maarnotto/jammer_usb_stick

You can choose to build 3+ versions of this board, with different features:
### Build 1: ESP8266 + RFM69HW 
This is the recommended build.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/c47348b1-75ef-43fd-ae79-6114c2d5e851)
### Build 2: ESP8266 + RFM69HW + 555 timer
The 555 timer circuit combined with the MOSFET adds a periodic (~2 min depending on the resistor values RA and RB) switch to the power of the board, as a redundancy mechanism. The periodic reset is a nice touch to Build 1, but not needed.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/d400cf27-3fcb-420c-869c-0e0b0cded81a)
### Build 3: 555 timer + FS1000A 
This version is very simply switching the jammer OFF periodically (~15 sec, depending on resistor values) for ~1 sec, allowing the user to look at the indicator LED to know exacly when to press the key fob. This mechanism is weaker because an attacker can just repeatedly use the key fob and get a lucky click inside the ~1 sec window.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/536c1ec9-6116-45b4-8d5d-ce6e3e6f8175)

Some other possible builds would be substituting the RFM69HW for the FS1000A. Note that the jamming power of the FS1000A is significantly lower to the RFM69HW.

## Firmware

The "firmware" directory contains the code required to run on the ESP8266 microcontroller. It depends on the [RFM69LPL library](https://github.com/luispl77/RFM69LPL), which is my custom made RFM69 module library, and is mantained by me. Make sure to include this library in Arduino IDE in order to compile and upload to the ESP8266.

The device waits 60 sec before starting the jammer once pluged in, and the last state of the jammer was 0, to give time to the user to lock the vehicle. Address 0x00 of the EEPROM (flash) of the ESP8266 keeps the state of the jammer (1/0).

This firmware controls the behavior of the key fob, allowing users to jam key signals near the car when they leave the vehicle and to disable the jammer when needed. The key fob creates an Access Point (AP) and a web server, accessible through the AP, to enable or disable the jammer.

(AI generated:)

This firmware controls the behavior of the key fob, allowing users to jam key signals near the car when they leave the vehicle and to disable the jammer when needed. The key fob creates an Access Point (AP) and a web server, accessible through the AP, to enable or disable the jammer.

### Key Features:

1. **Jammer Control:** The firmware sets up and controls the RFM69HW radio module to act as the key signal jammer. The frequency can be customized by changing the value in the line `radio.setFrequencyMHz(433.92);` to suit the jamming requirements.

2. **Web Server & AP:** The ESP8266 creates a WiFi Access Point (AP) with the SSID "iPhone de Maria" and password "12345678". Users can connect their smartphones or computers to this AP and access the built-in web server hosted at IP address 69.69.69.69. The web server provides an intuitive interface to enable or disable the jammer using buttons.

3. **EEPROM Memory:** The firmware utilizes the EEPROM library to store the state of the jammer (active or inactive) in the memory location 0x00. The function `retrieveState()` reads this memory location during startup to determine the previous state of the jammer.

### Functionality:

1. The firmware initializes the RFM69HW radio module with the specified frequency and transmit power settings to act as the jammer. It sets up the WiFi AP and web server for user interaction.

2. During startup, the firmware checks the EEPROM memory location 0x00 to retrieve the previous state of the jammer. If the state is "1", it indicates that the jammer was previously active, and it proceeds to activate the jammer again. Otherwise, it provides a 60-second delay, allowing the user to lock the vehicle before the jammer is activated.

3. When a user connects to the ESP8266's AP and accesses the web server, the firmware responds to HTTP GET requests for turning the jammer on or off. When the jammer is activated or deactivated, the corresponding state is stored in the EEPROM memory, ensuring the jammer retains its state even after power cycling.

4. The web server's user interface provides buttons to toggle the jammer state. When the jammer is active, the ESP8266 sets output pin 4 (GPIO4) high, initiating the jamming process. It also sets GPIO2 low to power off the green LED, indicating the jammer is active.

5. Conversely, when the jammer is deactivated, GPIO4 is set low to turn off the jamming signal, and GPIO2 is set high to power on the green LED, indicating the jammer is inactive.


## Contact me

If you have any questions or feedback, please contact me on Discord: luispl#9021
