/*
  TempSensorDecode.cpp - Library for decoding
  prologue temperature sensor data from inexpensive
  433 MHz based weather sensors.
  Created by Martin Marinov, 2020
  Released into the public domain.
*/

#include "Arduino.h"
#include "TempSensorDecode.h"

#define PULSE ((long) 500)
#define ZERO ((long) 2000)
#define ONE ((long) 4000)
#define SYNC ((long) 8500)
#define ERROR_PERCENTAGE (10)
#define NUMBER_OF_BITS_IN_PACKET (36)

#define NO_SYNC (-999)

#define CHECK(duration, expected_duration) ( (duration * 100 > expected_duration * (100 - ERROR_PERCENTAGE)) && (duration * 100 < expected_duration * (100 + ERROR_PERCENTAGE)) )

#if defined(ESP8266) || defined(ESP32)
    #define ISR_PREFIX ICACHE_RAM_ATTR
#else
    #define ISR_PREFIX
#endif

volatile int16_t TempSensorDecode::_temperature_raw;
volatile uint8_t TempSensorDecode::_humidity;
volatile bool TempSensorDecode::_isButtonPressed;
volatile uint8_t TempSensorDecode::_batteryState;
volatile uint8_t TempSensorDecode::_channel;
volatile bool TempSensorDecode::_hasNewData = false;
volatile bool TempSensorDecode::_hasAnyData = false;
volatile uint64_t TempSensorDecode::_packet_raw;

// API

bool TempSensorDecode::hasNewData() {
  bool hasNewData = TempSensorDecode::_hasNewData;
  TempSensorDecode::_hasNewData = false;
  return hasNewData;
}

bool TempSensorDecode::hasAnyData() {
  return TempSensorDecode::_hasAnyData;
}

float TempSensorDecode::getTemperature() {
  return float(TempSensorDecode::_temperature_raw) / 10;
}

uint8_t TempSensorDecode::getHumidity() {
  return TempSensorDecode::_humidity;
}

bool TempSensorDecode::getIsButtonPressed() {
  return TempSensorDecode::_isButtonPressed;
}

uint8_t TempSensorDecode::getBatteryState() {
  return TempSensorDecode::_batteryState;
}

uint8_t TempSensorDecode::getChannel() {
  return TempSensorDecode::_channel;
}

uint64_t TempSensorDecode::getRawPacket() {
  return TempSensorDecode::_packet_raw;
}

// Driver & Signal processing

void TempSensorDecode::setup(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
  attachInterrupt(digitalPinToInterrupt(pin), TempSensorDecode::_handleInterrupt, CHANGE);
}

ISR_PREFIX void TempSensorDecode::_handleInterrupt() {
  static unsigned long lastTime;
  long timeNow = micros();
  unsigned long duration = timeNow - lastTime;
  lastTime = timeNow;
  TempSensorDecode::_handleDuration(duration);
}

void TempSensorDecode::_handleDuration(unsigned long duration) {
  static bool lastWasPulse = false;
  static int validBits = NO_SYNC;
  static uint64_t packet = 0;
  
  bool isPulse = CHECK(duration, PULSE);

  if (lastWasPulse) {
    if (CHECK(duration, SYNC)) {
      // Start of data transmission
      validBits = 0;
    } else if (duration > PULSE && (duration * 100) < (ONE * (100 + ERROR_PERCENTAGE))) {
      // We have a valid zero or one, let's try to distinguish
      packet = packet << 1;
      if (2 * duration > ZERO + ONE) {
        // It's a 1
        bitSet(packet, 0);
      } else {
        // It's a 0
        bitClear(packet, 0);
      }
      validBits++;
      if (validBits == NUMBER_OF_BITS_IN_PACKET) {
        TempSensorDecode::_handlePacket(packet);
      } else if (validBits > NUMBER_OF_BITS_IN_PACKET) {
        // Too many bits, something is wrong
        validBits = NO_SYNC;
      }
    } else {
      // We were expecting a 0 or 1
      validBits = NO_SYNC;
    }
  } else if (!isPulse) {
    // Last was not pulse, this was not pulse
    // Reset as this is invalid packet
    validBits = NO_SYNC;
  }
  
  lastWasPulse = isPulse;
}

void TempSensorDecode::_handlePacket(uint64_t packet) {
  uint8_t battery = (packet >> 20) & 0x8;
  uint8_t channel = 1 + ((packet >> 20) & 0x3); // Channels start at 0
  // ESP32 do not support floating point operations in interrupts. Arrange
  // to shift the left-aligned 12-bit value as an int16_t so that we get
  // sign extension for the final value to support negative numbers.
  int16_t temperature_raw = (packet >> 4) & 0xFFFF;
  temperature_raw >>= 4;
  uint8_t humidity = packet & 0xFF;
  bool button = (packet >> 20) & 0x4;

  TempSensorDecode::_packet_raw = packet;

  if (temperature_raw > 550 || temperature_raw < -250) {
    // invalid temperature
    return;
  }

  if (humidity > 100) {
    // invalid humidity
    return;
  }

  TempSensorDecode::_temperature_raw = temperature_raw;
  TempSensorDecode::_humidity = humidity;
  TempSensorDecode::_channel = channel;
  TempSensorDecode::_batteryState = battery;
  TempSensorDecode::_isButtonPressed = button;

  TempSensorDecode::_hasNewData = true;
  TempSensorDecode::_hasAnyData = true;
}
