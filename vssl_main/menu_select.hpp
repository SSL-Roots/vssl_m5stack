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

#ifndef MENU_SELECT_HPP
#define MENU_SELECT_HPP

namespace menu_select {

extern const unsigned int LONG_PRESS_MS;
extern const unsigned int MIDDLE_PRESS_MS;
extern const unsigned int SHORT_PRESS_MS;

unsigned long watch_button_press(void);

}  // namespace menu_select

#endif  // MENU_SELECT_HPP
