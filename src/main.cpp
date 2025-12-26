#include <Arduino.h>
#include <Wire.h>

#include "app_config.h"
#include "app_state.h"

#include "sensors/i2c_utils.h"
#include "sensors/sensors.h"

#include "net/wifi_mgr.h"
#include "net/web_server.h"
#include "net/mdns_mgr.h"

#include "ui/indicators.h"
#include "ui/display_lcd.h"

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("\nIAQM Bring-up: SHT45 + VEML7700 + Wi-Fi web server");

  Wire.begin();
  Wire.setClock(100000);

  // LCD pins
  LcdPins lcdPins;
  lcdPins.cs  = LCD_CS_PIN;
  lcdPins.dc  = LCD_DC_PIN;
  lcdPins.rst = LCD_RST_PIN;
  lcdPins.bl  = LCD_BL_PIN;   // BL tied to 3V for now (no PWM)

  display_init(lcdPins);

  I2CUtils::scan(Serial);

  Indicators::begin();
  Sensors::begin(g);

  WifiMgr::begin();
  WebServerMgr::begin();
  MdnsMgr::begin();

  Serial.println("Open a browser to: http://iaqm.local/  (or use the IP printed above)");
}

void loop() {
  WebServerMgr::tick();
  WifiMgr::tick();

  static uint32_t lastReadMs = 0;
  if (millis() - lastReadMs >= SENSOR_READ_PERIOD_MS) {
    Sensors::read(g, WifiMgr::isConnected());
    lastReadMs = millis();
  }

  Indicators::tick(g);

  display_update(g, WifiMgr::isConnected(), WEB_HOST_NAME);
}
