# 2-Factor Key Fob Project Repository

![Project Image](project_image.png)

Welcome to the 2-Factor Key Fob project repository! This repository contains all the necessary information and design files for a custom board designed to provide a 2-factor entry system for vehicles. The key fob incorporates an ESP8266 and an RFM69HW radio module, and its primary goal is to enhance vehicle security by jamming all key signals near the car. The jammer can be disabled by accessing the Access Point (AP) created by the ESP8266 and accessing the web server at the IP address 69.69.69.69. Additionally, there is a button on the web page to disable the jammer manually.

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Getting Started](#getting-started)
4. [Hardware](#hardware)
5. [Firmware](#firmware)
6. [Usage](#usage)
7. [Contributing](#contributing)
8. [License](#license)

## Introduction

The 2-Factor Key Fob project aims to enhance vehicle security by adding a two-factor authentication system to the standard key fob. By utilizing an ESP8266 and an RFM69HW radio module, the key fob can jam all key signals near the car, preventing unauthorized access. To disable the jammer, users can connect to the ESP8266's Access Point and access the web server hosted at IP address 69.69.69.69, which allows manual control of the jammer.

## Features

- Two-factor entry system for vehicles
- ESP8266 WiFi module for jammer control
- RFM69HW radio module for key signal jamming
- Web server for manual jammer control
- Easy-to-use interface for disabling the jammer

## Getting Started

To get started with the 2-Factor Key Fob project, follow these steps:

1. **Clone the Repository**: Clone this repository to your local machine using `git clone https://github.com/your-username/2-factor-key-fob.git`.

2. **Hardware Setup**: Assemble the custom board designed using JLCPCB, incorporating the ESP8266 and RFM69HW radio module as per the provided design files.

3. **Firmware Installation**: Flash the firmware onto the ESP8266 using your preferred method (e.g., Arduino IDE, PlatformIO, etc.).

4. **Deploy Web Server**: Deploy the web server on the ESP8266 and configure it to listen at IP address 69.69.69.69.

5. **Connect and Test**: Power on the board, and you should see the ESP8266's Access Point available. Connect to it using your device and access the web server to enable/disable the jammer manually.

## Hardware

The "hardware" directory contains all the design files necessary for manufacturing the 2-Factor Key Fob board. The files include schematic, PCB layout, and Bill of Materials (BOM). These files were designed using JLCPCB's platform and can be directly uploaded for fabrication.

## Firmware

The "firmware" directory contains the code required to run on the ESP8266 microcontroller. You can use the Arduino IDE or any other compatible development environment to upload the firmware to the board.

## Usage

To use the 2-Factor Key Fob, follow these steps:

1. Power on the key fob board.

2. Wait for the ESP8266 to create an Access Point (AP). It will be named "2FA-KeyFob-XXXX" or something similar.

3. Connect your smartphone or computer to the ESP8266's Access Point.

4. Open a web browser and navigate to "http://69.69.69.69" to access the web server.

5. The web page will display a button to enable/disable the jammer. Click on the appropriate button to control the jammer.

6. When the jammer is enabled, the key signals near the car will be jammed, providing additional security.

7. To disable the jammer, simply return to the web page and click the disable button.

## Contributing

We welcome contributions to the 2-Factor Key Fob project! If you find any issues or have ideas to improve the project, feel free to open an issue or submit a pull request. Please refer to the [Contributing Guidelines](CONTRIBUTING.md) for more information on how to contribute.

## License

The 2-Factor Key Fob project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute this project as per the terms of the license.

---

Thank you for showing interest in the 2-Factor Key Fob project. If you have any questions or feedback, please don't hesitate to get in touch with us!
