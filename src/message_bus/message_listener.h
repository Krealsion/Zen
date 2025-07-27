#pragma once

#include "message.h"
#include "message_bus.h"

#include <memory>

namespace Zen {
class MessageListener {
public:
  MessageListener() {
    message_queue = MessageBus::get_message_queue();
  }

   std::shared_ptr<Message> poll_messages() {
    if (message_queue->empty()) return {nullptr};
    std::shared_ptr<Message> m = message_queue->front();
    message_queue->pop();
    return m;
  }

private:
  std::queue<std::shared_ptr<Message>>* message_queue;
};
}
