# Helium Basic Example

Here you will find a very basic example of a WisBlock/Helium embedded project that uses the Arduino IDE.
This version will join the Helium network and periodically send a "Hello" to the Helium network.

## Support Library Version Incompatibility
Recent API changes to the runtime support library, SX126x-Arduino, have resulted in library version incompatibility between version 1.3x and version 2.x.

| :warning: WARNING: This will require changes to the Arduino IDE installation, in particular the installed board support package files. The complete details of the changes required can be found [here](https://github.com/beegee-tokyo/SX126x-Arduino/blob/master/README_V2.md).|
|:---------------------------|

In addition to the IDE support file changes there are two major changes at the device application level if you are moving from version 1 to version 2 of the support library:
* The lmh_init() API requires 2 more parameters
* the device application is no longer required to call Radio.IrqProcess() within it's main processing loop. The processing will now be handled by the runtime layer.

The sample .ino file has been updated to be compatible with version 2 of the support library.


### Hardware
The only hardware required is:
* the [WisBlock Starter Kit](https://store.rakwireless.com/products/wisblock-starter-kit) containing  the base board with the core module installed.
* one USB 2.0 A-Male to Micro B interface cable.

#### Antenna Type/location
The WisBlock starter kit comes with two antenna types, 
* the one that resembles an "I" is the LoRa antenna, this one connects to the connector on the core module marked LoRa, which is below the large K in the RAK logo.
* the one that resembles a "T" is the BLE antenna, this one connects to the connector on the core module marked BLE

### Sample Decoder
* In the console-decoders directory you will find a simple sample decoder that goes along with this sample code. It is intended to be used as a Helium Console "Function". This will allow one to view the decoded payload within the Helium Console device debug view.
