#pragma once
#include <Arduino.h>

namespace S88 {
  void begin(int rxPin, int txPin, uint32_t baud = 9600);
  bool readCO2ppm(uint16_t& ppm, uint32_t timeoutMs = 250);
}
