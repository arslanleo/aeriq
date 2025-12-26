#include "i2c_utils.h"
#include <Wire.h>

namespace I2CUtils {

void scan(Stream& out) {
  out.print("I2C devices: ");
  bool any = false;

  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t err = Wire.endTransmission();
    if (err == 0) {
      char buf[8];
      snprintf(buf, sizeof(buf), "0x%02X ", addr);
      out.print(buf);
      any = true;
    }
  }

  if (!any) out.print("(none)");
  out.println();
}

} // namespace I2CUtils
