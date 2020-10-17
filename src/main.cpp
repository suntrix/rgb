// #include <Arduino.h>


// #include <Wire.h>
// #include <BME280I2C.h>
// #include <EnvironmentCalculations.h>

// BME280I2C bme;

// void setupBME280()
// {
//   // brzo_i2c_setup(SDA, SCL, I2C_ACK_TIMEOUT);

//   while(!bme.begin())
//   {
//     Serial.println("Could not find BME280 sensor!");
//     delay(1000);
//   }

//   switch(bme.chipModel())
//   {
//     case BME280::ChipModel_BME280:
//       Serial.println("Found BME280 sensor! Success.");
//       break;
//     case BME280::ChipModel_BMP280:
//       Serial.println("Found BMP280 sensor! No Humidity available.");
//       break;
//     default:
//       Serial.println("Found UNKNOWN sensor! Error!");
//   }
// }

// void printBME280Data(Stream* client)
// {
//   float temp(NAN), hum(NAN), pres(NAN);

//   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
//   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

//   bme.read(pres, temp, hum, tempUnit, presUnit);

//   client->print("Temp: ");
//   client->print(temp);
//   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
//   client->print("\t\tHumidity: ");
//   client->print(hum);
//   client->print("% RH");
//   client->print("\t\tPressure: ");
//   client->print(pres);
//   client->println("Pa");
// }


// #include <Adafruit_INA219.h>

// Adafruit_INA219 ina219;

// void setupINA219()
// {
//   while(!ina219.begin())
//   {
//     Serial.println("Could not find INA219!");
//     delay(1000);
//   }
// }

// void printINA219Data(Stream* client)
// {
//   float shuntvoltage = 0;
//   float busvoltage = 0;
//   float current_mA = 0;
//   float loadvoltage = 0;
//   float power_mW = 0;

//   shuntvoltage = ina219.getShuntVoltage_mV();
//   busvoltage = ina219.getBusVoltage_V();
//   current_mA = ina219.getCurrent_mA();
//   power_mW = ina219.getPower_mW();
//   loadvoltage = busvoltage + (shuntvoltage / 1000);
  
//   client->print("Bus Voltage: "); 
//   client->print(busvoltage);
//   client->println(" V");
//   client->print("Shunt Voltage: ");
//   client->print(shuntvoltage);
//   client->println(" mV");
//   client->print("Load Voltage: ");
//   client->print(loadvoltage);
//   client->println(" V");
//   client->print("Current: ");
//   client->print(current_mA);
//   client->println(" mA");
//   client->print("Power: ");
//   client->print(power_mW);
//   client->println(" mW");
//   client->println();
// }


#include <Adafruit_NeoPixel.h>

#define DATA_PIN 12
#define NUM_LEDS 144
// #define NUM_LEDS 84

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// void breathe() 
// {
//   float MaximumBrightness = 150;
//   float SpeedFactor = 0.008;
//   float StepDelay = 1;

//   for (int i = 0; i < 65535; i++)
//   {
//     float intensity = MaximumBrightness / 2.0 * (1.0 + sin(SpeedFactor * i));
//     strip.setBrightness(intensity);

//     for (int ledNumber = 0; ledNumber < NUM_LEDS; ledNumber++)
//     {
//       strip.setPixelColor(ledNumber, 255, 0, 0);
//     }

//     strip.show();

//     delay(StepDelay);
//   }
// }


// #include <Ticker.h>

// Ticker ticker;

// void printSersorData()
// {
//   printBME280Data(&Serial);
//   printINA219Data(&Serial);
// }

#include "secrets.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

void setupBlynk()
{
  Blynk.begin(SECRETS_BLYNK_AUTH, SECRETS_WIFI_SSID, SECRETS_WIFI_PASSWORD);

  strip.begin();
  // strip.setBrightness(50);
}

// uint32_t Wheel(byte WheelPos) {
//   if (WheelPos < 85) {
//     return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//   } else if (WheelPos < 170) {
//     WheelPos -= 85;
//     return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//   } else {
//     WheelPos -= 170;
//     return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//   }
// }

BLYNK_WRITE(V1)
{
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
  Serial.println(R);
  Serial.println(G);
  Serial.println(B);

  for (int i=0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, strip.Color(R,G,B));
  }

  strip.show();
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();

  strip.setBrightness(pinValue);

  strip.show();
}
//   if (pinValue == 1) {
//     strip.setBrightness(0);
//   } else {
//     strip.setBrightness(50);
//   }
// }

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
}

BlynkTimer timer;

// void sync()
// {
//   Blynk.syncVirtual(V2);
// }


void setup()
{
  Serial.begin(76800);

  // Wire.begin();
  
  // setupBME280();
  // setupINA219();
  
  // strip.begin();
  // strip.setBrightness(175);

  // // for (int ledNumber = 0; ledNumber < NUM_LEDS; ledNumber++)
  // // {
  // //   strip.setPixelColor(ledNumber, 255, 255, 255);
  // //   // strip.setPixelColor(ledNumber, 255, 147, 41);
  // // }

  // strip.show();

  setupBlynk();

  // ticker.attach(1, &printSersorData);

  // timer.setInterval(100L, sync);
}

void loop()
{
  Blynk.run();
  // timer.run();

  // breathe();
}
