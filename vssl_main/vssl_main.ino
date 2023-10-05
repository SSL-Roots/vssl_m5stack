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
#include "led_control.hpp"
#include "menu_select.hpp"
#include "hardware_test.hpp"
#include "robot_information.hpp"
#include "robot_info_writer.hpp"
#include "wifi_utils.h"

CommandReceiver g_receiver;

const IPAddress IP(192,168,11,20);
const IPAddress SUBNET(255,255,255,0);
constexpr unsigned int PORT = 10003;

RobotInformations receive_command() {
  RobotInformations robot_info;

  if(g_receiver.receive()) {
    // M5_LOGD("Received command!");
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

  return robot_info;
}

xTaskHandle g_led_control_task_handle = NULL;
xTaskHandle g_write_robot_info_task_handle = NULL;
xTaskHandle g_hardware_test_task_handle = NULL;
std::vector<xTaskHandle> g_selectable_task_handles;

void task_selct_task(void * arg) {

  unsigned int task_index = 0;

  while (true) {
    delay(100);
    if (g_selectable_task_handles.size() == 0) {
      continue;
    }

    // TODO(ShotaAk)  毎回全タスクを走査するのは無駄である
    bool is_all_task_suspended = true;
    // すべてのタスクがSuspended状態になったら、タスク選択を行う
    for (int i=0; i < g_selectable_task_handles.size(); i++) {
      if (g_selectable_task_handles[i] == NULL) {
        continue;
      }

      if (eTaskGetState(g_selectable_task_handles[i]) != eSuspended) {
        is_all_task_suspended = false;
        break;
      }
    }

    if (!is_all_task_suspended) {
      continue;
    }

    auto elapsed_time = menu_select::watch_button_press();

    if (elapsed_time > menu_select::LONG_PRESS_MS) {
      M5_LOGI("Restart task: %s",
        pcTaskGetName(g_selectable_task_handles[task_index]));
      led_control::turn_off();
      vTaskResume(g_selectable_task_handles[task_index]);

    } else if (elapsed_time > menu_select::MIDDLE_PRESS_MS) {
      M5_LOGI("Please press button long time to select task.");

    } else if (elapsed_time > menu_select::SHORT_PRESS_MS) {
      task_index++;
      if (task_index >= g_selectable_task_handles.size()) {
        task_index = 0;
      }
      M5_LOGI("Present task: %d : %s", 
               task_index, pcTaskGetName(g_selectable_task_handles[task_index]));
    }
    led_control::set_number(task_index + 1);
  }
}

void setup() {
  // 内部I2Cの通信を停止する
  // 参考: https://x.com/lovyan03/status/1627264113805242370?s=20
  auto config = M5.config();
  config.output_power = false;
  config.pmic_button = false;
  config.internal_imu = false;
  config.internal_rtc = false;
  config.internal_spk = false;
  config.internal_mic = false;
  M5.begin(config);

  // ログをカラー表示する
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_INFO);
  M5.Log.setEnableColor(m5::log_target_serial, true);

  if(!connect_wifi_via_smart_config(IP, IP, SUBNET)) {
    M5_LOGI("Failed to connect Wi-Fi. Reset.");
    ESP.restart();
  }

  if(!g_receiver.begin(PORT)) {
    M5_LOGI("Failed to begin udp connection. Reset.");
    ESP.restart();
  }

  xTaskCreateUniversal(led_control::led_control_task, "led_control_task",
                       4096, NULL, 1, &g_led_control_task_handle,
                       CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(robot_info_writer::write_robot_info_task, "write_robot_info_task",
                       4096, NULL, 2, &g_write_robot_info_task_handle,
                       CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(hardware_test::hardware_test_task, "hardware_test_task",
                       4096, NULL, 1, &g_hardware_test_task_handle,
                       CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(task_selct_task, "task_select_task",
                       4096, NULL, 1, NULL,
                       CONFIG_ARDUINO_RUNNING_CORE);

  g_selectable_task_handles.push_back(g_hardware_test_task_handle);

  M5_LOGI("Hello, world!");
}

void loop() {
  M5.update();

  delay(1);
  // robot_info_writer::g_robot_info = receive_command();
}
