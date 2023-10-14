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

#ifndef BATTERY_CHECK_HPP 
#define BATTERY_CHECK_HPP 

namespace battery_check {

void battery_check_task(void * arg);
uint32_t milli_volts(void);

}  // namespace battery_check

#endif  // BATTERY_CHECK_HPP
