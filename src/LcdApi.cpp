#include "LcdApi.h"
#include "IEncodeable.h"
#include "Rect.h"

void LcdApi::initialize(Rotation rot, Color color) {
  delay(800);
  while (!handshake()) {
    Serial.println("No ack from LCD Retrying!\n");
  }

  Serial.println("DWIN LCD connected\n");

  update();
  rotation(rot);

  if (rot % 2 == 0) {
    send(Rect().color(color).start(0, 0).end(LCD_WIDTH, LCD_HEIGHT));
  } else {
    send(Rect().color(color).start(0, 0).end(LCD_HEIGHT, LCD_WIDTH));
  }
}

void LcdApi::rotation(Rotation rot) {
  rotation_ = rot;
  add_byte(0x34);
  add_byte(0x5A);
  add_byte(0xA5);
  add_byte(rot);
  send_();
  update();
}

void LcdApi::send(const IEncodeable &buffer) {
  // get the buffer and copy properly to
  std::vector<uint8_t> data = buffer.encode();
  for (uint8_t byte : data) {
    add_byte(byte);
  }

  send_();
  update();
}

void LcdApi::update() {
  add_byte(0x3D);
  send_();
}

bool LcdApi::handshake() {
  LCD_SERIAL_PORT.begin(LCD_BAUDRATE, SERIAL_8N1, LCD_RX, LCD_TX);
  const uint32_t serial_connect_timeout = millis() + 1000UL;
  while (!LCD_SERIAL_PORT.available() &&
         PENDING(millis(), serial_connect_timeout)) { /*nada*/
  }
  delay(10);

  add_byte(0x00);
  send_();

  delay(20);

  size_t recnum = 0;

  while (LCD_SERIAL_PORT.available() > 0 &&
         recnum < (signed)sizeof(this->read_buf)) {
    this->read_buf[recnum] = LCD_SERIAL_PORT.read();

    if (read_buf[0] != FHONE) {
      if (recnum > 0) {
        recnum = 0;
        memset(read_buf, 0, sizeof(read_buf));
      }
      continue;
    }
    delay(10);
    recnum++;
  }

  return (recnum >= 3 && read_buf[0] == FHONE && read_buf[1] == '\0' &&
          read_buf[2] == 'O' && read_buf[3] == 'K');
}

void LcdApi::add_byte(uint8_t byte) { send_buf[buf_index++] = byte; }

void LcdApi::send_() {
  for (int i = 0; i < this->buf_index; i++) {
    LCD_SERIAL_PORT.write(this->send_buf[i]);
    delayMicroseconds(1);
  }

  for (int i = 0; i < 4; i++) {
    LCD_SERIAL_PORT.write(this->send_buf_tail[i]);
    delayMicroseconds(1);
  }

  /* Reset buffer index */
  this->buf_index = 1;
}
