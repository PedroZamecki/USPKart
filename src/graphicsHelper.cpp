#include <graphicsHelper.hpp>

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

unsigned int compileShader(GLenum type, const std::string &source)
{
    unsigned int shader = glCreateShader(type);
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
        glDeleteShader(shader); // Garante que o shader é deletado em caso de falha
        throw std::runtime_error("Shader compilation failed");
    }

    return shader;
}

unsigned int createShaderProgram(const std::vector<unsigned int> &shaders)
{
    unsigned int program = glCreateProgram();
    for (auto shader : shaders)
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
        glDeleteProgram(program); // Garante que o programa é deletado em caso de falha
        throw std::runtime_error("Program linking failed");
    }

    return program;
}

unsigned int loadShaders()
{
    std::string vertexShaderSource = readShader("shaders/shader.vs");
    std::string fragmentShaderSource = readShader("shaders/shader.fs");

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = createShaderProgram({vertexShader, fragmentShader});

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
// Configurações específicas do Windows aqui
#elif __linux__
    if (const std::string sessionType = std::getenv("XDG_SESSION_TYPE"); !sessionType.empty())
    {
        session = sessionType;
        if (std::string(session) == "wayland")
        {
            std::cout << "Running on Linux (Wayland)" << std::endl;
            putenv(const_cast<char *>("GDK_BACKEND=wayland"));
            putenv(const_cast<char *>("SDL_VIDEODRIVER=wayland"));
        }
        else if (session == "x11")
        {
            std::cout << "Running on Linux (X11)" << std::endl;
            putenv(const_cast<char *>("GDK_BACKEND=x11"));
            putenv(const_cast<char *>("SDL_VIDEODRIVER=x11"));
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
    // Make sure the audio works on Linux
    putenv(const_cast<char *>("SDL_AUDIODRIVER=pulseaudio"));
    putenv(const_cast<char *>("PULSE_RUNTIME_PATH=/run/user/1000/pulse"));
#else
    session = "unknown";
    std::cout << "Not running on known environment, please ask for new implementation if required" << std::endl;
    throw std::runtime_error("Unknown environment");
#endif
}

GraphicsHelper::GraphicsHelper(Data *data): glContext(), config(), shaderProgram(0),
                                            renderer(), window(), session(), rm(), font()
{
    this->data = data;
}

GraphicsHelper::~GraphicsHelper()
{
    delete data;
    delete cam;
    delete rm;
}

SDL_Window *GraphicsHelper::createWindow(const char *title, Configuration *configuration)
{
    config = configuration;
    const unsigned int width = config->getWidth();
    const unsigned int height = config->getHeight();
    const bool resizable = config->isResizable();
    const bool fullScreen = config->isFullScreen();
    const bool borderless = config->isBorderless();

    auto flags = SDL_WINDOW_OPENGL;
    if (resizable)
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_RESIZABLE);
    if (fullScreen)
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_FULLSCREEN);
    if (borderless)
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_BORDERLESS);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(width), static_cast<int>(height), flags);
    assert(window != nullptr);

    // Configure the window
    if (resizable)
    {
        SDL_SetWindowMinimumSize(window, 640, 480);
    }
    if (fullScreen)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    if (borderless)
    {
        SDL_SetWindowBordered(window, SDL_FALSE);
    }

    glContext = SDL_GL_CreateContext(window);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Load icon
    SDL_Surface *icon = SDL_LoadBMP("assets/icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize SDL");
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize TTF");
    }
    font = TTF_OpenFont("assets/font/vcr_osd_mono.ttf", 24);
    if (font == nullptr)
    {
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        throw std::runtime_error("Failed to load font");
    }

    /*
     * At this point, we should have a properly setup
     * double-buffered window for use with OpenGL.
     */
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    std::cout << "Video information:" << std::endl
              << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
              << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
              << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

    // Set up OpenGL
    setupOpenGL();

    // Start to play audio from ./assets/audio/test.mp3
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "Error: " << Mix_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize audio");
    }
    if (const auto music = Mix_LoadMUS("assets/audio/test.mp3"); Mix_PlayMusic(music, -1) == -1)
    {
        std::cerr << "Error: " << Mix_GetError() << std::endl;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);

    SDL_GL_SetSwapInterval(0);

    // Load textures
    rm = new ResourceManager();
    rm->loadTexture("assets/textures/null.png", 1, 1, "null");
    rm->loadTexture("assets/textures/kart/SNES_Donut_Plains_1.png", 1024, 1024, "track");
    rm->loadTexture("assets/textures/kart/background.png", 256, 1024, "background");
    rm->loadTexture("assets/textures/IME/ime_usp.png", 209, 668, "ime_usp");
    rm->loadTexture("assets/textures/IME/fluffy_4.png", 378, 378,"fluffy");
    rm->loadTexture("assets/textures/wheel_cap.png", 512, 512, "wheel_cap");

    return window;
}

