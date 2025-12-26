#pragma once
#include <Arduino.h>
#include <Wire.h>

// Sensirion SEN5x library
#include <SensirionI2CSen5x.h>

class Sen55 {
public:
  bool begin(TwoWire& wire, uint8_t i2cAddr = 0x69);
  bool read(float& pm1p0, float& pm2p5, float& pm4p0, float& pm10p0,
            float& tempC, float& rh,
            float& vocIndex, float& noxIndex);

  bool isOk() const { return ok_; }

private:
  SensirionI2CSen5x sen5x_;
  bool ok_ = false;
  uint8_t addr_ = 0x69;
};
