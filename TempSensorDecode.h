/*
  TempSensorDecode.h - Library for decoding
  prologue temperature sensor data from inexpensive
  433 MHz based weather sensors.
  Created by Martin Marinov, 2020
  Released into the public domain.
*/

#ifndef TempSensorDecode_h
#define WTempSensorDecode_h

#include "Arduino.h"

class TempSensorDecode
{
  public:
    static void setup(int pin);
    static bool hasNewData();
    static bool hasAnyData();

    static float getTemperature();
    static uint8_t getHumidity();
    static bool getIsButtonPressed();
    static uint8_t getBatteryState();
    static uint8_t getChannel();

  private:
  	TempSensorDecode() {}

    static float _temperature;
    static uint8_t _humidity;
    static bool _isButtonPressed;
    static uint8_t _batteryState;
    static uint8_t _channel;
    static volatile bool _hasNewData;
    static bool _hasAnyData;

    static void _handleInterrupt();
    static void _handleDuration(unsigned long duration);
    static void _handlePacket(uint64_t packet);
};

#endif