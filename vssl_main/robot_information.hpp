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

#ifndef ROBOT_INFORMATION_HPP
#define ROBOT_INFORMATION_HPP

//テストモードではMAX値を使わず、設定された値をそのまま送信する
#define MAX_VELOCITY 1.0
#define MAX_ROTATION_VELOCITY 1.0
// #define MAX_VELOCITY 2.0
// #define MAX_ROTATION_VELOCITY 6.28

typedef union {
  struct {
    unsigned char KICKER_CHARGE : 1;
    unsigned char KICKER_RELEASE : 1;
    unsigned char DRIBBLER_SPEED : 2;
    unsigned char : 4;
  };
  char ROBOT_COMMAND;
} __ROBOT_COMMANDbits;

class RobotInformations {
 public:
  RobotInformations(void);
  void setRobotVelocity(float v_x, float v_y, float v_omega);
  float getVelocity_x(void);
  float getVelocity_y(void);
  float getVelocity_omega(void);

  void setChargeFlag(bool chg);
  void setChargeFlag(void);
  void clrChargeFlag(void);
  bool getChargeFlag(void);

  void setKickFlag(bool kick);
  void setKickFlag(void);
  void clrKickFlag(void);
  bool getKickFlag(void);

  void setBallPosition(bool ball);
  bool isBallKeep(void);

  void setDribbleStrength(float dribbler);
  float getDribbleStrength(void);

  void makeCommunicateData(void);  // I2C送信用データ生成
  void getCommunicateData(unsigned char *send_data_add);

 private:
  float vx, vy, omega, dribble;
  bool is_kick, is_charge, is_ball;
  __ROBOT_COMMANDbits send_commands;
  unsigned char m_send_data[7] = {0x05, 0x01, 0, 0, 0, 0, 0};

  void setVelocityCommand(void);
  void setKickCommand(void);
  void setChecksum(void);
  void setRobotCommand(void);
};

#endif  // ROBOT_INFORMATION_HPP