void GraphicsHelper::setupOpenGL() const
{
    float ratio = static_cast<float>(config->getWidth()) / static_cast<float>(config->getHeight());
    static GLfloat lightPosition[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static GLfloat lightAmbient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    static GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    static GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    static GLfloat lightAttenuation[] = {0.00002f};
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
    currentTime = SDL_GetTicks();
    deltaTime = static_cast<float>(currentTime - startTime) / 1000.0f;
    if (deltaTime >= 1.0f)
    {
        fps = frameCount;
        frameCount = 0;
        startTime = currentTime;
    }
}

void GraphicsHelper::displayText(std::string message, SDL_Color color, int x, int y, int size) const
{
    // Function to create a rectangle with the text as a texture and render in front of the camera
    SDL_Surface *surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = size;
    rect.h = size;

    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void GraphicsHelper::drawWindow(const Camera *cam, const unsigned int shaderProgram, const float deltaTime) const
{
    static animationType last_anim = normal;
    constexpr float d = 60.0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                            static_cast<float>(config->getWidth()) / static_cast<float>(config->getHeight()),
                                            1.0f, 4024.0f);

    const float dist = cam->targetDist * cos(glm::radians(cam->pitch));

    const float cam_z = d * cam->target.z - sin(glm::radians(cam->yaw)) * dist;
    const float cam_x = d * cam->target.x - cos(glm::radians(cam->yaw)) * dist;
    const float cam_y = d * cam->target.y - sin(glm::radians(cam->pitch)) * cam->targetDist;

    const auto cameraPos = glm::vec3(cam_x, cam_y, cam_z);
    constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    direction.y = sin(glm::radians(cam->pitch));
    direction.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    const auto cameraFront = glm::normalize(direction);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);

    const GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    const GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    auto model = glm::mat4(1.0f);
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    //--------------------------------------------------
    auto normal = glm::mat4(1.0f);
    const GLint normalLoc = glGetUniformLocation(shaderProgram, "normal");
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    //--------------------------------------------------
    glm::vec3 lightPos = cameraPos;
    const GLint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPosLocation, lightPos[0], lightPos[1], lightPos[2]);

    const GLint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3f(viewPosLocation, cameraPos[0], cameraPos[1], cameraPos[2]);

    const GLint lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

    const GLint lightMinLoc = glGetUniformLocation(shaderProgram, "lightMin");
    glUniform3f(lightMinLoc, 0.0, 0.0, 0.0);
    //--------------------------------------------------

    drawTrack({0.0f, 0.0f, 0.0f}, shaderProgram, deltaTime, 0.0f, rm->getTexture("track"));
    drawBackground({0, 0, 0}, shaderProgram, deltaTime, rm->getTexture("background"));

    constexpr bool restart = false;
    last_anim = seated;
    drawFluffy({1, 1, 1}, 45.0, shaderProgram, deltaTime, restart, rm->getTexture("null"), 0);
    drawKart({1, 0.4, 1}, shaderProgram, deltaTime, restart, 45.0, 0.0, rm->getTexture("wheel_cap"), rm->getTexture("null"), rm->getTexture("fluffy"), rm->getTexture("ime_usp"), 0);

    glEnable(GL_TEXTURE_2D);
}

void GraphicsHelper::manageWindow()
{
    SDL_bool done = SDL_FALSE;
    startTime = SDL_GetTicks();

    // Set up the shaders
    shaderProgram = loadShaders();

    while (!done)
    {
        calculateFPS();

        SDL_Event event;
        while (!done && SDL_PollEvent(&event))
        {
            done = manageInputs(event);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Desenhe o triângulo uma vez.
        drawWindow(cam, shaderProgram, deltaTime);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }
    stop();
}

void GraphicsHelper::generateShaders(GLuint &programID, const char *vertexShader, const char *fragmentShader)
{
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile the vertex shader
    glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderID);

    // Check the vertex shader
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        std::cerr << &vertexShaderErrorMessage[0] << std::endl;
    }

    // Compile the fragment shader
    glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
    glCompileShader(fragmentShaderID);

    // Check the fragment shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
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
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        std::cerr << &programErrorMessage[0] << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

SDL_bool GraphicsHelper::manageInputs(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        return SDL_TRUE;
    }

    if (event.type == SDL_WINDOWEVENT)
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
            config->setWidth(event.window.data1);
            config->setHeight(event.window.data2);
            config->writeConfigurationFile();
            break;
        default:
            break;
        }
    }

    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return SDL_TRUE;
                break;
            case SDLK_w:
                cam->targetDist -= 0.1f;
                break;
            case SDLK_s:
                cam->targetDist += 0.1f;
                break;
            case SDLK_a:
                cam->yaw -= 1.0f;
                break;
            case SDLK_d:
                cam->yaw += 1.0f;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        return SDL_FALSE;
    }
    return SDL_FALSE;
}

void GraphicsHelper::stop()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    Mix_CloseAudio();
    Mix_Quit();

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
}