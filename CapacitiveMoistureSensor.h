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
#ifndef CapacitiveMoistureSensor_h
#define CapacitiveMoistureSensor_h
#include "Arduino.h"

class CapacitiveMoistureSensor
{
  public:
    typedef enum {
      SOIL_HUMIDITY_WET,
      SOIL_HUMIDITY_HUMID,
      SOIL_HUMIDITY_DRY,
      SOIL_HUMIDITY_ERROR
    } Humidity;

    CapacitiveMoistureSensor();
    CapacitiveMoistureSensor(int pin);
    void read();
    Humidity getMoisture();
    float getMoistureLevel();
    uint16_t getValue();
    const char* getStateStr();
    void setMoisture(Humidity moisture);

  private:
    static const uint16_t HUMIDITY_WET_THREASHOLD = 423;
    static const uint16_t HUMIDITY_HUMID_THREASHOLD = 516;
    static const uint16_t HUMIDITY_POLL_PERIOD = 200;
    static const uint16_t HUMIDITY_MAX_VALUE = 700;
    static const uint16_t HUMIDITY_MIN_VALUE = 320;

    unsigned long lastReadValueTime;
    uint16_t lastValue;
    uint8_t pin;
    Humidity soilHumidity;
    uint8_t numberErraticRead;
};


#endif
