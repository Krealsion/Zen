#pragma once

#include "message.h"

#include <memory>
#include <queue>

namespace Zen {
class MessageBus {
public:
  static std::vector<std::queue<std::shared_ptr<Message>>*> message_listeners;
  static MessageBus* _singleton;

  std::vector<std::shared_ptr<Message>> message_queue; // Keep track of history(?)
  std::vector<std::tuple<std::string, std::function<void(const DataPacket&)>>> message_hooks; // tuple<message_id, callback>

  static MessageBus get_singleton() {
    if (_singleton == nullptr) {
      _singleton = new MessageBus();
    }
    return *_singleton;
  }
  
  static void message_hook(const std::string& message_id, std::function<void(const DataPacket&)> callback) {
    get_singleton().message_hooks.emplace_back(message_id, std::move(callback));
  }

  static std::queue<std::shared_ptr<Message>>* get_message_queue() {
    auto new_queue = new std::queue<std::shared_ptr<Message>>();
    message_listeners.emplace_back(new_queue);
    return new_queue;
  }

  static void broadcast(const Message& message) {
    auto shared_message = std::make_shared<Message>(message);
    for (auto& listener_queue : message_listeners) {
      listener_queue->push(shared_message);
    }
    for (auto& hook : get_singleton().message_hooks) {
      if (std::get<0>(hook) == message.name) {
        std::get<1>(hook)(message.data);
      }
    }
  }
};

std::vector<std::queue<std::shared_ptr<Message>>*> MessageBus::message_listeners = std::vector<std::queue<std::shared_ptr<Message>>*>();
}
