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

#include "command_receiver.h"
#include "flash_config.hpp"
#include "led_control.hpp"
#include "menu_select.hpp"
#include "robocup_core.hpp"
#include "robot_information.hpp"
#include "robot_info_writer.hpp"
#include "wifi_utils.hpp"


namespace robocup_core {

CommandReceiver g_receiver;

RobotInformations make_robot_info() {
  RobotInformations robot_info;

  const long TIMEOUT_MS = 1000;
  static long last_coonect_timestamp = 0;

  if(g_receiver.receive()) {
    // M5_LOGD("Received command!");
    last_coonect_timestamp = millis();
  } else {
    if (millis() - last_coonect_timestamp > TIMEOUT_MS) {
      robot_info.setRobotVelocity(0.0, 0.0, 0.0);
      return robot_info;
    }
  }

  RobotControl command = g_receiver.get_latest_command();

  robot_info.setRobotVelocity(command.move_velocity.forward, command.move_velocity.left, command.move_velocity.angular);

  // チャージフラグは常時オンする必要あり
  robot_info.setChargeFlag(true);
  if (command.kick_speed > 0.0) {
    robot_info.setKickFlag(true);
  } else {
    robot_info.setKickFlag(false);
  }

  robot_info.setDribbleStrength(command.dribbler_speed);

  return robot_info;
}

bool restart_receiver(const unsigned int port) {
  g_receiver.stop();
  return g_receiver.begin(port);
}

void robocup_core_task(void * arg) {
  const IPAddress IP(192,168,1,133);
  const IPAddress SUBNET(255,255,255,0);
  constexpr unsigned int PORT_BASE = 10000;
  const unsigned int MAX_ROBOT_ID = 11;

  led_control::set_blink_ms(100);

  auto robot_id = flash_config::get_robot_id();
  auto port = PORT_BASE + robot_id;

  while(true) {
    led_control::set_color_yellow();
    auto elapsed_time = menu_select::watch_button_press();
    if (elapsed_time > menu_select::SHORT_PRESS_MS) {
      M5_LOGI("Exit.");
      vTaskSuspend(NULL);
    }

    if(!wifi_utils::connect_wifi_via_smart_config(IP, IP, SUBNET)) {
      M5_LOGE("Failed to connect Wi-Fi. Retry.");
      continue;
    }

    if(!g_receiver.begin(port)) {
      M5_LOGI("Failed to begin udp connection. Retry.");
      continue;
    }
    break;
  }

  M5_LOGI("Start robocup core task. IP: %s, PORT: %d, Robot ID: %d", IP.toString().c_str(), port, robot_id);
  led_control::set_number(robot_id);

  while (true) {
    led_control::set_color_green();
    robot_info_writer::g_robot_info = make_robot_info();

    auto elapsed_time = menu_select::watch_button_press();
    if (elapsed_time > menu_select::LONG_PRESS_MS) {
      M5_LOGI("Suspend myself.");
      led_control::turn_off();
      // TODO(ShotaAk): タスクをSuspended状態から復帰させてもWiFiは再接続されない
      wifi_utils::disconnect_wifi();
      g_receiver.stop();
      vTaskSuspend(NULL);

    } else if (elapsed_time > menu_select::MIDDLE_PRESS_MS) {
      M5_LOGI("Please press button long time to suspend myself.");
      if (flash_config::set_robot_id(robot_id)) {
        M5_LOGI("Succeeded to save robot_id: %d", robot_id);
      } else {
        M5_LOGI("Failed to save robot_id: %d", robot_id);
      }
      led_control::set_number(robot_id);

    } else if (elapsed_time > menu_select::SHORT_PRESS_MS) {
      // ロボットIDを変更する
      robot_id++;
      if (robot_id > MAX_ROBOT_ID) {
        robot_id = 0;
      }

      if (!restart_receiver(PORT_BASE + robot_id)) {
        M5_LOGI("Failed to restart udp connection. Suspend myself.");
        wifi_utils::disconnect_wifi();
        vTaskSuspend(NULL);
      }
      M5_LOGI("I am a RoboCupper. robot_id: %d", robot_id);
      led_control::set_number(robot_id);
    }

    delay(1);
  }
}

}  // namespace robocup_core
