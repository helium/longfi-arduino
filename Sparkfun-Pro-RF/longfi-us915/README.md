# Sparkfun Pro RF - Basic LongFi Example

This example demonstrates sending a simple data packet using a Sparkfun Pro RF development board. Visit our quickstart guide [here](https://developer.helium.com/devices/arduino-quickstart/sparkfun-pro-rf).

## Required Arduino Libraries

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type the library name below and install the latest version.

[IBM LMIC framework](https://github.com/matthijskooijman/arduino-lmic)  

### Required Configuration
This library requires that a config file be setup properly. After you have installed the `IBM LMIC framework` library, navigate to it's directory on your operating system found below. Next, replace the `config.h` file in this directory with this [config.h](config.h) file.

linux: /home/{user}/Arduino/libraries/IBM_LMIC_framework/src/lmic  
windows: Documents/Arduino/libraries/IBM_LMIC_framework/src/lmic  
mac os: Documents/Arduino/libraries/IBM_LMIC_framework/src/lmic  

## Required Arduino Board Support

### Arduino SAMD Boards (32-bits ARM Cortex-M0+) + SparkFun SAMD Boards
Install board support packages, find instructions [here](https://learn.sparkfun.com/tutorials/sparkfun-samd21-pro-rf-hookup-guide?_ga=2.148378999.1172134851.1586114454-289367592.1582349414&_gac=1.242421430.1585837307.EAIaIQobChMI86GEgfjJ6AIVBQF9Ch0mpwyeEAEYASAAEgLFn_D_BwE#setting-up-arduino).  

Arduino IDE:  
1. Select Tools -> Board: -> SparkFun SAMD21 Pro RF 

## Required Hardware

### Sparkfun Pro RF
* [Sparkfun Pro RF Product Page](https://www.sparkfun.com/products/14916?_ga=2.151319289.1172134851.1586114454-289367592.1582349414&_gac=1.175849750.1585837307.EAIaIQobChMI86GEgfjJ6AIVBQF9Ch0mpwyeEAEYASAAEgLFn_D_BwE#)  
* [Sparkfun Pro RF ](https://learn.sparkfun.com/tutorials/sparkfun-samd21-pro-rf-hookup-guide?_ga=2.5036659.1172134851.1586114454-289367592.1582349414&_gac=1.244975921.1585837307.EAIaIQobChMI86GEgfjJ6AIVBQF9Ch0mpwyeEAEYASAAEgLFn_D_BwE)