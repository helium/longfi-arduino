/**
 ******************************************************************************
 * @file   X_NUCLEO_IKS01A3_LSM6DSO_DoubleTap.ino
 * @author  SRA
 * @version V1.0.0
 * @date    February 2019
 * @brief   Arduino test application for the STMicrolectronics
 *          MEMS Inertial and Environmental sensor expansion board.
 *          This application makes use of C++ classes obtained from the C
 *          components' drivers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
//NOTE: this example isn't compatible with Arduino Uno

#include <LSM6DSOSensor.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <CayenneLPP.h>
#include <LSM6DSOSensor.h>

void do_send(osjob_t *);

static uint8_t mydata[] = "Double Tap";
static osjob_t sendjob;

#ifdef ARDUINO_SAM_DUE
#define DEV_I2C Wire1
#elif defined(ARDUINO_ARCH_STM32)
#define DEV_I2C Wire
#elif defined(ARDUINO_ARCH_AVR)
#define DEV_I2C Wire
#else
#define DEV_I2C Wire
#endif
#define SerialPort Serial

#define INT_1 4

LSM6DSOSensor *accGyr;
int32_t accelerometer[3];
int32_t gyroscope[3];

//Interrupts.
volatile int mems_event = 0;

char report[256];

void INT1Event_cb();

// For normal use, we require that you edit the sketch to replace FILLMEIN
// with values matching the Helium Console. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN to a non-
// working but innocuous value.

#ifdef COMPILE_REGRESSION_TEST
#define FILLMEIN 0
#else
#warning "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
#define FILLMEIN (#dont edit this, edit the lines that use FILLMEIN)
#endif

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from the Helium Console, this means to reverse
// the bytes.
static const u1_t PROGMEM DEVEUI[8] = {FILLMEIN};
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }

// This should also be in little endian format, see above.
static const u1_t PROGMEM APPEUI[8] = {FILLMEIN};
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the Helium Console can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = {FILLMEIN};
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

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
    .dio = {
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

void printHex2(unsigned v)
{
  v &= 0xff;
  if (v < 16)
    Serial.print('0');
  Serial.print(v, HEX);
}

void do_send(osjob_t *j)
{
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    Serial.println(F("OP_TXRXPEND, not sending"));
  }
  else
  {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, mydata, sizeof(mydata) - 1, 0);
    Serial.println(F("Packet queued"));
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent(ev_t ev)
{
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev)
  {
  case EV_SCAN_TIMEOUT:
    Serial.println(F("EV_SCAN_TIMEOUT"));
    break;
  case EV_BEACON_FOUND:
    Serial.println(F("EV_BEACON_FOUND"));
    break;
  case EV_BEACON_MISSED:
    Serial.println(F("EV_BEACON_MISSED"));
    break;
  case EV_BEACON_TRACKED:
    Serial.println(F("EV_BEACON_TRACKED"));
    break;
  case EV_JOINING:
    Serial.println(F("EV_JOINING"));
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
      for (size_t i = 0; i < sizeof(artKey); ++i)
      {
        if (i != 0)
          Serial.print("-");
        printHex2(artKey[i]);
      }
      Serial.println("");
      Serial.print("NwkSKey: ");
      for (size_t i = 0; i < sizeof(nwkKey); ++i)
      {
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
        ||     Serial.println(F("EV_RFU1"));
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
    if (LMIC.dataLen)
    {
      Serial.println(F("Received "));
      Serial.println(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
    }
    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(60), do_send);
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
        ||    Serial.println(F("EV_SCAN_FOUND"));
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

void setup()
{
  // Led.
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize serial for output.
  SerialPort.begin(115200);

  // Initialize I2C bus.
  DEV_I2C.begin();

  //Interrupts.
  attachInterrupt(INT_1, INT1Event_cb, RISING);

  accGyr = new LSM6DSOSensor(&DEV_I2C);
  accGyr->Enable_X();
  accGyr->Enable_Double_Tap_Detection(LSM6DSO_INT1_PIN);

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
  // the X/1000 means an error rate of 0.1%; the above issue discusses using values up to 10%.
  // so, values from 10 (10% error, the most lax) to 1000 (0.1% error, the most strict) can be used.
  LMIC_setClockError(1 * MAX_CLOCK_ERROR / 40);

  LMIC_setLinkCheckMode(0);
  LMIC_setDrTxpow(DR_SF7, 14);
  LMIC_selectSubBand(6);

  do_send(&sendjob);
}

void loop()
{
  if (mems_event)
  {
    mems_event = 0;
    LSM6DSO_Event_Status_t status;
    accGyr->Get_X_Event_Status(&status);
    if (status.DoubleTapStatus)
    {
      // Output data.
      SerialPort.println("Double Tap Detected!");
      do_send(&sendjob);
      // Led blinking.
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  os_runloop_once();
}

void INT1Event_cb()
{
  mems_event = 1;
}