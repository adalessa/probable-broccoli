#pragma once

#include <cstdint>
enum class AppMode : uint_fast8_t {
  Klipper = 0,
  Home = 1,
  Tasks = 2,
  Message = 3,
};
