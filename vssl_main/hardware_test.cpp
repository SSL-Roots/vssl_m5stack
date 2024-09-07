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

#include "hardware_test.hpp"
#include "led_control.hpp"
#include "menu_select.hpp"
#include "robot_information.hpp"
#include "robot_info_writer.hpp"


namespace hardware_test {

void test_vx_min_max(void) {
  const double ACC = 0.5;
  const unsigned int DELTA_T_MS = 1;
  const double MAX_VEL = 2.0;  // TODO(ShotaAk): RobotInfoから最大値を取得する

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

void test_omega_2pi_10sec(void) {
  const double TIME_MSEC = 10.0 * 1000;
  const unsigned int DELTA_T_MS = 1;
  const double OMEGA = 6.28;

  // 10秒間同じ速度で回転する
  for (double time = 0.0; time < TIME_MSEC; time += DELTA_T_MS) {
    robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, OMEGA);
    delay(DELTA_T_MS);
  }
  robot_info_writer::g_robot_info.setRobotVelocity(0.0, 0.0, 0.0);
}

void test_kick(void) {
  // n回連続でキックする
  const unsigned int WAIT_CHARGING_MS = 5000;
  const unsigned int WAIT_KICKING_MS = 3000;
  const unsigned int KICK_NUM = 3;

  robot_info_writer::g_robot_info.setChargeFlag(true);
  for (int i = 0; i < KICK_NUM; i++) {
    M5_LOGI("Charging.");
    robot_info_writer::g_robot_info.setKickFlag(false);
    delay(WAIT_CHARGING_MS);

    M5_LOGI("Kick!");
    robot_info_writer::g_robot_info.setKickFlag(true);
    delay(WAIT_KICKING_MS);
  }

  robot_info_writer::g_robot_info.setKickFlag(false);
  robot_info_writer::g_robot_info.setChargeFlag(false);
}

void test_dribble(void) {
  const unsigned int DRIBBLE_MS = 3000;
  robot_info_writer::g_robot_info.setDribbleStrength(0.4);
  M5_LOGI("Dribble strength: 0.4");
  delay(DRIBBLE_MS);
  robot_info_writer::g_robot_info.setDribbleStrength(0.7);
  M5_LOGI("Dribble strength: 0.7");
  delay(DRIBBLE_MS);
  robot_info_writer::g_robot_info.setDribbleStrength(1.0);
  M5_LOGI("Dribble strength: 1.0");
  delay(DRIBBLE_MS);
  robot_info_writer::g_robot_info.setDribbleStrength(0.0);
  M5_LOGI("Dribble strength: 0.0");
}

enum class TEST_LIST {
  VX_MIN_MAX = 1,
  VY_MIN_MAX,
  OMEGA_MIN_MAX,
  OMEGA_2PI_10SEC,
  TEST_NUM
};


void hardware_test_task(void * arg) {
  std::map<TEST_LIST, std::function<void(void)>> test_map = {
    {TEST_LIST::VX_MIN_MAX, test_vx_min_max},
    {TEST_LIST::VY_MIN_MAX, test_vy_min_max},
    {TEST_LIST::OMEGA_MIN_MAX, test_omega_min_max},
    {TEST_LIST::OMEGA_2PI_10SEC, test_omega_2pi_10sec},
  };

  unsigned int test_index = 1;

  led_control::set_number(test_index);

  M5_LOGI("Hardware Test Task Start!");
  while (true) {
    led_control::set_color_blue();
    auto elapsed_time = menu_select::watch_button_press();

    if (elapsed_time > menu_select::LONG_PRESS_MS) {
      M5_LOGI("Suspend this task.");
      led_control::turn_off();
      vTaskSuspend(NULL);

    } else if (elapsed_time > menu_select::MIDDLE_PRESS_MS) {
      M5_LOGI("Test index: %d Start!", test_index);
      test_map[static_cast<TEST_LIST>(test_index)]();

    } else if (elapsed_time > menu_select::SHORT_PRESS_MS) {
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
