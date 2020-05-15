#include "LoRaWAN.h"
#include "TimerMillis.h"
#include <CayenneLPP.h>

const char *devEui = "FILL_ME_IN";
const char *appEui = "FILL_ME_IN";
const char *appKey = "FILL_ME_IN";

const uint32_t TX_INTERVAL = 15000;
TimerMillis timer_send;

// Max Payload 11 Bytes for DR 0
uint8_t payload[] = {0};

static volatile bool button_pushed;
static volatile bool uplink_attempted;

void push_button_ISR() { button_pushed = true; }

void async_timer_send() {
  if (LoRaWAN.joined() && !LoRaWAN.busy()) {
    Serial.println("Timer Send");
    payload[0] = 0;
    // Send Packet
    LoRaWAN.sendPacket(1, payload, sizeof(payload));
    uplink_attempted = true;
  }
}

bool async_button_send() {
  if (LoRaWAN.joined() && !LoRaWAN.busy()) {
    Serial.println("Async Button Send");
    payload[0] = 1;
    // Send Packet
    LoRaWAN.sendPacket(1, payload, sizeof(payload));
    uplink_attempted = true;
    return false;
  } else {
    return true;
  }
}

void setup(void) {
  Serial.begin(9600);

  while (!Serial) {
  }

  // Configure User Button
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), push_button_ISR, FALLING);

  // US Region
  LoRaWAN.begin(US915);
  // Helium SubBand
  LoRaWAN.setSubBand(2);
  // Disable Adaptive Data Rate
  LoRaWAN.setADR(false);
  // Device IDs and Key
  LoRaWAN.joinOTAA(appEui, appKey, devEui);

  Serial.println("JOIN( )");

  while (!LoRaWAN.joined() && LoRaWAN.busy()) {
    Serial.println("JOINING( )");
    delay(5000);
  }
  Serial.println("JOINED( )");

  // Start Continuous Uplink Timer
  timer_send.start(async_timer_send, 0, TX_INTERVAL);
}

void loop(void) {
  if (uplink_attempted) {
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

    uplink_attempted = false;
  }

  if (button_pushed) {
    button_pushed = async_button_send();
  }

  delay(500);
}
