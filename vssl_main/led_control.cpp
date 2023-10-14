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

#include <FastLED.h>
#include <M5Unified.h>
#include <map>
#include <functional>

#include "led_control.hpp"


namespace led_control {

enum class LED_MODE {
  OFF = 0,
  CONTINUOUS,
  BLINK,
  NUMBER
};

const unsigned int IO_LED = GPIO_NUM_26;
LED_MODE g_led_mode = LED_MODE::OFF;
unsigned int g_blink_ms = 500;
unsigned int g_number = 0;

CRGB g_leds[1];
CRGB g_color = CRGB::Green;

std::map<m5::board_t, std::function<void(void)>> g_led_on_map= {
  {m5::board_t::board_M5Stack, [](){
    digitalWrite(IO_LED, HIGH);
    }},
  {m5::board_t::board_M5StampS3, [](){
    g_leds[0] = g_color;
    FastLED.show();
    }},
};
std::map<m5::board_t, std::function<void(void)>> g_led_off_map= {
  {m5::board_t::board_M5Stack, [](){
    digitalWrite(IO_LED, LOW);
    }},
  {m5::board_t::board_M5StampS3, [](){
    g_leds[0] = CRGB::Black;
    FastLED.show();
    }},
};


void led_on(void) {
  g_led_on_map[M5.getBoard()]();
}

void led_off(void) {
  g_led_off_map[M5.getBoard()]();
}

void led_continuous(void) {
  led_on();
}

void led_blink(void) {
  led_on();
  delay(g_blink_ms);
  led_off();
  delay(g_blink_ms);
}

void led_number(void) {
  for (unsigned int i = 0; i < g_number; i++) {
    led_on();
    delay(100);
    led_off();
    delay(100);
  }
  delay(1000);
}

void led_control_task(void * arg) {
  std::map<LED_MODE, std::function<void(void)>> led_mode_map = {
    {LED_MODE::OFF, led_off},
    {LED_MODE::CONTINUOUS, led_continuous},
    {LED_MODE::BLINK, led_blink},
    {LED_MODE::NUMBER, led_number},
  };

  if (M5.getBoard() == m5::board_t::board_M5StampS3) {
    FastLED.addLeds<WS2812, GPIO_NUM_21, GRB>(g_leds, 1);
  } else {
    pinMode(IO_LED, OUTPUT);
  }

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
  g_led_mode = LED_MODE::BLINK;
  g_blink_ms = ms;
}

void set_number(const unsigned int number) {
  g_led_mode = LED_MODE::NUMBER;
  g_number = number;
}

void set_color_green(void) {
  g_color = CRGB::Green;
}

void set_color_blue(void) {
  g_color = CRGB::Blue;
}

void set_color_red(void) {
  g_color = CRGB::Red;
}

void set_color_yellow(void) {
  g_color = CRGB::Yellow;
}

void set_color_purple(void) {
  g_color = CRGB::Purple;
}

}  // namespace  led_control
