#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <cstdarg>

#define critical(message) log(LogLevel::CRITICAL, message, __FILE__, __LINE__)
#define error(message) log(LogLevel::ERROR, message, __FILE__, __LINE__)
#define warning(message) log(LogLevel::WARNING, message, __FILE__, __LINE__)
#define info(message) log(LogLevel::INFO, message, __FILE__, __LINE__)
#define debug(message) log(LogLevel::DEBUG, message, __FILE__, __LINE__)

#define fcritical(format, ...) flog(LogLevel::CRITICAL, __FILE__, __LINE__, format, __VA_ARGS__)
#define ferror(format, ...) flog(LogLevel::ERROR, __FILE__, __LINE__, format, __VA_ARGS__)
#define fwarning(format, ...) flog(LogLevel::WARNING, __FILE__, __LINE__, format, __VA_ARGS__)
#define finfo(format, ...) flog(LogLevel::INFO, __FILE__, __LINE__, format, __VA_ARGS__)
#define fdebug(format, ...) flog(LogLevel::DEBUG, __FILE__, __LINE__, format, __VA_ARGS__)

#define glLog()                                                                                                        \
	if (const auto err = glGetError())                                                                                 \
	Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +                                    \
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
	static Logger *instance;
	std::ofstream logFile;
	LogLevel logLevel = LogLevel::INFO;
	std::queue<std::string> logQueue;
	std::thread logThread;
	std::condition_variable cv;
	bool exitFlag = false;

	void processQueue()
	{
		while (true)
		{
			std::unique_lock lock(mut);
			cv.wait(lock, [this] { return !logQueue.empty() || exitFlag; });

			if (exitFlag && logQueue.empty())
				break;

			auto logMessage = std::move(logQueue.front());
			logQueue.pop();
			lock.unlock();

			logFile << logMessage << std::endl;
			
		}
	}

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

		logThread = std::thread(&Logger::processQueue, this);
	}

	~Logger()
	{
		{
			std::lock_guard lock(mut);
			exitFlag = true;
		}
		cv.notify_all();
		logThread.join();
	}

public:
	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;

	static Logger *getInstance()
	{
		if (instance == nullptr)
		{
			std::lock_guard lock(mut);
			if (instance == nullptr)
				return (instance = new Logger());
		}
		return instance;
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
			std::ostringstream oss;
			oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
				<< std::setw(3) << ms.count() << " - " << levelStr << local << message;

			{
				std::lock_guard lock(mut);
				logQueue.push(oss.str());
			}
			cv.notify_one();
		}
	}

	void flog(const LogLevel level, const char *file, const int line, const char *format, ...)
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

		va_list args;
		va_start(args, format);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), format, args);
		va_end(args);

		out << levelStr << local << buffer << std::endl;

		if (logFile.is_open())
		{
			const auto now = std::chrono::system_clock::now();
			const auto now_c = std::chrono::system_clock::to_time_t(now);
			const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
			std::ostringstream oss;
			oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
				<< std::setw(3) << ms.count() << " - " << levelStr << local << buffer;

			{
				std::lock_guard lock(mut);
				logQueue.push(oss.str());
			}
			cv.notify_one();
		}
	}
};

#endif // LOGGER_HPP
