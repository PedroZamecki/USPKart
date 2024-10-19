#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <mutex>

class Configuration
{
	static Configuration *instance;
	static std::mutex mtx;

	Configuration() { readConfigurationFile(); }
	~Configuration() { writeConfigurationFile(); }

public:
	Configuration(const Configuration &obj) = delete;
	static Configuration *getInstance()
	{
		if (instance == nullptr)
		{
			std::lock_guard lock(mtx);
			if (instance == nullptr)
			{
				instance = new Configuration();
			}
		}
		return instance;
	}

	int width{800};
	int height{600};
	bool resizable{false};
	bool fullScreen{false};
	bool borderless{false};
	bool vsync{true};
	int posX{0};
	int posY{0};

	void readConfigurationFile();
	void writeConfigurationFile();
};

#endif
