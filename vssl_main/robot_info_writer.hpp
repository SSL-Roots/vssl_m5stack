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

#ifndef ROBOT_INFO_WRITER_HPP
#define ROBOT_INFO_WRITER_HPP

#include "robot_information.hpp"

namespace robot_info_writer {

extern RobotInformations g_robot_info;

void write_robot_info_task(void * arg);

}  // namespace robot_info_writer

#endif  // ROBOT_INFO_WRITER_HPP
