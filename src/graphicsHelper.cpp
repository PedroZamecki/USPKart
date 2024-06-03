#include "graphicsHelper.hpp"

Configuration* graphicsConfig;

void setupOpenGL() {
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glClearColor(1.0f, 0.7f, 0.8f, 1.0f);
}

GLFWwindow* createWindow(const char* title, Configuration *config)
{
    graphicsConfig = config;

    unsigned int width = config->getWidth();
    unsigned int height = config->getHeight();
    bool resizable = config->isResizable(); 
    bool fullScreen = config->isFullScreen();
    bool borderless = config->isBorderless();

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // Configure the OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Configure the color channels
    glfwWindowHint(GLFW_RED_BITS, 5);
    glfwWindowHint(GLFW_GREEN_BITS, 5);
    glfwWindowHint(GLFW_BLUE_BITS, 5);

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    if (!width || !height) {
        // Get the monitor information
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        config->setWidth(glfwGetVideoMode(monitor)->width);
        config->setHeight(glfwGetVideoMode(monitor)->height);
    }

    // Create the window
    GLFWwindow* window = glfwCreateWindow(width, height, title, fullScreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    // Load icon
    int iconWidth, iconHeight;
    unsigned char* icon = SOIL_load_image("assets/icon.png", &iconWidth, &iconHeight, 0, SOIL_LOAD_RGBA);
    if (icon) {
		GLFWimage *image = new GLFWimage();
		image->width = iconWidth;
		image->height = iconHeight;
		image->pixels = icon;
		glfwSetWindowIcon(window, 1, image);
		SOIL_free_image_data(icon);
	} else {
        std::cerr << "Failed to load icon" << std::endl;
    }

    glfwMakeContextCurrent(window);

    if (resizable) {
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            glfwMakeContextCurrent(window);
            glViewport(0, 0, width, height);
            // Update the configuration
            graphicsConfig->setWidth(width);
            graphicsConfig->setHeight(height);
            graphicsConfig->writeConfigurationFile();
            });
    }
    else {
        glfwSetWindowSizeLimits(window, width, height, width, height);
    }

    if (borderless || fullScreen) {
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    }

    std::cout << "Video information" << std::endl
        << "GLFW version: " << glfwGetVersionString() << std::endl
        << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
        << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
        << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

    glEnable(GL_TEXTURE_2D);

    return window;
}

void manageWindow(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();

        manageInputs(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

void generateShaders(GLuint& programID, const char* vertexShader, const char* fragmentShader) {
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile the vertex shader
    glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
    glCompileShader(vertexShaderID);

    // Check the vertex shader
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
		std::cerr << &vertexShaderErrorMessage[0] << std::endl;
	}

    // Compile the fragment shader
	glShaderSource(fragmentShaderID, 1, &fragmentShader, nullptr);
	glCompileShader(fragmentShaderID);

	// Check the fragment shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
		std::cerr << &fragmentShaderErrorMessage[0] << std::endl;
	}

	// Link the program
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
		std::cerr << &programErrorMessage[0] << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

    glUseProgram(programID);
}

static void manageInputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        std::cout << "Left key pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        std::cout << "Right key pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "Up key pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "Down key pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        std::cout << "Space key pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        std::cout << "Enter key pressed" << std::endl;
    }
}