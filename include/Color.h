#pragma once

#include <cstdint>

//// ===== Basic RGB565 Color Palette =====
enum Color : uint16_t {
  Black = 0x0000,      //   0,  0,  0
  White = 0xFFFF,      // 255,255,255
  Red = 0xF800,        // 255,  0,  0
  Lime = 0x07E0,       //   0,255,  0
  Blue = 0x001F,       //   0,  0,255
  Yellow = 0xFFE0,     // 255,255,  0
  Cyan = 0x07FF,       //   0,255,255
  Magenta = 0xF81F,    // 255,  0,255
  Gray = 0x8410,       // 128,128,128
  DarkGray = 0x4208,   //  64, 64, 64
  Orange = 0xFD20,     // 255,165,  0
  Brown = 0xA145,      // 165, 42, 42
  Pink = 0xFC18,       // 255,128,128
  Purple = 0x780F,     // 128,  0,128
  Teal = 0x0410,       //   0,128,128
  Navy = 0x000F,       //   0,  0,128
  SkyBlue = 0x867D,    // 135,206,235
  LightGreen = 0x87F0, // 173,255, 47
  DarkGreen = 0x03E0,  //   0,128,  0
  Gold = 0xFEA0,       // 255,215,  0
};
