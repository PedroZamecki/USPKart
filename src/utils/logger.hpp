#pragma once

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>

enum class LogLevel
{
	TRACE = 0,
	INFO = 1,
	WARNING = 2,
	ERROR = 3,
	CRITICAL = 4
};

class Logger
{
	static std::mutex mut;
	static std::unique_ptr<Logger> instance;
	std::ofstream logFile;
	LogLevel logLevel = LogLevel::INFO;

	Logger()
	{
		const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::ostringstream oss;
		oss << "logs/" << std::put_time(std::localtime(&now), "%Y%m%d.log");
		const auto logName = oss.str();

		// Test if the dir /logs exists, if not, create it
		if (!std::filesystem::exists("logs"))
		{
			std::filesystem::create_directory("logs");
		}

		logFile.open(logName, std::ios_base::app);
		if (!logFile.is_open())
		{
			error("Failed to open log file: " + logName);
			throw std::runtime_error("Failed to open log file: " + logName);
		}

		logFile << "-------------------------------------------------------------------------------------\n"
				<< std::put_time(std::localtime(&now), "Starting %Y-%m-%d %H:%M:%S\n") << std::endl;
	}

	void log(LogLevel level, const std::string &message)
	{
		if (level < logLevel)
			return;

		const char *levelStr = nullptr;
		switch (level)
		{
		case LogLevel::TRACE:
			levelStr = "[TRACE]";
			break;
		case LogLevel::INFO:
			levelStr = "[INFO]";
			break;
		case LogLevel::WARNING:
			levelStr = "[WARNING]";
			break;
		case LogLevel::ERROR:
			levelStr = "[ERROR]";
			break;
		case LogLevel::CRITICAL:
			levelStr = "[CRITICAL]";
			break;
		default:
			levelStr = "[?]";
			break;
		}

		std::ostream &out = (level == LogLevel::ERROR) ? std::cerr : std::cout;
		out << levelStr << " " << message << std::endl;

		if (logFile.is_open())
		{
			auto now = std::chrono::system_clock::now();
			auto now_c = std::chrono::system_clock::to_time_t(now);
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
			logFile << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
					<< std::setw(3) << ms.count() << " - " << levelStr << " " << message << std::endl;
		}
	}

public:
	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;

	static Logger *getInstance()
	{
		std::lock_guard<std::mutex> lock(mut);
		if (instance == nullptr)
		{
			instance = std::unique_ptr<Logger>(new Logger());
		}
		return instance.get();
	}

	void setLogLevel(LogLevel level)
	{
		std::lock_guard<std::mutex> lock(mut);
		logLevel = level;
	}

	void critical(const std::string &message) { log(LogLevel::CRITICAL, message); }

	void error(const std::string &message) { log(LogLevel::ERROR, message); }

	void warning(const std::string &message) { log(LogLevel::WARNING, message); }

	void info(const std::string &message) { log(LogLevel::INFO, message); }

	void trace(const std::string &message) { log(LogLevel::TRACE, message); }
};
