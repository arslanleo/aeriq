#include "indicators.h"
#include "../app_config.h"

#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "../sensors/sensors.h"

// ---------------------------
// LED pins (robust defaults)
// ---------------------------
#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

#ifndef PIN_NEOPIXEL
  // Fallback if core doesn't define it (most Adafruit cores do define it)
  #define PIN_NEOPIXEL 33
#endif

// One NeoPixel on-board
static Adafruit_NeoPixel pixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// ---------------------------
// Indicator runtime state
// ---------------------------
static uint32_t wifiBlinkT0 = 0;

static uint32_t npCycleT0 = 0;
static uint32_t npStepT0  = 0;
static int npRemainingBlinks = 0;
static bool npIsOn = false;

static void setNeoPixelBlue(bool on) {
  if (on) {
    pixel.setPixelColor(0, pixel.Color(0, 0, 50)); // blue (not too bright)
  } else {
    pixel.setPixelColor(0, 0);
  }
  pixel.show();
}

namespace Indicators {

void begin() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Some Adafruit boards require NeoPixel power enable (compile-time defines)
  #if defined(NEOPIXEL_POWER)
    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, HIGH);
  #elif defined(PIN_NEOPIXEL_POWER)
    pinMode(PIN_NEOPIXEL_POWER, OUTPUT);
    digitalWrite(PIN_NEOPIXEL_POWER, HIGH);
  #endif

  pixel.begin();
  setNeoPixelBlue(false);

  wifiBlinkT0 = millis();
  npCycleT0   = millis();
  npStepT0    = millis();
}

void tick(const Readings& state) {
  const uint32_t now = millis();
  const bool wifiOk = (WiFi.status() == WL_CONNECTED);

  // Fixed LED: blink once every 2s when WiFi OK, otherwise OFF
  if (!wifiOk) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    uint32_t phase = (now - wifiBlinkT0) % WIFI_BLINK_PERIOD_MS;
    digitalWrite(LED_BUILTIN, (phase < WIFI_BLINK_ON_MS) ? HIGH : LOW);
  }

  // NeoPixel: every 10s, blink n times (blue), where n = healthy sensors
  if (now - npCycleT0 >= NP_CYCLE_PERIOD_MS) {
    npCycleT0 = now;

    npRemainingBlinks = Sensors::countHealthy(state);
    npIsOn = false;
    npStepT0 = now;
    setNeoPixelBlue(false);
  }

  if (npRemainingBlinks > 0) {
    if (!npIsOn) {
      if (now - npStepT0 >= NP_BLINK_OFF_MS) {
        npIsOn = true;
        npStepT0 = now;
        setNeoPixelBlue(true);
      }
    } else {
      if (now - npStepT0 >= NP_BLINK_ON_MS) {
        npIsOn = false;
        npStepT0 = now;
        setNeoPixelBlue(false);
        npRemainingBlinks--;
      }
    }
  }
}

} // namespace Indicators
