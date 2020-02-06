# LongFi Arduino  

## Installing the MCCI LMIC Library

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type MCCI and select the MCCI LoRaWAN LMIC library should be the first result.

![](https://cdn-learn.adafruit.com/assets/assets/000/062/526/medium800/feather_Library_Manager_MCCI.png?1537882799)

## Board Support

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit  
Install board support package, find instructions [here](https://github.com/stm32duino/Arduino_Core_STM32#getting-started).  

Arduino IDE:  
1. Select Tools -> Board: -> Discovery 
2. Select Tools -> Board part number: -> Discovery L072Z-LRWAN1 

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

### Catena 4610 - MCCI  
Install board support package, find instructions [here](https://github.com/mcci-catena/Arduino_Core_STM32#getting-started). 

Arduino IDE:  
Select Tools -> Board: -> MCCI Catena 4610.  

[Catena 4610 Product Page](https://store.mcci.com/collections/iot-building-blocks/products/mcci-catena-4610-integrated-node-for-lorawan-technology)  
[Catena 4610 Pin Mapping Diagram](https://github.com/mcci-catena/HW-Designs/blob/master/Boards/Catena-4610/Catena-4610-Pinmapping.png)

### Feather M0 RFM96 - Adafruit

1. Add Adafruit package index, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/setup).
2. Install both Arduino and Adafruit SAMD board support, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-samd-support-6-5).
3. (Windows Only) Install drivers, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-drivers-windows-7-and-8-only-6-11)

[Adafruit Feather M0 with RFM95 Product Page](https://www.adafruit.com/product/3178)  
[Adafruit Feather M0 with RFM95 Datasheets & Files](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/downloads)

## Programming(Upload Method):

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit 

#### STM32CubeProgrammer(SWD)
Will use onboard ST-Link(Flasher/Debugger) to upload sketch.  
Download and Install required utility from ST [here](https://www.st.com/en/development-tools/stm32cubeprog.html).  

Arduino IDE:  
Select Tools -> Upload Method -> STM32CubeProgrammer(SWD)


### Catena 4610 - MCCI 

#### DFU
Arduino IDE:  
Select Tools -> Upload Method -> DFU
Select Tools -> Serial Interface -> USB + HW Serial

To put the board in DFU mode, hold down the boot button while pressing and releasing the reset button.

#### ST-Link
Arduino IDE:  
Select Tools -> Upload Method -> STLink

Requires an ST-Link debugger connected to SWCLK, SWDIO, Vref, and GND, refer to pin mapping diagram. 
