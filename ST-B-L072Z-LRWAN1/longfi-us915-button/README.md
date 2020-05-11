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

## Required Hardware

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

## Required Board Support & Library

### Arduino Core for STM32L0 
Arduino IDE:  
1. Navigate to (File > Preferences)
Find the section at the bottom called Additional Boards Manager URLs: 
2. Add the URL below to the list and click ok to close the preferences.
```
https://grumpyoldpizza.github.io/ArduinoCore-stm32l0/package_stm32l0_boards_index.json
```
![arduino_preferences](https://i.gyazo.com/558b58a463578b28e17ffb763a592c69.png)

3. Open Boards Manager: Select Tools > Board: > Boards Manager...
4. Search for "Tlera Corp STM32L0 Boards"
5. Select the newest version and install.

![arduino_board_support](https://i.gyazo.com/216457ad64b8f85016d1b6d7cc6df044.png)
## Programming (Uploading):

Arduino IDE:   
1. Select Board: Tools > Board: > B-L072Z-LRWAN1  
2. Select Port: Tools > Port > COM# or /dev/ttyACM#(B-L072Z-LRWAN1)
3. Upload Sketch: Select > Upload
4. (Optional) View Serial Debug Output: Tools > Serial Monitor > 9600 baud