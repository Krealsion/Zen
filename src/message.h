#pragma once

#include "data_packet.h"

#include <string>
#include <utility>

namespace Zen {
class Message {
public:
  Message(std::string name, DataPacket data) : name(std::move(name)), data(std::move(data)) {};

  const std::string name;
  DataPacket const data;
};
}

