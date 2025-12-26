#include "s88.h"
#include "../app_config.h"

#include <Arduino.h>
#include <HardwareSerial.h>

// Use UART1 (keeps USB Serial0 free for logging)
static HardwareSerial S88Serial(1);

static uint16_t modbusCRC16(const uint8_t* data, size_t len) {
  // Standard Modbus RTU CRC16 (poly 0xA001)
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int b = 0; b < 8; b++) {
      if (crc & 1) crc = (crc >> 1) ^ 0xA001;
      else         crc >>= 1;
    }
  }
  return crc;
}

namespace S88 {

void begin(int rxPin, int txPin, uint32_t baud) {
  // S88 default: 8 data bits, no parity; it transmits with 2 stop bits but
  // receiving with 1 stop bit is fine (extra stop bit is just idle high). :contentReference[oaicite:3]{index=3}
  S88Serial.begin(baud, SERIAL_8N1, rxPin, txPin);
  S88Serial.setTimeout(250);

  // Clear any junk bytes from power-up
  while (S88Serial.available()) (void)S88Serial.read();
}

bool readCO2ppm(uint16_t& ppm, uint32_t timeoutMs) {
  // From Senseair S88 Modbus examples: Read CO2 value from IR4 using function 0x04
  // Address=0xFE (any address), Start=0x0003, Qty=0x0001. :contentReference[oaicite:4]{index=4}
  uint8_t req[8] = {0xFE, 0x04, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00};
  const uint16_t crc = modbusCRC16(req, 6);
  req[6] = crc & 0xFF;         // CRC Lo
  req[7] = (crc >> 8) & 0xFF;  // CRC Hi

  // Flush RX buffer to avoid stale bytes
  while (S88Serial.available()) (void)S88Serial.read();

  // Send request
  S88Serial.write(req, sizeof(req));
  S88Serial.flush();

  // Expected reply: FE 04 02 HI LO CRCLO CRCHI (7 bytes) :contentReference[oaicite:5]{index=5}
  uint8_t resp[7] = {0};
  const uint32_t t0 = millis();
  size_t got = 0;

  while ((millis() - t0) < timeoutMs && got < sizeof(resp)) {
    if (S88Serial.available()) {
      int c = S88Serial.read();
      if (c >= 0) resp[got++] = (uint8_t)c;
    } else {
      delay(2);
    }
  }
  if (got != sizeof(resp)) return false;

  // Basic header check
  if (resp[0] != 0xFE || resp[1] != 0x04 || resp[2] != 0x02) return false;

  // CRC check (last two bytes are CRC Lo/Hi)
  const uint16_t respCrc = (uint16_t)resp[5] | ((uint16_t)resp[6] << 8);
  const uint16_t calcCrc = modbusCRC16(resp, 5);
  if (respCrc != calcCrc) return false;

  ppm = ((uint16_t)resp[3] << 8) | (uint16_t)resp[4];

  // Optional sanity: don’t reject aggressively; just basic bounds
  if (ppm == 0 || ppm > 20000) return false;

  return true;
}

} // namespace S88
