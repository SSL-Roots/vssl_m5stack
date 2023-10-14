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

#include "battery_check.hpp"


namespace battery_check {

const unsigned int IO_BATTERY_ADC = GPIO_NUM_9;
uint32_t g_battery_voltage_milli_volts = 0;

void battery_check_task(void * arg) {
  pinMode(IO_BATTERY_ADC, INPUT);
  M5_LOGI("Battery Check Task Start!");

  while (true) {
    g_battery_voltage_milli_volts = analogReadMilliVolts(IO_BATTERY_ADC);
    delay(1000);
  }
}

uint32_t milli_volts(void) {
  return g_battery_voltage_milli_volts;
}

}  // namespace  battery_check
