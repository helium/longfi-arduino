# ST B-L072Z-LRWAN1 - GNSS Example

This example demonstrates sending GNSS data in [CayenneLPP](https://developers.mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload) format, using a B-L072Z-LRWAN1 development board with a X-NUCLEO-GNSS1A1 expansion shield, to the myDevices Cayenne dashboard. For more information on adding your device to the Helium network, visit our quickstart guide [here](https://developer.helium.com/console/quickstart). For more information on adding your device to myDevices Cayenne, visit our guide [here](https://developer.helium.com/console/integrations/mydevices-cayenne-integration).

## Required Arduino Libraries

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type the library name below and install the latest version.

[MCCI Arduino LoRaWAN Library](https://github.com/mcci-catena/arduino-lmic)  
[CayenneLPP](https://github.com/ElectronicCats/CayenneLPP)  
[X-NUCLEO-GNSS1A1](https://github.com/stm32duino/X-NUCLEO-GNSS1A1)  

## Required Arduino Board Support

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit  
Install board support package, find instructions [here](https://github.com/stm32duino/Arduino_Core_STM32#getting-started).  

Arduino IDE:  
1. Select Tools -> Board: -> Discovery 
2. Select Tools -> Board part number: -> Discovery L072Z-LRWAN1 

## Required Hardware

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

### X-NUCLEO-GNSS1A1 - ST GNSS expansion board based on Teseo-LIV3F module

[X-NUCLEO-GNSS1A1 Product Page](https://www.st.com/en/ecosystems/x-nucleo-gnss1a1.html)  
[X-NUCLEO-GNSS1A1 User Manual](https://www.st.com/resource/en/user_manual/dm00453103-getting-started-with-the-xnucleognss1a1-expansion-board-based-on-teseoliv3f-tiny-gnss-module-for-stm32-nucleo-stmicroelectronics.pdf)  
## Programming (Uploading Method):

#### STM32CubeProgrammer(SWD)
Will use onboard ST-Link(Flasher/Debugger) to upload sketch.  
Download and Install required utility from ST [here](https://www.st.com/en/development-tools/stm32cubeprog.html).  

Arduino IDE:  
Select Tools -> Upload Method -> STM32CubeProgrammer(SWD)