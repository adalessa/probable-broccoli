#pragma once

#include "Color.h"
#include "IEncodeable.h"
#include "Rotation.h"
#include <HardwareSerial.h>
#include <cstddef>
#include <cstdint>

#define LCD_WIDTH 480
#define LCD_HEIGHT 272

#define LCD_RX 16
#define LCD_TX 17

#define LCD_BAUDRATE 115200
#define LCD_SERIAL_PORT Serial1
#define SERIAL_8N1 0x800001c

#define RECEIVED_NO_DATA 0x00
#define RECEIVED_SHAKE_HAND_ACK 0x01
#define FHONE 0xAA

#define PENDING(NOW, SOON) ((int32_t)(NOW - (SOON)) < 0)

class LcdApi {
private:
  uint8_t send_buf[11 + LCD_WIDTH / 6 * 2] = {0xAA};
  uint8_t send_buf_tail[4] = {0xCC, 0x33, 0xC3, 0x3C};
  uint8_t read_buf[26] = {0};
  uint8_t receivedType;
  size_t buf_index = 1;
  Rotation rotation_;

  bool handshake();
  void add_byte(uint8_t);
  void add_word(uint16_t);
  void add_long(uint32_t lval);
  void send_();

  void update();

public:
  void initialize(Rotation rot, Color color);
  void rotation(Rotation rot);
  void send(const IEncodeable &buffer);
};
