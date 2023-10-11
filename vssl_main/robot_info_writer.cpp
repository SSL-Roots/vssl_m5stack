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
#include <Wire.h>

#include "robot_info_writer.hpp"

namespace robot_info_writer {

constexpr unsigned char SLAVE_ADDR = 0x54;
constexpr unsigned int I2C_CLOCK = 100000;
RobotInformations g_robot_info;

bool write_robot_info(RobotInformations & robot_info) {
  const int DATA_SIZE = 7;
  unsigned char send_data[DATA_SIZE];
  robot_info.makeCommunicateData();
  robot_info.getCommunicateData(send_data);

  Wire1.beginTransmission(SLAVE_ADDR);
  Wire1.write(send_data, DATA_SIZE);

  if (Wire1.endTransmission() == 0) {
    return true;
  } else {
    return false;
  }
}

void write_robot_info_task(void * arg) {
  M5.In_I2C.release();
  M5.Ex_I2C.release();
  Wire.end();

  Wire1.begin(M5.Ex_I2C.getSDA(), M5.Ex_I2C.getSCL(), I2C_CLOCK);

  const unsigned int MAX_FAILURE_COUNT = 10;
  const unsigned int NORMAL_PERIOD_MS = 1;
  const unsigned int ERROR_PERIOD_MS = 1000;

  unsigned int failure_count = 0;
  unsigned int task_period_ms = NORMAL_PERIOD_MS;

  M5_LOGI("Write Robot Info Task Start!");

  while (true) {
    if (!write_robot_info(g_robot_info)) {
      M5_LOGE("Failed to write robot info.");

      if (failure_count > MAX_FAILURE_COUNT) {
        task_period_ms = ERROR_PERIOD_MS;
      } else {
        failure_count++;
      }
    } else {
      failure_count = 0;
      task_period_ms = NORMAL_PERIOD_MS;
    }

    delay(task_period_ms);
  }
}

}  // namespace robot_info_writer
