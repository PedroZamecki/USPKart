#include "logger.hpp"

Logger* Logger::instance = nullptr;
std::mutex Logger::mut;
