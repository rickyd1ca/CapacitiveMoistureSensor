/*
Copyright 2020 Eric Dyke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "CapacitiveMoistureSensor.h"

CapacitiveMoistureSensor::CapacitiveMoistureSensor(InputInterface& _input) :
  lastReadValueTime(0),
  lastValue(0),
  lastValidValue(0),
  input(_input),
  soilHumidity(CapacitiveMoistureSensor::SOIL_HUMIDITY_ERROR),
  numberErraticRead(0)
{
}

void CapacitiveMoistureSensor::read() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastReadValueTime > CapacitiveMoistureSensor::HUMIDITY_POLL_PERIOD) {
    // Read current value
    uint16_t currentValue = input.read();
    lastReadValueTime = currentMillis;

    if (abs(lastValue - currentValue) < 20 &&
        currentValue > CapacitiveMoistureSensor::HUMIDITY_MIN_VALUE &&
        currentValue < CapacitiveMoistureSensor::HUMIDITY_MAX_VALUE) {
      if (numberErraticRead > 0) {
        numberErraticRead--;
      }
      if (numberErraticRead == 0) {
        // 2 values are close enough, evaluate the state
        if (currentValue < CapacitiveMoistureSensor::HUMIDITY_WET_THREASHOLD) {
          soilHumidity = CapacitiveMoistureSensor::SOIL_HUMIDITY_WET;
        } else if (currentValue < CapacitiveMoistureSensor::HUMIDITY_HUMID_THREASHOLD) {
          soilHumidity = CapacitiveMoistureSensor::SOIL_HUMIDITY_HUMID;
        } else {
          soilHumidity = CapacitiveMoistureSensor::SOIL_HUMIDITY_DRY;
        }
        lastValidValue = currentValue;
      }
    } else {
      numberErraticRead++;
      if (numberErraticRead > 20) {
        soilHumidity = CapacitiveMoistureSensor::SOIL_HUMIDITY_ERROR;
        numberErraticRead = 20;
      }
    }
    lastValue = currentValue;
  }

}

CapacitiveMoistureSensor::Humidity CapacitiveMoistureSensor::getMoisture() {
  return soilHumidity;
}

float CapacitiveMoistureSensor::getMoistureLevel() {

  if (soilHumidity == CapacitiveMoistureSensor::SOIL_HUMIDITY_ERROR) {
    return 0;
  }

  uint32_t normalizedValue = CapacitiveMoistureSensor::HUMIDITY_MAX_VALUE - lastValidValue;
  float moisturePercentage = (float)normalizedValue / (float)(CapacitiveMoistureSensor::HUMIDITY_MAX_VALUE - CapacitiveMoistureSensor::HUMIDITY_MIN_VALUE);

  return moisturePercentage * 100;
}

uint16_t CapacitiveMoistureSensor::getValue() {
  return lastValidValue;
}

const char* CapacitiveMoistureSensor::getStateStr() {
  static const char *stateStr[] = {
    "W",
    "H",
    "D",
    "E"
  };

  return stateStr[soilHumidity];
}

void CapacitiveMoistureSensor::setMoisture(Humidity moisture) {
  soilHumidity = moisture;
}
