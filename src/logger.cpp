#include "logger.h"
#include <iostream>
#include <ctime>

namespace Zen {

std::ofstream Logger::log_file("zen_log.txt", std::ios::app);
std::mutex Logger::log_mutex;

void Logger::log(LogLevel level, const std::string& msg) {
  std::lock_guard<std::mutex> lock(log_mutex);
  std::time_t now = std::time(nullptr);
  char time_str[100];
  std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  std::string full_msg = "[" + std::string(time_str) + "] " + (level == LogLevel::INFO ? "INFO" : (level == LogLevel::WARNING ? "WARNING" : "ERROR")) + ": " + msg + "\n";
  if (level == LogLevel::INFO || level == LogLevel::WARNING)
    std::cout << full_msg;
  else
    std::cerr << full_msg;
  log_file << full_msg;
  log_file.flush();
}

}  // namespace Zen
