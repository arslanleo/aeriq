#pragma once
#include <Arduino.h>

namespace WifiMgr {
  void begin();            // connect + config (blocking up to timeout)
  void tick();             // periodic reconnect attempts
  bool isConnected();
}
