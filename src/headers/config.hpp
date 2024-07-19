#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Configuration {
	private:
		unsigned int width;
		unsigned int height;
		bool resizable;
		bool fullScreen;
		bool borderless;

	public:
		Configuration(	unsigned int width = 800, 
						unsigned int height = 600,
						bool resizable = false,
						bool fullScreen = false,
						bool borderless = false) : width(width), height(height), resizable(resizable), fullScreen(fullScreen), borderless(borderless) {
			readConfigurationFile();
		}
		~Configuration() {
			writeConfigurationFile();
		}
		unsigned int getWidth() const { return width; }
		unsigned int getHeight() const { return height; }
		bool isResizable() const { return resizable; }
		bool isFullScreen() const { return fullScreen; }
		bool isBorderless() const { return borderless; }
		void setWidth(unsigned int width) { this->width = width; }
		void setHeight(unsigned int height) { this->height = height; }
		void setResizable(bool resizable) { this->resizable = resizable; }
		void setFullScreen(bool fullScreen) { this->fullScreen = fullScreen; }
		void setBorderless(bool borderless) { this->borderless = borderless; }

		void readConfigurationFile();
		void writeConfigurationFile();
};

#endif