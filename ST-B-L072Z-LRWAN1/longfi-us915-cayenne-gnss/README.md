# ST B-L072Z-LRWAN1 - Cayenne GNSS Example

This example demonstrates sending GNSS data in [CayenneLPP](https://developers.mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload) format, using a B-L072Z-LRWAN1 development board with a X-NUCLEO-GNSS1A1 expansion shield, to the myDevices Cayenne dashboard. For more information on adding your device to the Helium network, visit our quickstart guide [here](https://developer.helium.com/console/quickstart). For more information on adding your device to myDevices Cayenne, visit our guide [here](https://developer.helium.com/console/integrations/mydevices-cayenne-integration).

## Required Hardware

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

### X-NUCLEO-GNSS1A1 - ST GNSS expansion board based on Teseo-LIV3F module

[X-NUCLEO-GNSS1A1 Product Page](https://www.st.com/en/ecosystems/x-nucleo-gnss1a1.html)  
[X-NUCLEO-GNSS1A1 User Manual](https://www.st.com/resource/en/user_manual/dm00453103-getting-started-with-the-xnucleognss1a1-expansion-board-based-on-teseoliv3f-tiny-gnss-module-for-stm32-nucleo-stmicroelectronics.pdf)  

## Required Driver (Windows Only)
Download driver [here](https://www.st.com/en/development-tools/stsw-link009.html).

## Required Arduino Libraries

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type the library name below and install the latest version.

[CayenneLPP](https://github.com/ElectronicCats/CayenneLPP)  
[X-NUCLEO-GNSS1A1](https://github.com/stm32duino/X-NUCLEO-GNSS1A1)  

## Required Board Support

### Arduino Core for STM32L0 
Arduino IDE:  
1. Navigate to (File > Preferences)
Find the section at the bottom called Additional Boards Manager URLs: 
2. Add the URL below to the list and click ok to close the preferences.
```
https://grumpyoldpizza.github.io/ArduinoCore-stm32l0/package_stm32l0_boards_index.json
```
![arduino_preferences](https://i.gyazo.com/148c4bc3646aaf71f8d9a0499c82fec4.png)

3. Open Boards Manager: Select Tools > Board: > Boards Manager...
4. Search for "Tlera Corp STM32L0 Boards"
5. Select the newest version and install.

### Temporary Manual Fix
This issue has been fixed but not released yet, until then you will need to insert three lines of code in a library file.
Insert the following:
```
#ifndef NULL
#define NULL  0
#endif
```
Into the file found here:
linux: /home/{user}/Arduino/libraries/IBM_LMIC_framework/src/lmic  
windows: Documents/Arduino/libraries/IBM_LMIC_framework/src/lmic  
mac os: Documents/Arduino/libraries/IBM_LMIC_framework/src/lmic  

![arduino_board_support](https://i.gyazo.com/216457ad64b8f85016d1b6d7cc6df044.png)
## Programming (Uploading):

Arduino IDE:   
1. Select Board: Tools > Board: > B-L072Z-LRWAN1  
2. Select Port: Tools > Port > COM# or /dev/ttyACM#(B-L072Z-LRWAN1)
3. Upload Sketch: Select > Upload
4. (Optional) View Serial Debug Output: Tools > Serial Monitor > 9600 baud  