#pragma once

#include "message.h"

#include <memory>
#include <vector>

namespace Zen {
class MessageBus {
public:
  static std::vector<std::vector<std::shared_ptr<Message>>*> message_listeners;

  static std::vector<std::shared_ptr<Message>>* get_message_queue() {
    auto new_queue = new std::vector<std::shared_ptr<Message>>();
    message_listeners.push_back(new_queue);
    return new_queue;
  }

  static void broadcast(const Message& message) {
    auto shared_message = std::make_shared<Message>(message);
    for (auto& listener_queue : Zen::MessageBus::message_listeners) {
      listener_queue->push_back(shared_message);
    }
  }
};
std::vector<std::vector<std::shared_ptr<Message>>*> MessageBus::message_listeners = std::vector<std::vector<std::shared_ptr<Message>>*>();
}
