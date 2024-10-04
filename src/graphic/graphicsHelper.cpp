#include <GL/glew.h>

#include "graphicsHelper.hpp"
#include <fstream>

#include <iostream>
#include <vector>

#include <AL/alc.h>

#include "game/config.hpp"

void GraphicsHelper::configureEnvironment()
{
#ifdef _WIN32
	std::cout << "Running on Windows" << std::endl;
#elif __linux__
	if (const std::string sessionType = std::getenv("XDG_SESSION_TYPE"); !sessionType.empty())
	{
		if (std::string(sessionType) == "wayland")
		{
			std::cout << "Running on Linux (Wayland)" << std::endl;
			putenv(const_cast<char *>("GDK_BACKEND=wayland"));
		}
		else if (sessionType == "x11")
		{
			std::cout << "Running on Linux (X11)" << std::endl;
			putenv(const_cast<char *>("GDK_BACKEND=x11"));
		}
		else
		{
			std::cout << "Unknown session type: " << sessionType << std::endl;
		}
	}
	else
	{
		std::cout << "Variable XDG_SESSION_TYPE not defined." << std::endl;
	}
#else
	std::cout << "Not running on known environment, please ask for new implementation if required" << std::endl;
	throw std::runtime_error("Unknown environment");
#endif
}

GraphicsHelper::~GraphicsHelper()
{
	// Cleanup
	glfwTerminate();

	// Close OpenAL
	ALCcontext *context = alcGetCurrentContext();
	ALCdevice *device = alcGetContextsDevice(context);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void *GraphicsHelper::createWindow(const char *title, const GLFWimage *icon)
{
	GLFWwindow *window;
	const auto c = Configuration::getInstance();

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, c->resizable);
	glfwWindowHint(GLFW_DECORATED, !c->borderless);

	if (c->fullScreen)
	{
		GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
		window = glfwCreateWindow(mode->width, mode->height, title, primaryMonitor, nullptr);
	}

	else
	{
		window = glfwCreateWindow(c->width, c->height, title, nullptr, nullptr);
	}

	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowIcon(window, 1, (GLFWimage *)icon);

	glfwSetWindowPos(window, c->posX, c->posY);
	glfwMakeContextCurrent(window);
	glewInit();

	std::cout << "Video information:" << std::endl
			  << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
			  << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
			  << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

	// Set up OpenGL
	setupOpenGL(c->width, c->height);
	// Turn on VSync if requested
	glfwSwapInterval(c->vsync);

	// Initialize OpenAL
	ALCdevice *device = alcOpenDevice(nullptr);
	if (!device)
	{
		std::cerr << "Failed to open OpenAL device" << std::endl;
		throw std::runtime_error("Failed to open OpenAL device");
	}

	ALCcontext *context = alcCreateContext(device, nullptr);
	if (!context)
	{
		std::cerr << "Failed to create OpenAL context" << std::endl;
		throw std::runtime_error("Failed to create OpenAL context");
	}

	if (!alcMakeContextCurrent(context))
	{
		alcDestroyContext(context);
		alcCloseDevice(device);
		std::cerr << "Failed to make OpenAL context current" << std::endl;
		throw std::runtime_error("Failed to make OpenAL context current");
	}

	return window;
}

void GraphicsHelper::setupOpenGL(const int width, const int height)
{
	/*
	float ratio = static_cast<float>(config->getWidth()) / static_cast<float>(config->getHeight());
	static GLfloat lightPosition[] = {0.0f, 0.0f, 0.0f, 1.0f};
	static GLfloat lightAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};
	static GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	static GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	static GLfloat lightAttenuation[] = {0.00002f};
	*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	/* Set the clear color. */
	glClearColor(1, 0.6, 0.8, 0);

	/* Setup our viewport. */
	glViewport(0, 0, width, height);
}
