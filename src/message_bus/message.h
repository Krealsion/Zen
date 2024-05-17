#pragma once

#include "data_packet.h"

#include <string>
#include <utility>

namespace Zen {
class Message {
public:
  // TODO make data be able to be attached to a message directly, integrate Data packet into message
  Message(std::string name, DataPacket data) : name(std::move(name)), data(std::move(data)) {};

  DataPacket* get_data() {
    return const_cast<DataPacket*>(&data);
  }

  const std::string name;
  DataPacket const data;
};
}

