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
#include "wifi_utils.h"

CommandReceiver g_receiver;

void setup() {
  M5.begin();

  // ログをカラー表示する
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_INFO);
  M5.Log.setEnableColor(m5::log_target_serial, true);


  const IPAddress ip(192,168,11,20);
  const IPAddress subnet(255,255,255,0);
  if(!connect_wifi_via_smart_config(ip, ip, subnet)) {
    M5_LOGI("Failed to connect Wi-Fi. Reset.");
    ESP.restart();
  }

  if(!g_receiver.begin(10003)) {
    M5_LOGI("Failed to begin udp connection. Reset.");
    ESP.restart();
  }

  M5_LOGI("Hello, world!");
}

void loop() {
  M5.delay(1);
  M5.update();

  if(g_receiver.receive()) {
    M5_LOGI("Received command!");
    RobotControl command = g_receiver.get_latest_command();
    M5_LOGI("vel_forward: %f", command.move_velocity.forward);
    M5_LOGI("vel_left: %f", command.move_velocity.left);
    M5_LOGI("vel_angular: %f", command.move_velocity.angular);
    M5_LOGI("kick_speed: %f", command.kick_speed);
  }

  if (M5.BtnA.wasHold()) {
    M5_LOGI("Button A was Hold!");
  }

  if (M5.BtnA.wasPressed()) {
    M5_LOGI("Button A was pressed!");
  }

  if (M5.BtnA.isHolding()) {
    M5_LOGI("Button A is holding!"); 
    ESP.restart();
  }

}
