#include "uiProvider.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void UIProvider::init(GLFWwindow *window)
{
	this->window = window;

	// Start ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	setupFont(originalFontSize); // Ajuste o tamanho da fonte conforme necessário
}

void UIProvider::setupFont(float size)
{
	ImGuiIO &io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("assets/fonts/toxigenesis bd.otf", size);
	ImGui_ImplOpenGL3_CreateFontsTexture();
}

void UIProvider::resizeFont(float scale)
{
	ImGui::GetIO().FontGlobalScale = scale;
}

void UIProvider::beginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UIProvider::drawText(const std::string &text, float x, float y, float fontSize, ImVec4 textColor, float outlineSize, ImVec4 outlineColor)
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
	ImGui::SetNextWindowSize(ImVec2(config->width - x, config->height - y));
	ImGui::Begin("TextOverlay", nullptr,
				 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);

	float scale = fontSize / originalFontSize;
	resizeFont(scale);

	ImGui::PushFont(font);

	if (outlineSize > 0.0f)
	{
		ImVec2 offsets[] = {
			ImVec2(-outlineSize, -outlineSize), ImVec2(0, -outlineSize), ImVec2(outlineSize, -outlineSize),
			ImVec2(-outlineSize, 0), ImVec2(outlineSize, 0),
			ImVec2(-outlineSize, outlineSize), ImVec2(0, outlineSize), ImVec2(outlineSize, outlineSize)
		};

		for (const auto& offset : offsets)
		{
			ImGui::SetCursorPos(ImVec2(offset.x, offset.y));
			ImGui::PushStyleColor(ImGuiCol_Text, outlineColor);
			ImGui::Text("%s", text.c_str());
			ImGui::PopStyleColor();
		}
	}

	ImGui::SetCursorPos(ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Text, textColor);
	ImGui::Text("%s", text.c_str());
	ImGui::PopStyleColor();

	ImGui::PopFont();
	ImGui::End();
}

void UIProvider::queueText(const std::string &text, float x, float y, float fontSize, ImVec4 textColor, float outlineSize, ImVec4 outlineColor)
{
	textQueue.push_back({text, x, y, fontSize, textColor, outlineColor, outlineSize});
}

void UIProvider::render()
{
	for (const auto &item : textQueue)
	{
		drawText(item.text, item.x, item.y, item.fontSize, item.textColor, item.outlineSize, item.outlineColor);
	}
	textQueue.clear();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIProvider::destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	font = nullptr;
}
