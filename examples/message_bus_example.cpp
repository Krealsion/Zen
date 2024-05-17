#include <memory>
#include "src/message_bus/message.h"
#include "src/message_bus/message_bus.h"
#include "src/message_bus/message_listener.h"
#include <timer.h>
#include <iostream>

using namespace Zen;

class MessageNeedingClass : public MessageListener {
public:
  void update() {
    std::shared_ptr<Message> m;
    while ((m = poll_messages())) {
      if (*(m->data.get_object<bool>("is_hit"))) {
        hits++;
        std::cout << "hits: " << hits << std::endl;
      }
    }
  }
  int hits = 0;
};

int main() {
  MessageNeedingClass mnc;

  auto t = Timer(.5);
  srand(Timer::get_current_time());
  while (true) {
    if (t.is_time()) {
      DataPacket d;
      bool is_hit = rand() % 2 == 0;
      d.attach_object("is_hit", is_hit);
      std::cout << "Broadcasting message with is_hit being " << is_hit << std::endl;
      MessageBus::broadcast(Message("Hit", d));
    }
    mnc.update();
    if (mnc.hits == 10) {
      break;
    }
  }
}
