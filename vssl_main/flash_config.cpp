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
#include <Preferences.h>
#include "flash_config.hpp"

namespace flash_config {

Preferences preferences;

bool initialize(void) {
  return preferences.begin("config", false);
}

unsigned int get_robot_id(void) {
  return preferences.getUInt("robot_id", 0);
}

bool set_robot_id(const unsigned int robot_id) {
  return preferences.putUInt("robot_id", robot_id);
}

unsigned int get_mode_number(void) {
  return preferences.getUInt("mode_number", 0);
}

bool set_mode_number(const unsigned int number) {
  return preferences.putUInt("mode_number", number);
}

}  // namespace flash_config
