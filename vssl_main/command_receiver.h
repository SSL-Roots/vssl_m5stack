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

#ifndef COMMAND_RECEIVER_H
#define COMMAND_RECEIVER_H

#include <WiFiUdp.h>

#include "vssl_robot_control.pb.h"

class CommandReceiver {
 public:
  CommandReceiver();
  ~CommandReceiver();
  bool begin(const int port=10001);
  bool receive(void);
  RobotControl get_latest_command(void) const { return latest_command_; }

 private:

  WiFiUDP udp_;
  RobotControl latest_command_;
};

#endif  // COMMAND_RECEIVER_H
