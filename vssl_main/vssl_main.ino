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

void hardware_test_task(void * arg) {

  while (true) {
    delay(100);
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
                       4096, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(robot_info_writer::write_robot_info_task, "write_robot_info_task",
                       4096, NULL, 2, NULL, CONFIG_ARDUINO_RUNNING_CORE);

  M5_LOGI("Hello, world!");
}

void loop() {
  M5.update();

  if(M5.BtnA.isPressed()) {
    led_control::set_continuous();
  } else if(M5.BtnB.isPressed()) {
    led_control::set_blink_ms(500);
  } else if(M5.BtnC.isPressed()) {
    led_control::turn_off();
  }

  robot_info_writer::g_robot_info = receive_command();
}
