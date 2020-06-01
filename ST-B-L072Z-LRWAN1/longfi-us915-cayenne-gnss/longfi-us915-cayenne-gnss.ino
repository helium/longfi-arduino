#include "LoRaWAN.h"
#include "TimerMillis.h"
#include <CayenneLPP.h>
#include <MicroNMEA.h>

const char *devEui = "FILL_ME_IN";
const char *appEui = "FILL_ME_IN";
const char *appKey = "FILL_ME_IN";

const uint32_t TX_INTERVAL = 10000;
TimerMillis timer_send;

// Sensors
float longitude_mdeg;
float latitude_mdeg;
long alt;

#define RESET_PIN 7

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
  // If a message is received, print all the info
  if (ppsTriggered) {
    ppsTriggered = false;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);

    // Clear Payload
    lpp.reset();

    // Output GPS information from previous second
    Serial.print("Valid fix: ");
    Serial.println(nmea.isValid() ? "yes" : "no");

    Serial.print("Nav. system: ");
    if (nmea.getNavSystem())
      Serial.println(nmea.getNavSystem());
    else
      Serial.println("none");

    Serial.print("Num. satellites: ");
    Serial.println(nmea.getNumSatellites());

    Serial.print("HDOP: ");
    Serial.println(nmea.getHDOP() / 10., 1);

    Serial.print("Date/time: ");
    Serial.print(nmea.getYear());
    Serial.print('-');
    Serial.print(int(nmea.getMonth()));
    Serial.print('-');
    Serial.print(int(nmea.getDay()));
    Serial.print('T');
    Serial.print(int(nmea.getHour()));
    Serial.print(':');
    Serial.print(int(nmea.getMinute()));
    Serial.print(':');
    Serial.println(int(nmea.getSecond()));

    latitude_mdeg = nmea.getLatitude();
    longitude_mdeg = nmea.getLongitude();

    Serial.print("Latitude (deg): ");
    Serial.println(latitude_mdeg / 1000000., 6);

    Serial.print("Longitude (deg): ");
    Serial.println(longitude_mdeg / 1000000., 6);

    // long alt;
    Serial.print("Altitude (m): ");
    if (nmea.getAltitude(alt))
      Serial.println(alt / 1000., 3);
    else
      Serial.println("not available");

    lpp.addGPS(1, latitude_mdeg / 1000000, longitude_mdeg / 1000000,
               alt / 1000);

    Serial.print("Speed: ");
    Serial.println(nmea.getSpeed() / 1000., 3);
    Serial.print("Course: ");
    Serial.println(nmea.getCourse() / 1000., 3);

    Serial.println("-----------------------");
    nmea.clear();
  }

  // While the message isn't complete
  while (!ppsTriggered && gps.available()) {
    // Fetch the character one by one
    char c = gps.read();
    Serial.print(c);
    // Pass the character to the library
    nmea.process(c);
  }
}

void async_timer_send() {
  if (LoRaWAN.joined() && !LoRaWAN.busy()) {
    Serial.println("Timer Send");
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

  readGPS();
}