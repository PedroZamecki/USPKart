#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>

#define critical(message) log(LogLevel::CRITICAL, message, __FILE__, __LINE__)
#define error(message) log(LogLevel::ERROR, message, __FILE__, __LINE__)
#define warning(message) log(LogLevel::WARNING, message, __FILE__, __LINE__)
#define info(message) log(LogLevel::INFO, message, __FILE__, __LINE__)
#define debug(message) log(LogLevel::DEBUG, message, __FILE__, __LINE__)

#define glLog()																											\
	if (const auto err = glGetError())                                                                             \
	Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +       \
								 reinterpret_cast<const char *>(glewGetErrorString(err)))

enum class LogLevel
{
	DEBUG = 0,
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
			log(LogLevel::ERROR, "Failed to open log file: " + logName, __FILE__, __LINE__);
			throw std::runtime_error("Failed to open log file: " + logName);
		}

		logFile << "-------------------------------------------------------------------------------------\n"
				<< std::put_time(std::localtime(&now), "Starting %Y-%m-%d %H:%M:%S\n") << std::endl;
	}

public:
	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;

	static Logger *getInstance()
	{
		std::lock_guard lock(mut);
		if (instance == nullptr)
		{
			instance = std::unique_ptr<Logger>(new Logger());
		}
		return instance.get();
	}

	void setLogLevel(const LogLevel level)
	{
		std::lock_guard lock(mut);
		logLevel = level;
	}

	void log(const LogLevel level, const std::string &message, const char *file, const int line)
	{
		if (level < logLevel)
			return;

		const char *levelStr = nullptr;
		switch (level)
		{
		case LogLevel::DEBUG:
			levelStr = "[DEBUG]";
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

		const auto local =
			(level >= LogLevel::ERROR ? " [" + std::string(file) + ":" + std::to_string(line) + "] " : " ");
		std::ostream &out = (level >= LogLevel::ERROR) ? std::cerr : std::cout;
		out << levelStr << local << message << std::endl;

		if (logFile.is_open())
		{
			const auto now = std::chrono::system_clock::now();
			const auto now_c = std::chrono::system_clock::to_time_t(now);
			const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
			logFile << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
					<< std::setw(3) << ms.count() << " - " << levelStr << local << message << std::endl;
		}
	}
};

#endif // LOGGER_HPP