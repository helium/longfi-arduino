#include "LoRaWAN.h"
#include "TimerMillis.h"
#include <CayenneLPP.h>
#include <MicroNMEA.h>

const char *devEui = "FILL_ME_IN";
const char *appEui = "FILL_ME_IN";
const char *appKey = "FILL_ME_IN";

#define RESET_PIN 7

const uint32_t TX_INTERVAL = 60000; // 60 Seconds
TimerMillis timer_send;

float longitude_mdeg;
float latitude_mdeg;
long alt;

// Refer to serial devices by use
HardwareSerial &console = Serial;
HardwareSerial &gps = Serial1;

CayenneLPP lpp(51);
static volatile bool uplink_attempted;

// MicroNMEA library structures
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

bool ledState = LOW;
volatile bool ppsTriggered = false;

void ppsHandler(void);

void ppsHandler(void) { ppsTriggered = true; }

void gpsHardwareReset() {
  // Empty input buffer
  while (gps.available())
    gps.read();

  // reset the device
  digitalWrite(RESET_PIN, LOW);
  delay(50);
  digitalWrite(RESET_PIN, HIGH);

  // wait for reset to apply
  delay(2000);
}

void setupGPS() {
  delay(3000);
  console.begin(115200); // console
  Serial.println("Starting GPS Example...");

  gps.begin(9600); // gps

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);

  // Start the module
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  Serial.println();
  Serial.println("Resetting GPS module ...");
  gpsHardwareReset();
  Serial.println("done.");

  // Change the echoing messages to the ones recognized by the MicroNMEA library
  MicroNMEA::sendSentence(gps, "$PSTMSETPAR,1201,0x00000042");
  MicroNMEA::sendSentence(gps, "$PSTMSAVEPAR");

  // Reset the device so that the changes could take plaace
  MicroNMEA::sendSentence(gps, "$PSTMSRR");

  delay(4000);

  // clear serial buffer
  while (gps.available())
    gps.read();

  pinMode(6, INPUT);
  attachInterrupt(digitalPinToInterrupt(6), ppsHandler, RISING);
}

void readGPS() {
  // If a message is received
  if (ppsTriggered) {
    ppsTriggered = false;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);

    // Clear Payload
    lpp.reset();

    latitude_mdeg = nmea.getLatitude();
    longitude_mdeg = nmea.getLongitude();
    nmea.getAltitude(alt);

    lpp.addGPS(1, latitude_mdeg / 1000000, longitude_mdeg / 1000000,
               alt / 1000);

    nmea.clear();
  }

  // While the message isn't complete
  while (!ppsTriggered && gps.available()) {
    // Fetch the character one by one
    char c = gps.read();
    // Serial.print(c);
    // Pass the character to the library
    nmea.process(c);
  }
}

void async_timer_send() {
  if (LoRaWAN.joined() && !LoRaWAN.busy()) {
    // Send Packet
    LoRaWAN.sendPacket(1, lpp.getBuffer(), lpp.getSize());
    uplink_attempted = true;
  }
}

void setup(void) {
  setupGPS();

  Serial.begin(9600);

  while (!Serial) {
  }

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
    Serial.print("Latitude (deg): ");
    Serial.print(latitude_mdeg / 1000000., 6);
    Serial.print(" Longitude (deg): ");
    Serial.print(longitude_mdeg / 1000000., 6);
    Serial.print("  Altitude (m): ");
    if (nmea.getAltitude(alt))
      Serial.println(alt / 1000., 3);
    else
      Serial.println("not available");

    uplink_attempted = false;
  }

  readGPS();
}