#pragma once

#include <memory>
#include <message.h>
#include <message_bus.h>

namespace Zen {
class MessageListener {
public:
  MessageListener() {
    message_queue = MessageBus::get_message_queue();
  }

   bool poll_messages(std::shared_ptr<Message>& m) {
    if (message_queue->empty()) {
      return false;
    } else {
      m = ((*message_queue)[0]);
      message_queue->erase(message_queue->begin());
      return true;
    }
  }

private:
  std::vector<std::shared_ptr<Message>>* message_queue;
};
}
