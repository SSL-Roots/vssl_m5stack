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

#ifndef LED_CONTROL_HPP
#define LED_CONTROL_HPP

#include <map>
#include <functional>

namespace led_control {

void led_control_task(void * arg);

void turn_off();
void set_continuous(void);
void set_blink_ms(const unsigned int ms);
void set_number(const unsigned int number);

}  // namespace led_control

#endif  // LED_CONTROL_HPP
