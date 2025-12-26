#pragma once
#include <Arduino.h>
#include "../app_state.h"

namespace Sensors {
  void begin(Readings& state);
  void read(Readings& state, bool wifiConnected);   // updates state; errors print immediately
  int  countHealthy(const Readings& state);         // for NeoPixel blink count
}
