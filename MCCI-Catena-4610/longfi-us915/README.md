# MCCI Catena 4610

## Installing the MCCI LMIC Library

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type MCCI and select the MCCI LoRaWAN LMIC library should be the first result.

![](https://developer.helium.com/static/library_manager-56bed2bb23b6f93e5cc3b25bdfd345a2.png)

## Board Support

### Catena 4610 - MCCI  
Install board support package, find instructions [here](https://github.com/mcci-catena/Arduino_Core_STM32#getting-started). 

Arduino IDE:  
Select Tools -> Board: -> MCCI Catena 4610.

For the GPS sketch:
Select Tools -> Serial Interface -> USB + HW Serial  

[Catena 4610 Product Page](https://store.mcci.com/collections/iot-building-blocks/products/mcci-catena-4610-integrated-node-for-lorawan-technology)  
[Catena 4610 Pin Mapping Diagram](https://github.com/mcci-catena/HW-Designs/blob/master/Boards/Catena-4610/Catena-4610-Pinmapping.png)

## Programming (Uploading Method):

### Catena 4610 - MCCI 

#### DFU
Arduino IDE:  
Select Tools -> Upload Method -> DFU

To put the board in DFU mode, hold down the boot button while pressing and releasing the reset button.

#### ST-Link
Arduino IDE:  
Select Tools -> Upload Method -> STLink

Requires an ST-Link debugger connected to SWCLK, SWDIO, Vref, and GND, refer to pin mapping diagram. 