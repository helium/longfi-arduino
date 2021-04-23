# Heltec CubeCell Board 

This example demonstrates sending a simple data packet using a Heltec CubeCell Board . Please follow every instruction step below from top to bottom.

[Quickstart Guide](https://developer.helium.com/devices/arduino-quickstart/heltec-cubecell-board)
[Heltec CubeCell Board Product Page](https://heltec.org/project/htcc-ab01/)  
[Heltec CubeCell Board Docs](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/quick_start.html#)

## Install Serial Driver
Find Directions [here](https://heltec-automation-docs.readthedocs.io/en/latest/general/establish_serial_connection.html).

## Install CubeCell Board Support
Find Directions [here](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/quick_start.html#install-cubecell-relevant-framework)

## Select Board
Arduino IDE:  
1. Select Tools -> Board: -> CubeCell-Board

## Select Region
Arduino IDE:  
1. Select Tools -> LoRaWAN Region: -> REGION_US915

## Upload `LoRaWAN` example
Arduino IDE:  
1. Select File -> Open -> longfi-arduino/Heltec-CubeCell-Boardc/longfi-us915.ino
2. Select Tools -> Port: "COM# or ttyACM#" 
3. Enter DevEUI(msb), AppEUI(msb), and AppKey(msb) from Helium Console, at lines 14, 15, 16. 
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

## Sample Decoder
* In the console-decoders directory you will find a simple sample decoder that goes along with this sample code. It is intended to be used as a Helium Console "Function". This will allow one to view the decoded payload within the Helium Console device debug view.
