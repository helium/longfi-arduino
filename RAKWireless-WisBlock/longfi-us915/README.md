
# Helium Basic Example

Here you will find a very basic example of a WisBlock/Helium embedded project that uses the Arduino IDE.
This version will join the Helium network and periodically send a "Hello" to the Helium network.

### Hardware
The only hardware required is:
* the [WisBlock Starter Kit](https://store.rakwireless.com/products/wisblock-starter-kit) containing  the base board with the core module installed.
* one USB 2.0 A-Male to Micro B interface cable.

#### Antenna Type/location
The WisBlock starter kit comes with two antenna types, 
* the one that resembles an "I" is the LoRa antenna, this one connects to the connector on the core moduke marked LoRa, which is below the large K in the RAK logo.
* the one that resembles a "T" is the BLE antenna, this one connects to the connector on the core module marked BLE

### Sample Decoder
* In the console-decoders directory you will find a simple sample decoder that goes along with this sample code. It is intended to be used as a Helium Console "Function". This will allow one to view the decoded payload within the Helium Console device debug view.
