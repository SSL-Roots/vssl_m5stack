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

#include "command_receiver.h"
#include "led_control.hpp"
#include "robot_information.hpp"
#include "wifi_utils.h"

CommandReceiver g_receiver;

const IPAddress IP(192,168,11,20);
const IPAddress SUBNET(255,255,255,0);
constexpr unsigned int PORT = 10003;
constexpr unsigned int I2C_CLOCK = 400000;
constexpr unsigned char SLAVE_ADDR = 0x54;

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

bool write_robot_info(RobotInformations & robot_info) {
  const int DATA_SIZE = 7;
  unsigned char send_data[DATA_SIZE];
  robot_info.makeCommunicateData();
  robot_info.getCommunicateData(send_data);

  Wire1.beginTransmission(SLAVE_ADDR);
  Wire1.write(send_data, DATA_SIZE);
  
  if(Wire1.endTransmission() == 0) {
    return true;
  } else {
    return false;
  }
}

RobotInformations g_robot_info;
void write_robot_info_task(void * arg) {
  while(true) {
    if (!write_robot_info(g_robot_info)) {
      M5_LOGI("Failed to write command.");
    }
    ets_delay_us(1000);
  }
}

void setup() {
  // 内部I2Cの通信を停止する
  // 参考: https://x.com/lovyan03/status/1627264113805242370?s=20
  auto config = M5.config();
  config.internal_imu = false;
  config.internal_rtc = false;
  M5.begin(config);
  M5.In_I2C.release();
  M5.Ex_I2C.release();
  Wire.end();

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

  Wire1.begin(M5.In_I2C.getSDA(), M5.In_I2C.getSCL(), I2C_CLOCK);

  led_control::init_gpio();
  xTaskCreateUniversal(led_control::led_control_task, "led_control_task", 4096, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(write_robot_info_task, "write_robot_info_task", 4096, NULL, 2, NULL, CONFIG_ARDUINO_RUNNING_CORE);

  M5_LOGI("Hello, world!");
}


void loop() {
  M5.update();

  if(M5.BtnA.isPressed()) {
    led_control::g_led_mode = led_control::LED_MODE::CONTINUOUS;
  } else if(M5.BtnB.isPressed()) {
    led_control::g_led_mode = led_control::LED_MODE::BLINK_1HZ;
  } else if(M5.BtnC.isPressed()) {
    led_control::g_led_mode = led_control::LED_MODE::OFF;
  }

  g_robot_info = receive_command();
}
