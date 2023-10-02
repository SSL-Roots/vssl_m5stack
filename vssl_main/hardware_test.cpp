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
#include <map>
#include <functional>

#include "led_control.hpp"
#include "hardware_test.hpp"
#include "robot_information.hpp"
#include "robot_info_writer.hpp"


namespace hardware_test {

enum class TEST_LIST {
  VX_MIN_MAX = 1,
  VY_MIN_MAX,
  OMEGA_MIN_MAX,
  TEST_NUM
};

void test_vx_min_max(void) {
  const double ACC = 0.5;
  const unsigned int DELTA_T_MS = 1;
  const double MAX_VEL = 2.0;  // TODO(ShotaAk): RobotInfoから最大値を取得する

  RobotInformations robot_info;

  for (double vel = 0.0; vel < MAX_VEL; vel += ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(vel, 0.0, 0.0);
    delay(DELTA_T_MS);
  }
  for (double vel = MAX_VEL; vel > -MAX_VEL; vel -= ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(vel, 0.0, 0.0);
    delay(DELTA_T_MS);
  }
  for (double vel = -MAX_VEL; vel < 0.0; vel += ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(vel, 0.0, 0.0);
    delay(DELTA_T_MS);
  }
  robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, 0.0);
}

void test_vy_min_max(void) {
  const double ACC = 0.5;
  const unsigned int DELTA_T_MS = 1;
  const double MAX_VEL = 2.0;

  RobotInformations robot_info;

  for (double vel = 0.0; vel < MAX_VEL; vel += ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, vel, 0.0);
    delay(DELTA_T_MS);
  }
  for (double vel = MAX_VEL; vel > -MAX_VEL; vel -= ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, vel, 0.0);
    delay(DELTA_T_MS);
  }
  for (double vel = -MAX_VEL; vel < 0.0; vel += ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, vel, 0.0);
    delay(DELTA_T_MS);
  }
  robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, 0.0);
}

void test_omega_min_max(void) {
  const double ACC = 2.0;
  const unsigned int DELTA_T_MS = 1;
  const double MAX_VEL = 6.28;

  RobotInformations robot_info;

  for (double vel = 0.0; vel < MAX_VEL; vel += ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, vel);
    delay(DELTA_T_MS);
  }
  for (double vel = MAX_VEL; vel > -MAX_VEL; vel -= ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, vel);
    delay(DELTA_T_MS);
  }
  for (double vel = -MAX_VEL; vel < 0.0; vel += ACC * DELTA_T_MS / 1000.0) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, vel);
    delay(DELTA_T_MS);
  }
  robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, 0.0);
}

void hardware_test_task(void * arg) {
  std::map<TEST_LIST, std::function<void(void)>> test_map = {
    {TEST_LIST::VX_MIN_MAX, test_vx_min_max},
    {TEST_LIST::VY_MIN_MAX, test_vy_min_max},
    {TEST_LIST::OMEGA_MIN_MAX, test_omega_min_max},
  };

  const unsigned int THRESHOLD_MS_SUSPEND = 5000;
  const unsigned int THRESHOLD_MS_EXECUTE_TEST = 1000;
  const unsigned int THRESHOLD_MS_CHANGE_TEST = 10;
  unsigned int test_index = 1;
  unsigned int pressed_ms = millis();

  led_control::set_number(test_index);

  M5_LOGI("Hardware Test Task Start!");
  while (true) {
    unsigned elapsed_time = 0;
    // ボタンを何ms以上押したかで処理を変更する
    if (M5.BtnA.isPressed()) {
      pressed_ms = millis();

      while (M5.BtnA.isPressed()) {
        elapsed_time = millis() - pressed_ms;

        // LED点灯を操作して、ボタンが押されていることを示す
        if (elapsed_time > THRESHOLD_MS_SUSPEND) {
          led_control::set_blink_ms(50);
        } else if (elapsed_time > THRESHOLD_MS_EXECUTE_TEST) {
          led_control::set_blink_ms(200);
        }
        M5.update();
        delay(1);
      }
    }

    if (elapsed_time > THRESHOLD_MS_SUSPEND) {
      M5_LOGI("Suspend this task.");
      led_control::turn_off();
      vTaskSuspend(NULL);

    } else if (elapsed_time > THRESHOLD_MS_EXECUTE_TEST) {
      M5_LOGI("Test index: %d Start!", test_index);
      test_map[static_cast<TEST_LIST>(test_index)]();

    } else if (elapsed_time > THRESHOLD_MS_CHANGE_TEST) {
      test_index++;
      if (test_index >= static_cast<unsigned int>(TEST_LIST::TEST_NUM)) {
        test_index = 1;
      }
      M5_LOGI("Present test index: %d", test_index);
    }
    led_control::set_number(test_index);
    delay(100);
  }
}

}  // namespace hardware_test
