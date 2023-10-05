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
#include "menu_select.hpp"

namespace menu_select {

const unsigned int LONG_PRESS_MS = 5000;
const unsigned int MIDDLE_PRESS_MS = 1000;
const unsigned int SHORT_PRESS_MS = 10;

unsigned long watch_button_press(void) {
  const unsigned int LONG_PRESS_BLINK_MS = 50;
  const unsigned int MIDDLE_PRESS_BLINK_MS = 200;
  const unsigned int SHORT_PRESS_BLINK_MS = 300;

  // ボタンを押している時間を計測する
  unsigned long pressed_ms = millis();
  unsigned long elapsed_time = 0;

  if (M5.BtnA.isPressed()) {
    pressed_ms = millis();

    while (M5.BtnA.isPressed()) {
      elapsed_time = millis() - pressed_ms;

      // LED点灯を操作して、ボタンが押されていることを示す
      if (elapsed_time > LONG_PRESS_MS) {
        led_control::set_blink_ms(LONG_PRESS_BLINK_MS);
      } else if (elapsed_time > MIDDLE_PRESS_MS) {
        led_control::set_blink_ms(MIDDLE_PRESS_BLINK_MS);
      } else if (elapsed_time > SHORT_PRESS_MS) {
        led_control::set_blink_ms(SHORT_PRESS_BLINK_MS);
      }
      M5.update();
      delay(1);
    }
  }

  return elapsed_time;
}


}  // namespace menu_select
