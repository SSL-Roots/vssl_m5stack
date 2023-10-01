// Copyright 2023 Roots
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <M5Unified.h>

#include "led_control.hpp"


namespace led_control {

enum class LED_MODE {
  OFF = 0,
  CONTINUOUS,
  BLINK_1HZ
};

const unsigned int IO_LED = GPIO_NUM_26;
LED_MODE g_led_mode = LED_MODE::OFF;
unsigned int g_blink_ms = 500;

void led_off(void) {
  digitalWrite(IO_LED, LOW);
}

void led_continuous(void) {
  digitalWrite(IO_LED, HIGH);
}

void led_blink_1hz(void) {
  digitalWrite(IO_LED, HIGH);
  delay(g_blink_ms);
  digitalWrite(IO_LED, LOW);
  delay(g_blink_ms);
}

void led_control_task(void * arg) {
  std::map<LED_MODE, std::function<void(void)>> led_mode_map = {
    {LED_MODE::OFF, led_off},
    {LED_MODE::CONTINUOUS, led_continuous},
    {LED_MODE::BLINK_1HZ, led_blink_1hz},
  };

  pinMode(IO_LED, OUTPUT);

  M5_LOGI("LED Control Task Start!");

  while (true) {
    led_mode_map[g_led_mode]();
    delay(100);
  }
}

void turn_off() {
  g_led_mode = LED_MODE::OFF;
}

void set_continuous(void) {
  g_led_mode = LED_MODE::CONTINUOUS;
}

void set_blink_ms(const unsigned int ms) {
  g_led_mode = LED_MODE::BLINK_1HZ;
  g_blink_ms = ms;
}

}