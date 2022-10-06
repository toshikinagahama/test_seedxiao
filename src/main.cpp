#include <Arduino.h>
#include "LSM6DS3.h"
#include "Wire.h"
#include <ArduinoBLE.h>
BLEService ledService("4FAFC201-1FB5-459E-8FCC-C5C9C331914B");
BLEStringCharacteristic switchCharacteristic("BEB5483E-36E1-4688-B7F5-EA07361B26A8", BLERead | BLEWrite, 24);

// Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A); // I2C device address 0x6A
const int ledPin = LED_BUILTIN;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  if (myIMU.begin() != 0)
  {
    Serial.println("Device error");
  }
  else
  {
    Serial.println("Device OK!");
  }
  // begin initialization
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1)
      ;
  }
  BLE.setLocalName("DualGY521_M5Stamp");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}

void loop()
{
  BLEDevice central = BLE.central();
  if (central)
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected())
    {
      if (switchCharacteristic.written())
      {
        String val_string = switchCharacteristic.value();
        char val[128];
        val_string.toCharArray(val, 128);
        Serial.println(val);
        if (strcmp(val, "1111") == 0)
        {
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH); // will turn the LED on
        }
        else if (strcmp(val, "2222") == 0)
        {
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW); // will turn the LED off
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  // Serial.print("\nAccelerometer:\n");
  // Serial.print(" X1 = ");
  // Serial.println(myIMU.readFloatAccelX(), 4);
  // Serial.print(" Y1 = ");
  // Serial.println(myIMU.readFloatAccelY(), 4);
  // Serial.print(" Z1 = ");
  // Serial.println(myIMU.readFloatAccelZ(), 4);

  // // Gyroscope
  // Serial.print("\nGyroscope:\n");
  // Serial.print(" X1 = ");
  // Serial.println(myIMU.readFloatGyroX(), 4);
  // Serial.print(" Y1 = ");
  // Serial.println(myIMU.readFloatGyroY(), 4);
  // Serial.print(" Z1 = ");
  // Serial.println(myIMU.readFloatGyroZ(), 4);

  // // Thermometer
  // Serial.print("\nThermometer:\n");
  // Serial.print(" Degrees C1 = ");
  // Serial.println(myIMU.readTempC(), 4);
  // Serial.print(" Degrees F1 = ");
  // Serial.println(myIMU.readTempF(), 4);

  // delay(1000);
}