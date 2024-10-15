#include "logger.hpp"

std::unique_ptr<Logger> Logger::instance = nullptr;
std::mutex Logger::mut;
