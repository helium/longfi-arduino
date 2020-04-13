# LongFi Arduino  

## Installing the MCCI LMIC Library

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type MCCI and select the MCCI LoRaWAN LMIC library should be the first result.

![](https://developer.helium.com/static/library_manager-56bed2bb23b6f93e5cc3b25bdfd345a2.png)

## Board Support

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit  
Install board support package, find instructions [here](https://github.com/stm32duino/Arduino_Core_STM32#getting-started).  

Arduino IDE:  
1. Select Tools -> Board: -> Discovery 
2. Select Tools -> Board part number: -> Discovery L072Z-LRWAN1 

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

## Programming (Uploading Method):

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit 

#### STM32CubeProgrammer(SWD)
Will use onboard ST-Link(Flasher/Debugger) to upload sketch.  
Download and Install required utility from ST [here](https://www.st.com/en/development-tools/stm32cubeprog.html).  

Arduino IDE:  
Select Tools -> Upload Method -> STM32CubeProgrammer(SWD)
