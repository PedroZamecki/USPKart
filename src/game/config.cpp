#include "config.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

const auto configPath = "../config/config.json";

Configuration *Configuration::instance = nullptr;
std::mutex Configuration::mtx;

void Configuration::readConfigurationFile()
{
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
		std::cerr << "Failed to open the configuration file" << std::endl;
		std::cout << "Creating a new configuration file..." << std::endl;
		// Create a new configuration file.
		std::fstream newFile;
		newFile.open(configPath, std::ios::out);
		if (!newFile.is_open())
		{
			std::cerr << "Failed to create the configuration file" << std::endl;
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
		std::cerr << "Invalid width or height in the configuration file" << std::endl;
		width = 800;
		height = 600;
		writeConfigurationFile();
	}
}

void Configuration::writeConfigurationFile()
{
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
		std::cerr << "Failed to open the configuration file" << std::endl;
		return;
	}

	file << json.dump(4);
	file.close();
}
