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

const unsigned int IO_LED = GPIO_NUM_26;
LED_MODE g_led_mode = LED_MODE::OFF;

void init_gpio(void) {
  pinMode(IO_LED, OUTPUT);
}

void led_off(void) {
  digitalWrite(IO_LED, LOW);
}

void led_continuous(void) {
  digitalWrite(IO_LED, HIGH);
}

void led_blink_1hz(void) {
  digitalWrite(IO_LED, HIGH);
  delay(500);
  digitalWrite(IO_LED, LOW);
  delay(500);
}

void led_control_task(void * arg) {
  std::map<LED_MODE, std::function<void(void)>> led_mode_map = {
    {LED_MODE::OFF, led_off},
    {LED_MODE::CONTINUOUS, led_continuous},
    {LED_MODE::BLINK_1HZ, led_blink_1hz},
  };

  M5_LOGI("LED Control Task Start!");

  while (true) {
    led_mode_map[g_led_mode]();
    ets_delay_us(1000);
  }
}

}