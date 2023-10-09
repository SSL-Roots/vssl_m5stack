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

#ifndef FLASH_CONFIG_HPP
#define FLASH_CONFIG_HPP

namespace flash_config {

bool initialize(void);

unsigned int get_robot_id(void);
bool set_robot_id(const unsigned int robot_id);
unsigned int get_mode_number(void);
bool set_mode_number(const unsigned int number);


}  // namespace flash_config

#endif  // FLASH_CONFIG_HPP
