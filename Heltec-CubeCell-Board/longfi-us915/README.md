# Heltec CubeCell Board 

This example demonstrates sending a simple data packet using a Heltec CubeCell Board . Please follow every instruction step below from top to bottom.

[Quickstart Guide](https://developer.helium.com/devices/arduino-quickstart/heltec-cubecell-board)
[Heltec CubeCell Board Product Page](https://heltec.org/project/htcc-ab01/)  
[Heltec CubeCell Board Docs](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/quick_start.html#)

## Install Serial Driver
Find Directions [here](https://heltec-automation-docs.readthedocs.io/en/latest/general/establish_serial_connection.html).

## Install CubeCell Board Support
Find Directions [here](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/quick_start.html#install-cubecell-relevant-framework)

## Manual updates to the Heltec runtime libraries
#### Verify Data Rate

Some versions of Heltec's runtime libraries have set a default configuration variable to a value that is incompatible with the Helium network, especially when the Heltec device is configured for the North American market. Before attempting to use the libraries it is best to verify the value of the variable.

The top level location of the file of interest depends on the library installation directory of the IDE you are using, Arduino IDE vs Platformio IDE, as well as the host platform, Windows vs Linux vs Mac. With the Arduino IDE location, the library version number may also be different depending on when you download the package.

For example on a Linux platform the files should be located at:

- Arduino IDE library version 1.0.0:   ~/.arduino15/packages/CubeCell/hardware/CubeCell/1.0.0/libraries/LoRa/src/LoRaWan_APP.cpp

- Platformio IDE:  
~/.platformio/packages/framework-arduinoasrmicro650x/libraries/LoRa/src/LoRaWan_APP.cpp

In LoRaWan_APP.cpp look for #define LORAWAN_DEFAULT_DATARATE
Depending on the version of the Heltec runtime that is installed this default may be set to DR_0, DR_3, DR_5 or some other value.
Note: DR_5 is not valid for US915, the North American market.

The LORAWAN_DEFAULT_DATARATE setting is tied directly to the maximum size of the data packet you are transferring. While other runtime versions may allow programatic overide of this default, the Heltec implementation does not currently support overriding.


NOTE: If you try to transfer a packet that is larger than this setting allows, your device may well successfully join the network but the data transmit will fail silently. Hopefully a future release will give the application layer failure notification.

|Data Rate (DR)  |    Max Application Payload|
-----------------|--------------------------|
|DR_0            |     11 bytes             |
|DR_1            |     53 bytes             |
|DR_2            |     125 bytes            |
|DR_3            |     242 bytes            |
|DR_4            |     242 bytes            |
|DR_5 - 7        |     Not Valid            |

Update the LORAWAN_DEFAULT_DATARATE as appropriate for your application needs.

The above values are valid for the US902-928MHz region(North America), the values may differ for other LoRa regions. This link will take you to document which may help determine the correct value for other LoRa regions.
https://lora-alliance.org/resource-hub/rp2-101-lorawanr-regional-parameters

#### LoRaWAN preamble size
There are some versions of the Heltec runtime libraries that may set a LoRaWAN preamble size that is incompatible with the current LoRaWan specification. If the preamble size is not set correctly your device cannot join the network.

This should be verified in the following file corresponding to your target LoRaWan region. For region US915 this file is RegionUS915.c:

- For Arduino IDE library version 1.0.0 the file is located:    ~/.arduino15/packages/CubeCell/hardware/CubeCell/1.0.0/cores/asr650x/loramac/mac/region/RegionUS915.c

- Platformio IDE:  
~/.platformio/packages/framework-arduinoasrmicro650x/cores/asr650x/loramac/mac/region/RegionUS915.c

In this file the line: ( your version could have 14 or 16 for the 7th parameter, it needs to be 8)

Change:
```
Radio.SetTxConfig( MODsEM_LORA, phyTxPower, 0, bandwidth, phyDr, 1, 14, false, true, 0, 0, false, 3e3 );
```
To:
```
Radio.SetTxConfig( MODEM_LORA, phyTxPower, 0, bandwidth, phyDr, 1, 8, false, true, 0, 0, false, 3e3 );
```

Heltec support has been notified of these issues, hopefully a future release of those libs will resolve the issues.

## Select Board
Arduino IDE:  
1. Select Tools -> Board: -> CubeCell-Board

## Select Board options
Arduino IDE:  
Until you are familiar with their configuration behavior it is recommended you set the board options as follows:
1. Select Tools -> LORAWAN_REGION:   -> REGION_US915
2. Select Tools -> LORAWAN_CLASS:    -> CLASS_A
3. Select Tools -> LORAWAN_NETMODE:  -> OTTA
4. Select Tools -> LORAWAN_ADR:      -> OFF
5. Select Tools -> LORAWAN_UPLINKMODE:      -> UNCONFIRMED
6. Select Tools -> LORAWAN_Net_Reservation: -> OFF
7. Select Tools -> LORAWAN_AT_SUPPORT:      -> OFF
8. Select Tools -> LORAWAN_AT_RGB :         -> ACTIVE
9. Select Tools -> LoRaWan_ Debug Level :   -> FREQ&&DIO      for most verbose messages

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
