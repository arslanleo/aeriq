#include "mdns_mgr.h"
#include "../app_config.h"

#include <Arduino.h>
#include <ESPmDNS.h>

namespace MdnsMgr {

void begin() {
  if (MDNS.begin(MDNS_NAME)) {
    MDNS.addService("http", "tcp", 80);
    Serial.printf("mDNS started: http://%s.local/\n", MDNS_NAME);
  } else {
    Serial.println("mDNS failed to start.");
  }
}

} // namespace MdnsMgr
