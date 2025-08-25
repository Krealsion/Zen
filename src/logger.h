#pragma once

#include <string>
#include <fstream>
#include <mutex>

namespace Zen {

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
  static void log(LogLevel level, const std::string& msg);

private:
  static std::ofstream log_file;
  static std::mutex log_mutex;
};

}  // namespace Zen
