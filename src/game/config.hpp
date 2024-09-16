#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

class Configuration
{
public:
	int width;
	int height;
	bool resizable;
	bool fullScreen;
	bool borderless;
	bool vsync;
	int posX;
	int posY;


	explicit Configuration(const int width = 800, const int height = 600, const bool resizable = false,
						   const bool fullScreen = false, const bool borderless = false, const bool vsync = false,
						   const int posX = 0, const int posY = 0) :
		width(width), height(height), resizable(resizable), fullScreen(fullScreen), borderless(borderless),
		vsync(vsync), posX(posX), posY(posY)
	{
		readConfigurationFile();
	}
	~Configuration() { writeConfigurationFile(); }

	void readConfigurationFile();
	void writeConfigurationFile();
};

#endif
