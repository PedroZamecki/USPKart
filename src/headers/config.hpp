#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Configuration {
	private:
		int width;
		int height;
		bool resizable;
		bool fullScreen;
		bool borderless;

	public:
		explicit Configuration(	const int width = 800,
								const int height = 600,
			                    const bool resizable = false,
			                    const bool fullScreen = false,
								const bool borderless = false) : width(width), height(height), resizable(resizable), fullScreen(fullScreen), borderless(borderless) {
			readConfigurationFile();
		}
		~Configuration() {
			writeConfigurationFile();
		}
		int getWidth() const { return width; }
		int getHeight() const { return height; }
		bool isResizable() const { return resizable; }
		bool isFullScreen() const { return fullScreen; }
		bool isBorderless() const { return borderless; }
		void setWidth(const int width) { this->width = width; }
		void setHeight(const int height) { this->height = height; }
		void setResizable(const bool resizable) { this->resizable = resizable; }
		void setFullScreen(const bool fullScreen) { this->fullScreen = fullScreen; }
		void setBorderless(const bool borderless) { this->borderless = borderless; }

		void readConfigurationFile();
		void writeConfigurationFile();
};

#endif