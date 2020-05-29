# ST B-L072Z-LRWAN1 - Basic LongFi Example

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

## Required Driver (Windows Only)
Download driver [here](https://www.st.com/en/development-tools/stsw-link009.html).

## Required Board Support & Library

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

![arduino_board_support](https://i.gyazo.com/216457ad64b8f85016d1b6d7cc6df044.png)
## Programming (Uploading):

Arduino IDE:   
1. Select Board: Tools > Board: > B-L072Z-LRWAN1  
2. Select Port: Tools > Port > COM# or /dev/ttyACM#(B-L072Z-LRWAN1)
3. Upload Sketch: Select > Upload
4. (Optional) View Serial Debug Output: Tools > Serial Monitor > 9600 baud
