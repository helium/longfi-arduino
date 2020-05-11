/* Simple OTAA join for a LoRaWAN network
 *  
 *  In setup() below please replace the argument to LoRaWAN.begin()
 *  with your appropriate region specific band:
 *
 *  AS923
 *  AU915
 *  EU868
 *  IN865
 *  KR920
 *  US915
 *
 *  AU915/US915 networks have 64+8 channels. Typical gateways support only
 *  8 (9) channels. Hence it's a good idea to pick the proper channel
 *  subset via select via LoRaWAN.setSubBand(),
 *    
 *  EU868/IN865 have duty cycle restrictions. For debugging it makes sense
 *  to disable those via setDutyCycle(false);
 *    
 *  For an external antenna one should set the proper antenna gain
 *  (default is 2.0) via setAntennaGain().
 *    
 *  Please edit the keys below as they are just debugging samples.
 *    
 *    
 * This example code is in the public domain.
 */
#include "LoRaWAN.h"
#include <CayenneLPP.h>

#include <LSM6DSOSensor.h>
#include <LIS2DW12Sensor.h>
#include <LIS2MDLSensor.h>
#include <LPS22HHSensor.h>
#include <STTS751Sensor.h>
#include <HTS221Sensor.h>

const char *devEui = "FILL_ME_IN";
const char *appEui = "FILL_ME_IN";
const char *appKey = "FILL_ME_IN";

// Sensors
LSM6DSOSensor *AccGyr;
LPS22HHSensor *PressTemp;
HTS221Sensor *HumTemp;

// Init CayenneLPP Payload
CayenneLPP lpp(51);

void readSensors() {
  // Read humidity and temperature.
  float humidity = 0;
  float temperature = 0;
  HumTemp->GetHumidity(&humidity);
  HumTemp->GetTemperature(&temperature);

  // Read pressure and temperature.
  float pressure = 0;
  PressTemp->GetPressure(&pressure);

  // Read accelerometer and gyroscope.
  int32_t accelerometer[3];
  int32_t gyroscope[3];
  AccGyr->Get_X_Axes(accelerometer);
  AccGyr->Get_G_Axes(gyroscope);

  // Clear Payload
  lpp.reset();
  
  // Pack Packload
  lpp.addTemperature(1, temperature);
  lpp.addRelativeHumidity(2, humidity);
  lpp.addBarometricPressure(3, pressure); 
  lpp.addAccelerometer(4, accelerometer[0], accelerometer[1], accelerometer[2]);
  lpp.addGyrometer(5, gyroscope[0], gyroscope[1], gyroscope[2]);

  // Debug Print Data 
  Serial.print("| Hum[%]: ");
  Serial.print(humidity, 2);
  Serial.print(" | Temp[C]: ");
  Serial.print(temperature, 2);
  Serial.print(" | Pres[hPa]: ");
  Serial.print(pressure, 2);
  Serial.print(" | Acc[mg]: ");
  Serial.print(accelerometer[0]);
  Serial.print(" ");
  Serial.print(accelerometer[1]);
  Serial.print(" ");
  Serial.print(accelerometer[2]);
  Serial.print(" | Gyr[mdps]: ");
  Serial.print(gyroscope[0]);
  Serial.print(" ");
  Serial.print(gyroscope[1]);
  Serial.print(" ");
  Serial.println(gyroscope[2]);
}

void setup( void )
{
  Serial.begin(9600);
  
  while (!Serial) { }

  // Initialize I2C bus.
  Wire.begin();

  // Enable Sensors
  AccGyr = new LSM6DSOSensor (&Wire);
  AccGyr->Enable_X();
  AccGyr->Enable_G();
  PressTemp = new LPS22HHSensor(&Wire);
  PressTemp->Enable();
  HumTemp = new HTS221Sensor (&Wire);
  HumTemp->Enable();

  // US Region
  LoRaWAN.begin(US915);
  // Helium SubBand
  LoRaWAN.setSubBand(7);
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

    // Read Sensor Values
    readSensors();

    // Send Packet
    LoRaWAN.sendPacket(1, lpp.getBuffer(), lpp.getSize());
  }

  delay(20000);
}