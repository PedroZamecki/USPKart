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
		explicit Configuration(	int width = 800,
			                       int height = 600,
			                       bool resizable = false,
			                       bool fullScreen = false,
			                       bool borderless = false) : width(width), height(height), resizable(resizable), fullScreen(fullScreen), borderless(borderless) {
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
		void setWidth(int width) { this->width = width; }
		void setHeight(int height) { this->height = height; }
		void setResizable(bool resizable) { this->resizable = resizable; }
		void setFullScreen(bool fullScreen) { this->fullScreen = fullScreen; }
		void setBorderless(bool borderless) { this->borderless = borderless; }

		void readConfigurationFile();
		void writeConfigurationFile();
};

#endif