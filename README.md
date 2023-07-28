# 2-Factor Key Fob Board

This repository contains all the necessary information and design files for a custom board designed to provide a 2-factor entry system for vehicles. The board incorporates an ESP8266 and an RFM69HW radio module, and its primary goal is to enhance vehicle security by jamming all key signals near the car. The jammer can be disabled by accessing the Access Point (AP) created by the ESP8266 and accessing the web server at the IP address 69.69.69.69, where there is a button on the web page to disable the jammer manually.

The goal of this project is to make it impossible for a thief to open a vehicle using the stolen key fob. The thief would need to know the series of steps to deactivate the jammer, along with the AP's password, creating a two-factor system. Note that the thief might be able to use the mechanical key in the key fob, so some aditional thought would need to be added, to make it truly impossible for a thief to (non-destructively) enter the vehicle with the key.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/eeb82920-18ce-409a-95a3-c509b7b5a05f)


## Demo

https://www.youtube.com/watch?v=CixVvEEoNlQ

## Hardware
![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/3be17873-e7d1-46d0-a409-c72e3b98c3fa)
![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/96d09399-80df-4427-a9b1-1bcdfe1d85eb)


The "hardware" directory contains all the design files necessary for manufacturing the 2-Factor Key Fob board. These files were designed using JLCPCB's platform and can be directly uploaded for fabrication. The EasyEDA project can also be accessed here: https://oshwlab.com/maarnotto/jammer_usb_stick

You can choose to build 3+ versions of this board, with different features:
### Build 1: ESP8266 + RFM69HW 
This is the recommended build.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/c81abc7b-b7d9-4066-be22-8ebb0e19cc0d)

### Build 2: ESP8266 + RFM69HW + 555 timer
The 555 timer circuit combined with the MOSFET adds a periodic (~2 min depending on the resistor values RA and RB) switch to the power of the board, as a redundancy mechanism. The periodic reset is a nice touch to Build 1, but not needed.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/6fd9d55a-070d-4b45-bb97-47fb49bd8e50)

### Build 3: 555 timer + FS1000A 
This version is very simply switching the jammer OFF periodically (~15 sec, depending on resistor values) for ~1 sec, allowing the user to look at the indicator LED to know exacly when to press the key fob. This mechanism is weaker because an attacker can just repeatedly use the key fob and get a lucky click inside the ~1 sec window.

![image](https://github.com/luispl77/2factorkeyfob/assets/81360502/3a496d27-9c9b-4ed6-bffc-acc625bd5cef)


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
