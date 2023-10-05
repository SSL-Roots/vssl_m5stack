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

#include <math.h>

#include "robot_information.hpp"

RobotInformations::RobotInformations(void) {}

void RobotInformations::setRobotVelocity(float v_x, float v_y, float v_omega) {
  vx = v_x;
  vy = v_y;
  omega = v_omega;
}
float RobotInformations::getVelocity_x(void) { return vx; }
float RobotInformations::getVelocity_y(void) { return vy; }
float RobotInformations::getVelocity_omega(void) { return omega; }

void RobotInformations::setChargeFlag(bool chg) { is_charge = chg; }
void RobotInformations::setChargeFlag(void) { is_charge = true; }
void RobotInformations::clrChargeFlag(void) { is_charge = false; }
bool RobotInformations::getChargeFlag(void) { return is_charge; }

void RobotInformations::setKickFlag(bool kick) { is_kick = kick; }
void RobotInformations::setKickFlag(void) { is_kick = true; }
void RobotInformations::clrKickFlag(void) { is_kick = false; }
bool RobotInformations::getKickFlag(void) { return is_kick; }

void RobotInformations::setBallPosition(bool ball) { is_ball = ball; }
bool RobotInformations::isBallKeep(void) { return is_ball; }

void RobotInformations::setDribbleStrength(float dribbler) {
  dribble = dribbler;
}
float RobotInformations::getDribbleStrength(void) { return dribble; }

//通信関係
void RobotInformations::makeCommunicateData(void) {
  // I2C送信用データ生成
  setVelocityCommand();
  setRobotCommand();
  setChecksum();
}
void RobotInformations::getCommunicateData(unsigned char *send_data_add) {
  for (int i = 0; i < 7; i++) {
    send_data_add[i] = m_send_data[i];
  }
}
void RobotInformations::setVelocityCommand(void) {
  if (vx >= MAX_VELOCITY) {
    m_send_data[2] = (unsigned char)(roundf(0.99 * (1 << 7)));
  } else if (vx <= -MAX_VELOCITY) {
    m_send_data[2] = (unsigned char)(roundf(-0.99) * (1 << 7));
  } else {
    m_send_data[2] = (unsigned char)(roundf(vx / MAX_VELOCITY * (1 << 7)));
  }

  if (vy >= MAX_VELOCITY) {
    m_send_data[3] = (unsigned char)(roundf(0.99 * (1 << 7)));
  } else if (vx <= -MAX_VELOCITY) {
    m_send_data[3] = (unsigned char)(roundf(-0.99) * (1 << 7));
  } else {
    m_send_data[3] = (unsigned char)(roundf(vy / MAX_VELOCITY * (1 << 7)));
  }

  if (omega >= MAX_ROTATION_VELOCITY) {
    m_send_data[4] = (unsigned char)(roundf((0.99) * (1 << 7)));
  } else if (omega <= -MAX_ROTATION_VELOCITY) {
    m_send_data[4] = (unsigned char)(roundf((-0.99) * (1 << 7)));
  } else {
    m_send_data[4] =
        (unsigned char)(roundf(omega / MAX_ROTATION_VELOCITY * (1 << 7)));
  }
}
void RobotInformations::setRobotCommand(void) {
  send_commands.KICKER_CHARGE = is_charge;
  send_commands.KICKER_RELEASE = is_kick;

  if ((int)dribble / 0.3 > 3) {
    send_commands.DRIBBLER_SPEED = 3;
  }
  send_commands.DRIBBLER_SPEED = (int)dribble / 0.3;
  m_send_data[5] = send_commands.ROBOT_COMMAND;
}

void RobotInformations::setKickCommand(void) { m_send_data[5] = 0x01; }
void RobotInformations::setChecksum(void) {
  int i = 0;
  m_send_data[6] = m_send_data[0];
  for (i = 1; i < 6; i++) {
    m_send_data[6] ^= m_send_data[i];
  }
}
