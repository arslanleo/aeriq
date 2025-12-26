#include "sen55.h"

bool Sen55::begin(TwoWire& wire, uint8_t i2cAddr) {
  addr_ = i2cAddr;

  // Some Sensirion libs take begin(wire) and set address internally,
  // others take begin(wire, address). We'll handle both patterns
  // by calling begin(wire) and setting address via API if available.

  sen5x_.begin(wire);

  // If your library version exposes setI2cAddress(), uncomment:
  // sen5x_.setI2cAddress(addr_);

  uint16_t err = 0;
  char msg[64];

  // Reset device (recommended on boot)
  err = sen5x_.deviceReset();
  if (err) {
    errorToString(err, msg, sizeof(msg));
    Serial.printf("SEN55 deviceReset error: %s\n", msg);
    ok_ = false;
    return false;
  }

  // Start measurements
  err = sen5x_.startMeasurement();
  if (err) {
    errorToString(err, msg, sizeof(msg));
    Serial.printf("SEN55 startMeasurement error: %s\n", msg);
    ok_ = false;
    return false;
  }

  ok_ = true;
  Serial.println("SEN55 started (measurements running).");
  return true;
}

bool Sen55::read(float& pm1p0, float& pm2p5, float& pm4p0, float& pm10p0,
                 float& tempC, float& rh,
                 float& vocIndex, float& noxIndex) {
  if (!ok_) return false;

  uint16_t err = 0;
  char msg[64];

  // Typical API in Sensirion SEN5x library:
  // readMeasuredValues(pm1, pm2_5, pm4, pm10, humidity, temperature, vocIndex, noxIndex)
  // NOTE: order for humidity/temperature can vary between versions.
  // This mapping matches the most common upstream examples.

  float humidity = NAN, temperature = NAN;
  err = sen5x_.readMeasuredValues(pm1p0, pm2p5, pm4p0, pm10p0,
                                 humidity, temperature,
                                 vocIndex, noxIndex);

  if (err) {
    errorToString(err, msg, sizeof(msg));
    Serial.printf("SEN55 readMeasuredValues error: %s\n", msg);
    ok_ = false; // mark unhealthy; you can choose to keep trying
    return false;
  }

  // normalize naming
  rh = humidity;
  tempC = temperature;

  return true;
}
