#include "graphicsHelper.hpp"

#include <graphic/drawingHelper.hpp>
#include <iostream>
#include <sstream>

#include <AL/alc.h>
#include <glm/glm.hpp>

std::string readShader(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error reading shader: " << filename << std::endl;
        throw std::runtime_error("Failed to open shader file.");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(GLenum const type, const std::string &source)
{
    const GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl
                  << infoLog << std::endl;
        glDeleteShader(shader); // Guarantee that the shader is deleted in case of failure
        throw std::runtime_error("Shader compilation failed");
    }

    return shader;
}

GLuint createShaderProgram(const std::vector<GLuint> &shaders)
{
    const GLuint program = glCreateProgram();
    for (const auto shader : shaders)
    {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED" << std::endl
                  << infoLog << std::endl;
        glDeleteProgram(program); // Guarantee that the shader is deleted in case of failure
        throw std::runtime_error("Program linking failed");
    }

    return program;
}

GLuint loadShaders()
{
    const std::string vertexShaderSource = readShader("shaders/shader.vs");
    const std::string fragmentShaderSource = readShader("shaders/shader.fs");

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    const GLuint shaderProgram = createShaderProgram({vertexShader, fragmentShader});

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    return shaderProgram;
}

void GraphicsHelper::configureEnvironment()
{
#ifdef _WIN32
    session = "windows";
    std::cout << "Running on Windows" << std::endl;
#elif __linux__
    if (const std::string sessionType = std::getenv("XDG_SESSION_TYPE"); !sessionType.empty())
    {
        session = sessionType;
        if (std::string(session) == "wayland")
        {
            std::cout << "Running on Linux (Wayland)" << std::endl;
            putenv(const_cast<char *>("GDK_BACKEND=wayland"));
        }
        else if (session == "x11")
        {
            std::cout << "Running on Linux (X11)" << std::endl;
            putenv(const_cast<char *>("GDK_BACKEND=x11"));
        }
        else
        {
            std::cout << "Unknown session type: " << session << std::endl;
        }
    }
    else
    {
        std::cout << "Variable XDG_SESSION_TYPE not defined." << std::endl;
    }
#else
    session = "unknown";
    std::cout << "Not running on known environment, please ask for new implementation if required" << std::endl;
    throw std::runtime_error("Unknown environment");
#endif
}

GraphicsHelper::GraphicsHelper(Data *data): config(new Configuration)
{
    this->data = data;
}

GraphicsHelper::~GraphicsHelper()
{
    delete data;
    delete cam;
    delete rm;
}

GLFWwindow *GraphicsHelper::createWindow(const char *title, Configuration *configuration)
{
    config = configuration;
    const unsigned int width = config->getWidth();
    const unsigned int height = config->getHeight();
    const bool resizable = config->isResizable();
    const bool fullScreen = config->isFullScreen();
    const bool borderless = config->isBorderless();

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    if (resizable)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    else
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (borderless)
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    if (fullScreen)
    {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        window = glfwCreateWindow(mode->width, mode->height, title, primaryMonitor, nullptr);
    }
    else
    {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }


    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    std::cout << "Video information:" << std::endl
              << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
              << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
              << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

    // Set up OpenGL
    setupOpenGL();

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

    // Load audio file

    // Load textures
    rm = new ResourceManager();
    rm->loadTexture("assets/textures/null.png", 1, 1, "null");
    rm->loadTexture("assets/textures/kart/SNES_Donut_Plains_1.png", 1024, 1024, "track");
    rm->loadTexture("assets/textures/kart/background.png", 256, 1024, "background");
    rm->loadTexture("assets/textures/IME/ime_usp.png", 209, 668, "ime_usp");
    rm->loadTexture("assets/textures/IME/fluffy_4.png", 378, 378,"fluffy");
    rm->loadTexture("assets/textures/wheel_cap.png", 512, 512, "wheel_cap");

    ch = new ControlsHandler(window);
    return window;
}

void GraphicsHelper::setupOpenGL() const
{
    glfwSetWindowSizeCallback(
        window,
        [](GLFWwindow *window, int const width, int const height)
        {
            glViewport(0, 0, width, height);
            std::cout << "Window resized to " << width << "x" << height << std::endl;
        }
    );
    /*
    float ratio = static_cast<float>(config->getWidth()) / static_cast<float>(config->getHeight());
    static GLfloat lightPosition[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static GLfloat lightAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    static GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    static GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    static GLfloat lightAttenuation[] = {0.00002f};
    */
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    /* Set the clear color. */
    glClearColor(1, 0.6, 0.8, 0);

    /* Setup our viewport. */
    glViewport(0, 0, static_cast<int>(config->getWidth()), static_cast<int>(config->getHeight()));
}

void GraphicsHelper::calculateFPS()
{
    frameCount++;
    currentTime = time(nullptr);
    deltaTime = static_cast<float>(currentTime - startTime) / 1000.0f;
    if (deltaTime >= 1.0f)
    {
        fps = frameCount;
        frameCount = 0;
        startTime = currentTime;
    }
}

void GraphicsHelper::manageWindow()
{
    startTime = time(nullptr);

    // Set up the shaders
    shaderProgram = loadShaders();

    while (!glfwWindowShouldClose(window))
    {
        calculateFPS();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        drawWindow(config->getHeight(), config->getWidth(), cam, shaderProgram, deltaTime, rm);

        drawInterface(config->getHeight(), config->getWidth(), cam, shaderProgram, rm->getTexture("track"));

        glfwSwapBuffers(window);
    }
    stop();
}

void GraphicsHelper::generateShaders(GLuint &programID, const char *vertexShader, const char *fragmentShader)
{
    // Create the shaders
    const GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile the vertex shader
    glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
    glCompileShader(vertexShaderID);

    // Check the vertex shader
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, vertexShaderErrorMessage.data());
        std::cerr << vertexShaderErrorMessage.data() << std::endl;
    }

    // Compile the fragment shader
    glShaderSource(fragmentShaderID, 1, &fragmentShader, nullptr);
    glCompileShader(fragmentShaderID);

    // Check the fragment shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, fragmentShaderErrorMessage.data());
        std::cerr << fragmentShaderErrorMessage.data() << std::endl;
    }

    // Link the program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr, programErrorMessage.data());
        std::cerr << programErrorMessage.data() << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void GraphicsHelper::stop() const
{
    // Cleanup
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    // Close OpenAL
    alcMakeContextCurrent(nullptr);
    alcCloseDevice(alcGetContextsDevice(alcGetCurrentContext()));
}
