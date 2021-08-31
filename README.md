<p align="center">
  <a href="" rel="noopener">
 <img width=1000px height=200px src="Circuit/pickcounter.png" alt="Project logo"></a>
</p>

<h3 align="center">PickCounter</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()


</div>

---


<p align="center"> PickCounter
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Installing](#installing)
- [PythonScript Configuration](#pythonscript)
- [Built Using](#built_using)
- [Authors](#authors)


## 🧐 About <a name = "about"></a>

This repo contains firmware and configuration instructions for PickCounter Projects

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See [deployment](#deployment) for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them.

```
- Arduino IDE
- A PC with python3 installed
```

### Installing <a name = "installing"></a>

A step by step series that tell you how to get the Firmware and Backend running

#### ESP32 Configuration

You should have Arduino IDE Installed

  1.  Add ESP32 Board to your Arduino IDE
    1. In your Arduino IDE, go to File> Preferences
        Installing ESP32 Add-on in Arduino IDE Windows, Mac OS X, Linux open preferences
    2. Enter ```https://dl.espressif.com/dl/package_esp32_index.json``` 
        into the “Additional Board Manager URLs” field then, click the “OK” button:
    Note: if you already have the ESP32 boards URL, you can separate the URLs with a comma as follows:
    ```https://dl.espressif.com/dl/package_esp32_index.json,
      http://arduino.esp8266.com/stable/package_esp8266com_index.json```
    1. Open the Boards Manager. Go to Tools > Board > Boards Manager…
    2. Search for ESP32 and press install button for the ESP32 by Espressif Systems“:
    3. That’s it. It should be installed after a few seconds.

  2.  Now copy the contents of the libs folder to the libraries directory of your Arduino
    4. If you are using windows, the libraries directory will be Documents/Arduino/libraries

##### ESP32 Node FW Uploading
  1.  Select ESP32 Dev Module from Tools->Board->ESP32
  2.  Select the correct port from Tools->Port
  3.  Then open ESP32Firmware.ino file, and open WiFiCreds.h tab on line number 4 and 5, put your WiFi creds
  4.  Upload the Code to your ESP32 Wrover Module Gateway Board




## 🎈 PythonScript Configuration <a name="pythonscript"></a>

1.  Download and install Python3 from official website: https://www.python.org/downloads/
2.  Go in the PythonScript folder and open the terminal
3.  Execute ```pip3 install -r requirements.txt```
### Python Script running

- Run the python script using ```python3 Script.py```
- The script will start running, you can enter the color and the values which will be instantly sent to the ESP32 and the ESP32 will send back the response.
## ⛏️ Built Using <a name = "built_using"></a>

- [Arduino](https://www.arduino.cc/) - IDE
- [Python](https://www.python.org/downloads/) - Programming Language

## Demo Video

- [PickCounter](https://youtu.be/y3RfxgQILcE) - PickCounter ESP32 and Python Script Demo Video

## ✍️ Authors <a name = "authors"></a>

- [@Nauman3S](https://github.com/Nauman3S) - Development and Deployment

