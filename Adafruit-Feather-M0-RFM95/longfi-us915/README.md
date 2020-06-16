# Adafruit Feather M0 RFM95

[Quickstart Guide](https://developer.helium.com/devices/arduino-quickstart/adafruit-feather-m0-rfm95)
[Adafruit Feather M0 with RFM95 Product Page](https://www.adafruit.com/product/3178)  
[Adafruit Feather M0 with RFM95 Datasheets & Files](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/downloads).

## Required Arduino Libraries

### Important
Make sure you do not have other LMIC arduino libraries installed, otherwise you will experience 
build errors. You can find your Arduino libraries for your operating system below, simply delete the directory if you would like to remove a library.

linux: /home/{user}/Arduino/libraries
windows: Documents/Arduino/libraries
mac os: Documents/Arduino/libraries 

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type the library name below and install version `1.5.0+arduino-2`.

[IBM LMIC framework](https://github.com/matthijskooijman/arduino-lmic)  

### Required Configuration
This library requires that a config file be setup properly. After you have installed the `IBM LMIC framework` library, navigate to it's directory on your operating system found below. Next, replace the `config.h` file in this directory with this [config.h](https://github.com/helium/longfi-arduino/blob/master/Sparkfun-Pro-RF/longfi-us915/config.h) file.

linux: /home/{user}/Arduino/libraries/IBM_LMIC_framework/src/lmic  
windows: Documents/Arduino/libraries/IBM_LMIC_framework/src/lmic  
mac os: Documents/Arduino/libraries/IBM_LMIC_framework/src/lmic  

## Required Arduino Board Support

1. Install Arduino SAMD Support, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-samd-support-6-5).  
2. Install Adafruit SAMD Support, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-adafruit-samd-6-7).  
3. (Windows 7 & 8 Only) Install drivers, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-drivers-windows-7-and-8-only-6-11)  