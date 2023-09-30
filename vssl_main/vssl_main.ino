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

#include <Wire.h>
#include <M5Stack.h>

#include "command_receiver.h"
#include "robot_information.hpp"
#include "wifi_utils.h"

CommandReceiver g_receiver;
constexpr int I2C_SDA = 21;
constexpr int I2C_SCL = 22;
const IPAddress IP(192,168,11,20);
const IPAddress SUBNET(255,255,255,0);
constexpr int PORT = 10003;

void setup() {
  M5.begin();
  Serial.begin(115200);

  if(!connect_wifi_via_smart_config(IP, IP, SUBNET)) {
    // Failed to connect Wi-Fi. Reset.
    ESP.restart();
  }

  if(!g_receiver.begin(PORT)) {
    // Failed to begin udp connection. Reset.
    ESP.restart();
  }

  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("VSSL Robot Start!");
}

void loop() {
  M5.update();

  if(g_receiver.receive()) {
    // Received command!
  }

  RobotControl command = g_receiver.get_latest_command();

  RobotInformations robot_info;
  robot_info.setRobotVelocity(command.move_velocity.forward, command.move_velocity.left, command.move_velocity.angular);

  // チャージフラグは常時オンする必要あり
  robot_info.setChargeFlag(true);
  if (command.kick_speed > 0.0) {
    robot_info.setKickFlag(true);
  } else {
    robot_info.setKickFlag(false);
  }

  const int DATA_SIZE = 7;
  unsigned char send_data[DATA_SIZE];
  robot_info.makeCommunicateData();
  robot_info.getCommunicateData(send_data);

  const unsigned char SLAVE_ADDR = 0x54;
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(send_data, DATA_SIZE);
  
  if(Wire.endTransmission() == 0) {
    // Sent command!
  } else {
    // Failed to send command!
  }
}
