#include <MicroNMEA.h>
#include <SPI.h>
#include <hal/hal.h>
#include <lmic.h>

#include <CayenneLPP.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

// Sensors
long longitude_mdeg;
long latitude_mdeg;
long alt;

// Define Serial1 for STM32 Nucleo boards
#ifdef ARDUINO_ARCH_STM32
HardwareSerial Serial1(PA10, PA9);
#endif

#define RESET_PIN 7

// Refer to serial devices by use
HardwareSerial &console = Serial;
HardwareSerial &gps = Serial1;

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

//
// For normal use, we require that you edit the sketch to replace FILLMEIN
// with values assigned by the TTN console. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN to a non-
// working but innocuous value.
//
#ifdef COMPILE_REGRESSION_TEST
#define FILLMEIN 0
#else
#warning                                                                       \
    "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
#define FILLMEIN (#dont edit this, edit the lines that use FILLMEIN)
#endif

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8] = {FILL_ME_IN};
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
//// This is the "App EUI" in Helium. Make sure it is little-endian (lsb).
// static const u1_t PROGMEM APPEUI[8] = {FILL_ME_IN};
// void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8] = {FILL_ME_IN};
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }

//// This should also be in little endian format
//// These are user configurable values and Helium console permits anything
// static const u1_t PROGMEM DEVEUI[8] = {FILL_ME_IN};
// void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = {FILL_ME_IN};
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

//// This is the "App Key" in Helium. It is big-endian (msb).
// static const u1_t PROGMEM APPKEY[16] = {FILL_ME_IN};
// void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

CayenneLPP lpp(51);
static osjob_t sendjob;
void do_send(osjob_t *j);

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 10;

// Pin mapping
//
// Adafruit BSPs are not consistent -- m0 express defs ARDUINO_SAMD_FEATHER_M0,
// m0 defs ADAFRUIT_FEATHER_M0
//
#if defined(ARDUINO_SAMD_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0)
// Pin mapping for Adafruit Feather M0 LoRa, etc.
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = {3, 6, LMIC_UNUSED_PIN},
    .rxtx_rx_active = 0,
    .rssi_cal = 8, // LBT cal for the Adafruit Feather M0 LoRa, in dB
    .spi_freq = 8000000,
};
#elif defined(ARDUINO_AVR_FEATHER32U4)
// Pin mapping for Adafruit Feather 32u4 LoRa, etc.
// Just like Feather M0 LoRa, but uses SPI at 1MHz; and that's only
// because MCCI doesn't have a test board; probably higher frequencies
// will work.
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = {7, 6, LMIC_UNUSED_PIN},
    .rxtx_rx_active = 0,
    .rssi_cal = 8, // LBT cal for the Adafruit Feather 32U4 LoRa, in dB
    .spi_freq = 1000000,
};
#elif defined(ARDUINO_CATENA_4551)
// Pin mapping for Murata module / Catena 4551
const lmic_pinmap lmic_pins = {
    .nss = 7,
    .rxtx = 29,
    .rst = 8,
    .dio =
        {
            25, // DIO0 (IRQ) is D25
            26, // DIO1 is D26
            27, // DIO2 is D27
        },
    .rxtx_rx_active = 1,
    .rssi_cal = 10,
    .spi_freq = 8000000 // 8MHz
};
#elif defined(MCCI_CATENA_4610)
#include "arduino_lmic_hal_boards.h"
const lmic_pinmap lmic_pins = *Arduino_LMIC::GetPinmap_Catena4610();
#elif defined(ARDUINO_DISCO_L072CZ_LRWAN1)
#include "arduino_lmic_hal_boards.h"
// Pin mapping Discovery
const lmic_pinmap lmic_pins = *Arduino_LMIC::GetPinmap_Disco_L072cz_Lrwan1();
#else
#error "Unknown target"
#endif

void printHex2(unsigned v) {
  v &= 0xff;
  if (v < 16)
    Serial.print('0');
  Serial.print(v, HEX);
}

