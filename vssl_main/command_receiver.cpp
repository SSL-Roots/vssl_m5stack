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
#include "pb_encode.h"
#include "pb_decode.h"

CommandReceiver::CommandReceiver() {
}

CommandReceiver::~CommandReceiver() {
  udp_.stop();
}

bool CommandReceiver::begin(const int port) {

  return udp_.begin(port);
}

bool CommandReceiver::receive(void) {
  int packetSize = udp_.parsePacket();
  if (packetSize) {
    M5_LOGD("received packet");
    uint8_t receivedBuffer[256];
    int bytesRead = udp_.read(receivedBuffer, sizeof(receivedBuffer));

    pb_istream_t stream = pb_istream_from_buffer(receivedBuffer, bytesRead);

    RobotControl receivedMessage = RobotControl_init_zero;

    if (pb_decode(&stream, RobotControl_fields, &receivedMessage)) {
      latest_command_ = receivedMessage;
      return true;
    } else {
      M5_LOGD("failed to decode");
      return false;
    }
  }

  return false;
}
