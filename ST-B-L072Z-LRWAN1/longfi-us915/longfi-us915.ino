#include "LoRaWAN.h"

const char *devEui = "FILL_ME_IN";
const char *appEui = "FILL_ME_IN";
const char *appKey = "FILL_ME_IN";

// Max Payload 53 Bytes for DR 1
const uint8_t payload[] = "Hello, World!";

void setup( void )
{
    Serial.begin(9600);
    
    while (!Serial) { }

    // US Region
    LoRaWAN.begin(US915);
    // Helium SubBand
    LoRaWAN.setSubBand(2);
    // Disable Adaptive Data Rate
    LoRaWAN.setADR(false);
    // Set Data Rate 1 - Max Payload 53 Bytes
    LoRaWAN.setDataRate(1);
    // Device IDs and Key
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("JOIN( )");
}

void loop( void )
{
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
        Serial.print("TRANSMIT( ");
        Serial.print("TimeOnAir: ");
        Serial.print(LoRaWAN.getTimeOnAir());
        Serial.print(", NextTxTime: ");
        Serial.print(LoRaWAN.getNextTxTime());
        Serial.print(", MaxPayloadSize: ");
        Serial.print(LoRaWAN.getMaxPayloadSize());
        Serial.print(", DR: ");
        Serial.print(LoRaWAN.getDataRate());
        Serial.print(", TxPower: ");
        Serial.print(LoRaWAN.getTxPower(), 1);
        Serial.print("dbm, UpLinkCounter: ");
        Serial.print(LoRaWAN.getUpLinkCounter());
        Serial.print(", DownLinkCounter: ");
        Serial.print(LoRaWAN.getDownLinkCounter());
        Serial.println(" )");

        // Send Packet
        LoRaWAN.sendPacket(1, payload, sizeof(payload));
    }

    delay(20000);
}