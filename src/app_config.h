#pragma once
#include <stdint.h>

// =====================
// Secrets (Wi-Fi, etc.)
// =====================
#include "secrets.h"
#ifndef WIFI_SSID
  #error "WIFI_SSID not defined. Create src/secrets.h from secrets_example.h"
#endif

#ifndef WIFI_PASS
  #error "WIFI_PASS not defined. Create src/secrets.h from secrets_example.h"
#endif

// =====================
// mDNS
// =====================
static const char* MDNS_NAME = "iaqm";   // http://iaqm.local/
static const char* WEB_HOST_NAME = "http://iaqm.local/";

// =====================
// I2C addresses
// =====================
static constexpr uint8_t SHT4X_I2C_ADDR = 0x44; // SHT45 (SHT4x family)

// =====================
// Senseair S88 (UART Modbus)
// =====================
static constexpr int S88_UART_RX_PIN = 38;   // ESP32 RX  <- S88 TXD
static constexpr int S88_UART_TX_PIN = 39;   // ESP32 TX  -> S88 RXD
static constexpr uint32_t S88_UART_BAUD = 9600;

// =====================
// LCD Display pins
// =====================
static constexpr int LCD_CS_PIN  = 13;    // D13
static constexpr int LCD_DC_PIN  = 12;    // D12
static constexpr int LCD_RST_PIN = 11;    // D11
static constexpr int LCD_BL_PIN  = 10;   // D10 (set to -1 if tied to 3V)


// =====================
// Scheduler periods
// =====================
static constexpr uint32_t SENSOR_READ_PERIOD_MS   = 5000;     // read sensors every 5s
static constexpr uint32_t WIFI_CHECK_PERIOD_MS    = 5000;     // check Wi-Fi connection every 5s
static constexpr uint32_t SERIAL_STATUS_PERIOD_MS = 60000;    // print status line to Serial every 60s

// =====================
// Indicator timing parameters
// =====================
static constexpr uint32_t WIFI_BLINK_PERIOD_MS = 2000;   // once every 2s
static constexpr uint32_t WIFI_BLINK_ON_MS     = 100;

static constexpr uint32_t NP_CYCLE_PERIOD_MS   = 10000;  // every 10s
static constexpr uint32_t NP_BLINK_ON_MS       = 150;
static constexpr uint32_t NP_BLINK_OFF_MS      = 150;
