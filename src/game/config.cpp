#include "config.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <utils/logger.hpp>

const auto configPath = "../config/config.json";

Configuration *Configuration::instance = nullptr;
std::mutex Configuration::mtx;

void Configuration::readConfigurationFile()
{
	const auto logger = Logger::getInstance();
	// Create the directory if it doesn't exist.
	if (const auto directory = "../config"; !std::filesystem::exists(directory))
	{
		std::filesystem::create_directory(directory);
	}

	// Read the JSON file with the configuration.
	std::fstream file;
	file.open(configPath, std::ios::in);

	if (!file.is_open())
	{
		logger->warning("Failed to open the configuration file");
		logger->info("Creating a new configuration file...");
		// Create a new configuration file.
		std::fstream newFile;
		newFile.open(configPath, std::ios::out);
		if (!newFile.is_open())
		{
			logger->error("Failed to create the configuration file");
			return;
		}
		newFile.close();
		writeConfigurationFile();
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	// Parse the JSON file.
	nlohmann::json json = nlohmann::json::parse(buffer.str());

	// Set the configuration.
	width = json.value("width", width);
	height = json.value("height", height);
	resizable = json.value("resizable", resizable);
	fullScreen = json.value("fullScreen", fullScreen);
	borderless = json.value("borderless", borderless);
	vsync = json.value("vsync", vsync);
	posX = json.value("posX", posX);
	posY = json.value("posY", posY);

	if (width <= 0 || height <= 0 || width > 1920 * 16 || height > 1080 * 16)
	{
		logger->error("Invalid width or height in the configuration file");
		width = 800;
		height = 600;
		writeConfigurationFile();
	}
}

void Configuration::writeConfigurationFile()
{
	const auto logger = Logger::getInstance();
	// Write the JSON file with the configuration.
	nlohmann::json json;
	json["width"] = width;
	json["height"] = height;
	json["resizable"] = resizable;
	json["fullScreen"] = fullScreen;
	json["borderless"] = borderless;
	json["vsync"] = vsync;
	json["posX"] = posX;
	json["posY"] = posY;

	std::fstream file;
	file.open(configPath, std::ios::out);
	if (!file.is_open())
	{
		logger->error("Failed to open the configuration file");
		return;
	}

	file << json.dump(4);
	file.close();
}