void onEvent(ev_t ev) {
  DEBUG_PRINT(os_getTime());
  DEBUG_PRINT(": ");
  switch (ev) {
  case EV_SCAN_TIMEOUT:
    DEBUG_PRINTLN(F("EV_SCAN_TIMEOUT"));
    break;
  case EV_BEACON_FOUND:
    DEBUG_PRINTLN(F("EV_BEACON_FOUND"));
    break;
  case EV_BEACON_MISSED:
    DEBUG_PRINTLN(F("EV_BEACON_MISSED"));
    break;
  case EV_BEACON_TRACKED:
    DEBUG_PRINTLN(F("EV_BEACON_TRACKED"));
    break;
  case EV_JOINING:
    DEBUG_PRINTLN(F("EV_JOINING"));
    break;
  case EV_JOINED:
    Serial.println(F("EV_JOINED"));
    {
      u4_t netid = 0;
      devaddr_t devaddr = 0;
      u1_t nwkKey[16];
      u1_t artKey[16];
      LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
      Serial.print("netid: ");
      Serial.println(netid, DEC);
      Serial.print("devaddr: ");
      Serial.println(devaddr, HEX);
      Serial.print("AppSKey: ");
      for (size_t i = 0; i < sizeof(artKey); ++i) {
        if (i != 0)
          Serial.print("-");
        printHex2(artKey[i]);
      }
      Serial.println("");
      Serial.print("NwkSKey: ");
      for (size_t i = 0; i < sizeof(nwkKey); ++i) {
        if (i != 0)
          Serial.print("-");
        printHex2(nwkKey[i]);
      }
      Serial.println();
    }
    // Disable link check validation (automatically enabled
    // during join, but because slow data rates change max TX
    // size, we don't use it in this example.
    LMIC_setLinkCheckMode(0);
    break;
  /*
    || This event is defined but not used in the code. No
    || point in wasting codespace on it.
    ||
    || case EV_RFU1:
    ||     DEBUG_PRINTLN(F("EV_RFU1"));
    ||     break;
  */
  case EV_JOIN_FAILED:
    Serial.println(F("EV_JOIN_FAILED"));
    break;
  case EV_REJOIN_FAILED:
    Serial.println(F("EV_REJOIN_FAILED"));
    break;
    break;
  case EV_TXCOMPLETE:
    Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
    if (LMIC.txrxFlags & TXRX_ACK)
      Serial.println(F("Received ack"));
    if (LMIC.dataLen) {
      Serial.println(F("Received "));
      Serial.println(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
    }
    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL),
                        do_send);
    break;
  case EV_LOST_TSYNC:
    Serial.println(F("EV_LOST_TSYNC"));
    break;
  case EV_RESET:
    Serial.println(F("EV_RESET"));
    break;
  case EV_RXCOMPLETE:
    // data received in ping slot
    Serial.println(F("EV_RXCOMPLETE"));
    break;
  case EV_LINK_DEAD:
    Serial.println(F("EV_LINK_DEAD"));
    break;
  case EV_LINK_ALIVE:
    Serial.println(F("EV_LINK_ALIVE"));
    break;
  /*
    || This event is defined but not used in the code. No
    || point in wasting codespace on it.
    ||
    || case EV_SCAN_FOUND:
    ||    DEBUG_PRINTLN(F("EV_SCAN_FOUND"));
    ||    break;
  */
  case EV_TXSTART:
    Serial.println(F("EV_TXSTART"));
    break;
  case EV_TXCANCELED:
    Serial.println(F("EV_TXCANCELED"));
    break;
  case EV_RXSTART:
    /* do not print anything -- it wrecks timing */
    break;
  case EV_JOIN_TXCOMPLETE:
    Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
    break;

  default:
    Serial.print(F("Unknown event: "));
    Serial.println((unsigned)ev);
    break;
  }
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
    DEBUG_PRINT("Valid fix: ");
    DEBUG_PRINTLN(nmea.isValid() ? "yes" : "no");

    DEBUG_PRINT("Nav. system: ");
    if (nmea.getNavSystem())
      DEBUG_PRINTLN(nmea.getNavSystem());
    else
      DEBUG_PRINTLN("none");

    DEBUG_PRINT("Num. satellites: ");
    DEBUG_PRINTLN(nmea.getNumSatellites());

    DEBUG_PRINT("HDOP: ");
    DEBUG_PRINTLN(nmea.getHDOP() / 10.); //, 1);

    DEBUG_PRINT("Date/time: ");
    DEBUG_PRINT(nmea.getYear());
    DEBUG_PRINT('-');
    DEBUG_PRINT(int(nmea.getMonth()));
    DEBUG_PRINT('-');
    DEBUG_PRINT(int(nmea.getDay()));
    DEBUG_PRINT('T');
    DEBUG_PRINT(int(nmea.getHour()));
    DEBUG_PRINT(':');
    DEBUG_PRINT(int(nmea.getMinute()));
    DEBUG_PRINT(':');
    DEBUG_PRINTLN(int(nmea.getSecond()));

    latitude_mdeg = nmea.getLatitude();
    longitude_mdeg = nmea.getLongitude();

    DEBUG_PRINT("Latitude (deg): ");
    DEBUG_PRINTLN(latitude_mdeg / 1000000.); //, 6);

    DEBUG_PRINT("Longitude (deg): ");
    DEBUG_PRINTLN(longitude_mdeg / 1000000.); //, 6);

    // long alt;
    DEBUG_PRINT("Altitude (m): ");
    if (nmea.getAltitude(alt))
      DEBUG_PRINTLN(alt / 1000.); //, 3);
    else
      DEBUG_PRINTLN("not available");

    lpp.addGPS(1, latitude_mdeg / 1000000, longitude_mdeg / 1000000,
               alt / 1000);

    DEBUG_PRINT("Speed: ");
    DEBUG_PRINTLN(nmea.getSpeed() / 1000.); //, 3);
    DEBUG_PRINT("Course: ");
    DEBUG_PRINTLN(nmea.getCourse() / 1000.); //, 3);

    DEBUG_PRINTLN("-----------------------");
    nmea.clear();
  }

  // While the message isn't complete
  while (!ppsTriggered && gps.available()) {
    // Fetch the character one by one
    char c = gps.read();
    DEBUG_PRINT(c);
    // Pass the character to the library
    nmea.process(c);
  }
}

