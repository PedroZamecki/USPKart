#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <mutex>

class Configuration
{
	static Configuration *instance;
	static std::mutex mtx;

	explicit Configuration(const int width = 800, const int height = 600, const bool resizable = false,
						   const bool fullScreen = false, const bool borderless = false, const bool vsync = false,
						   const int posX = 0, const int posY = 0) :
		width(width), height(height), resizable(resizable), fullScreen(fullScreen), borderless(borderless),
		vsync(vsync), posX(posX), posY(posY)
	{
		readConfigurationFile();
	}
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

	int width;
	int height;
	bool resizable;
	bool fullScreen;
	bool borderless;
	bool vsync;
	int posX;
	int posY;

	void readConfigurationFile();
	void writeConfigurationFile();
};

#endif
