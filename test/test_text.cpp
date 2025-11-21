#include <Arduino.h>
#include <unity.h>
#include "Text.h"

void test_text_padding() {
  Text t("Hi", 5);
  std::vector<uint8_t> encoded = t.encode();
  // The encoded text starts after the first 10 bytes
  std::string result(encoded.begin() + 10, encoded.end());
  TEST_ASSERT_EQUAL_STRING_LEN("Hi   ", result.c_str(), 5);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_text_padding);
  UNITY_END();
}

void loop() {}
