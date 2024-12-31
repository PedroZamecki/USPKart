#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <string>
#include <vector>
#include <game/config.hpp>

class UIProvider
{
	UIProvider() = default;
	~UIProvider() = default;
	UIProvider(const UIProvider &) = delete;
	UIProvider &operator=(const UIProvider &) = delete;

	GLFWwindow *window = nullptr;
	ImFont *font = nullptr;
	float originalFontSize = 200.0f;
	Configuration *config = Configuration::getInstance(); 

	struct TextItem
	{
		std::string text;
		float x, y, fontSize;
		ImVec4 textColor, outlineColor;
		float outlineSize;
	};

	std::vector<TextItem> textQueue;

public:
	static UIProvider &getInstance()
	{
		static UIProvider instance;
		return instance;
	}

	void init(GLFWwindow *window);
	void beginFrame();
	void drawText(const std::string &text, float x, float y, float fontSize, ImVec4 textColor = ImVec4(0, 0, 0, 1),
				  float outlineSize = 0.0f, ImVec4 outlineColor = ImVec4(1, 1, 1, 1));
	void render();
	void destroy();
	void setupFont(float size);
	void resizeFont(float scale);
	void queueText(const std::string &text, float x, float y, float fontSize, ImVec4 textColor = ImVec4(0, 0, 0, 1),
				   float outlineSize = 0.0f, ImVec4 outlineColor = ImVec4(1, 1, 1, 1));
};
