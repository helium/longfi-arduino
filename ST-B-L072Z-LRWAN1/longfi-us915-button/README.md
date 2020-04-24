# ST B-L072Z-LRWAN1 - Button to AWS Example

This is an example code for building an AWS IoT application that sends you text messages using Twilio.

You'll need:
* ST-B-L072Z-LRWAN1
* An AWS account
* A Twilio account

You need to load this sketch onto the ST-B-L072Z-LRWAN1. The sketch will send a heartbeat every 15 seconds and will asyncronously send an alert whenever the button is pressed.

Once you confirm that the device is connecting and sending data via Helium Console, you'll want to setup an integration to AWS IoT.

Once you do that, you want to create a Lambda function using the Python file here: lambda_button_text.py

You'll need to set environmental variables to make it work:
* TWILIO_AUTH_TOKEN: get this from your Twilio dashboard
* TWILIO_ACCOUNT_SID: get this from your Twilio dashboard	
* TWILIO_ASSIGNED_NUMBER: get this from your Twilio dashboard	
* USER_NUMBER‬: this is the cell phone number which will get text message alerts when the button is pressed

Once you save the lambda function, you will simply need to connect the lambda function to an AWS IoT trigger. Use the rule query statement: SELECT * FROM "helium"

Now you're done! Press the button and you should get a text message.

## Required Arduino Libraries

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type the library name below and install the latest version.

[MCCI Arduino LoRaWAN Library](https://github.com/mcci-catena/arduino-lmic)  

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

## Programming (Uploading Method):

#### STM32CubeProgrammer(SWD)
Will use onboard ST-Link(Flasher/Debugger) to upload sketch.  
Download and Install required utility from ST [here](https://www.st.com/en/development-tools/stm32cubeprog.html).  

Arduino IDE:  
Select Tools -> Upload Method -> STM32CubeProgrammer(SWD)

### PlatformIO Support 

The PlatformIO Board file for this board is currently using the incorrect OpenOCD (Upload/Debug)
script for the microcontroller on this board. We are in the process of pushing a fix upstream. When 
uploading or debugging, hold the reset button down right until the upload or debug process initiates 
it's routine in communicating with the board, this seems to aleviate the issue for right now.

`platformio.ini`
```
[env:disco_l072cz_lrwan1]
platform = ststm32
board = disco_l072cz_lrwan1
framework = arduino

lib_deps =
     MCCI LoRaWAN LMIC library
     CayenneLPP
``` 
