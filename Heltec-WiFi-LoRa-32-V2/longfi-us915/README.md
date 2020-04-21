# Heltec WiFi LoRa 32 V2 

This example demonstrates sending a simple data packet using a Heltec Wifi LoRa 32 V2 development board. Please follow every instruction step below from top to bottom.

[Heltec LoRa 32 V2 Product Page](https://heltec.org/project/wifi-lora-32/)  
[Heltec LoRa 32 V2 Docs](https://heltec-automation-docs.readthedocs.io/en/latest/esp32/index.html)

## Installing the Heltec ESP32 Library

From the Arduino IDE, open the Library Manager (Sketch->Include Library->Manage Libraries). In the search box, type Heltec ESP32 and look for `Heltec ESP32 Dev-Boards`. Select the latest version and install.

## Install Serial Driver
Find Directions [here](https://heltec-automation-docs.readthedocs.io/en/latest/general/establish_serial_connection.html).

## Install Arduino-ESP32 Board Support
Find Directions [here](https://heltec-automation-docs.readthedocs.io/en/latest/esp32+arduino/quick_start.html#via-arduino-board-manager)

## Select Board
Arduino IDE:  
1. Select Tools -> Board: -> WiFi LoRa 32(V2)

## Select Region
Arduino IDE:  
1. Select Tools -> LoRaWAN Region: -> REGION_US915

## Obtain Heltec License Key

### Upload `GetChipID` example
Arduino IDE:  
1. Select File -> Examples -> ESP32 -> ChipID -> GetChipID  
2. Select Tools -> Port: "COM# or ttyACM#"  
3. Select Sketch -> Upload 
4. Wait for Done uploading message
5. Select Tools -> Serial Monitor
Serial Monitor Window
1. Select 115200 baud from bottom right dropdown.
2. You should see something that looks like this every second `ESP32 Chip ID = ############`  
3. Save this Chip ID

### Obtain License Key with Chip ID
1.Go to [resource.heltec.cn/search](http://resource.heltec.cn/search)
2. Enter ChipID
3. Save license field, will look like `0x########,0x#########,0x########,0x########`	

## Upload `OTAA_OLED` example
Arduino IDE:  
1. Select File -> Examples -> ESP32_LoRaWAN -> OTAA_LED
2. Enter License Key at line 32 
```
uint32_t  license[4] = {`0x########,0x#########,0x########,0x########};
```
3. Enter DevEUI(msb), AppEUI(msb), and AppKey(msb) from Helium Console, at lines 34,35,36.
```
uint8_t DevEui[] = { FILL_ME_IN };
uint8_t AppEui[] = { FILL_ME_IN };
uint8_t AppKey[] = { FILL_ME_IN };
```
4. Select Sketch -> Upload.
5. Wait for Done uploading message
6. Select Tools -> Serial Monitor
Serial Monitor Window
1. Select 115200 baud from bottom right dropdown.
2. Wait for device to successfully join, may take 1-3 min, and show several failures. Do not be alarmed by the failures, it is expected.