#pragma once

#include "code_generator.h"
#include "rttr_wrapper.h"
#include "message_bus.h"
#include "logger.h"
#include <string>
#include <iostream>
#include <sstream>

namespace Zen {

class ConsoleInterface {
public:
  void run() {
    std::string input;
    while (std::getline(std::cin, input)) {
      std::istringstream iss(input);
      std::string cmd;
      iss >> cmd;
      if (cmd == "create_method") {
        std::string class_name, method_name, code;
        iss >> class_name >> method_name;
        std::getline(iss, code);
        nlohmann::json options = { {"whitelist", {"gmail.com"}}, {"blacklist", {"spam.com"}} };
        if (CodeGenerator::generate_and_compile(code, class_name, method_name, options)) {
          Logger::log(LogLevel::INFO, "Method created");
          auto config = CodeGenerator::load_config("user_dll_config.json");
          RttrWrapper::register_class(config["class_name"], config["method_name"], [](const std::string& arg) { return ""; });  // Stub; real from DLL
        }
      } else if (cmd == "runcommand") {
        std::string func_call;
        std::getline(iss, func_call);
        size_t paren_pos = func_call.find('(');
        if (paren_pos == std::string::npos) continue;
        std::string func_name = func_call.substr(0, paren_pos);
        std::string arg = func_call.substr(paren_pos + 1, func_call.find(')') - paren_pos - 1);
        if (!RttrWrapper::method_exists(func_name)) {
          Logger::log(LogLevel::ERROR, "Method not found");
          continue;
        }
        DataPacket result = RttrWrapper::invoke_dynamic(func_name, arg);
        // Output parsed (e.g., if "valid" key)
        auto valid = result.get_object<bool>("valid");
        std::cout << (valid ? *valid ? "true" : "false" : "no result") << std::endl;
        auto error = result.get_object<std::string>("error");
        if (error) std::cout << "Error: " << *error << std::endl;
      }
    }
  }
};

}  // namespace Zen