void do_send(osjob_t *j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    DEBUG_PRINTLN(F("OP_TXRXPEND, not sending"));
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, lpp.getBuffer(), lpp.getSize(), 0);
    DEBUG_PRINTLN(F("Packet queued"));
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void setup(void) {
  delay(3000);
  console.begin(115200); // console
  DEBUG_PRINTLN("Starting #IoTForGood GPS Example...");

  gps.begin(9600); // gps

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);

  // Start the module
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  DEBUG_PRINTLN("Resetting GPS module ...");
  gpsHardwareReset();
  DEBUG_PRINTLN("... done");

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

#if defined(ARDUINO_DISCO_L072CZ_LRWAN1)
  SPI.setMOSI(RADIO_MOSI_PORT);
  SPI.setMISO(RADIO_MISO_PORT);
  SPI.setSCLK(RADIO_SCLK_PORT);
  SPI.setSSEL(RADIO_NSS_PORT);
#endif

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  // allow much more clock error than the X/1000 default. See:
  // https://github.com/mcci-catena/arduino-lorawan/issues/74#issuecomment-462171974
  // https://github.com/mcci-catena/arduino-lmic/commit/42da75b56#diff-16d75524a9920f5d043fe731a27cf85aL633
  // the X/1000 means an error rate of 0.1%; the above issue discusses using
  // values up to 10%. so, values from 10 (10% error, the most lax) to 1000
  // (0.1% error, the most strict) can be used.
  LMIC_setClockError(1 * MAX_CLOCK_ERROR / 40);

  LMIC_setLinkCheckMode(0);
  LMIC_setDrTxpow(DR_SF7, 14);
  // Sub-band 2 - Helium Network
  LMIC_selectSubBand(1); // zero indexed

  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
}

void loop(void) {
  os_runloop_once();
  readGPS();
}