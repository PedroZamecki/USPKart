#include "graphicsHelper.hpp"

Configuration* graphicsConfig;
std::string session;

TTF_Font* font;
SDL_Color textColor = {255, 255, 255, 255}; // White

Uint32 startTime = 0, currentTime = 0;
float deltaTime = 0;
int frameCount = 0, fps = 0;

void GraphicsHelper::configureEnvironment() {
	#ifdef _WIN32
    session = "windows";
    std::cout << "Running on Windows" << std::endl;
    // Configurações específicas do Windows aqui
    #elif __linux__
    const char* sessionType = std::getenv("XDG_SESSION_TYPE");
    if (sessionType != nullptr) {
        session = sessionType;
        if (session == "wayland") {
            std::cout << "Running on Linux (Wayland)" << std::endl;    
            putenv(const_cast<char*>("GDK_BACKEND=wayland"));
            putenv(const_cast<char*>("SDL_VIDEODRIVER=wayland"));
        } else if (session == "x11") {
            std::cout << "Running on Linux (X11)" << std::endl;
            putenv(const_cast<char*>("GDK_BACKEND=x11"));
            putenv(const_cast<char*>("SDL_VIDEODRIVER=x11"));
        } else {
            std::cout << "Tipo de sessão desconhecido: " << session << std::endl;
        }
    } else {
        std::cout << "Variável XDG_SESSION_TYPE não definida." << std::endl;
    }
	#else
    session = "unknown";
	std::cout << "Not running on known environment, please ask for new implementation if required" << std::endl;
    exit(404);
    #endif
}

SDL_Window* GraphicsHelper::createWindow(const char* title, Configuration *config)
{
    graphicsConfig = config;
    unsigned int width = config->getWidth();
    unsigned int height = config->getHeight();
    bool resizable = config->isResizable(); 
    bool fullScreen = config->isFullScreen();
    bool borderless = config->isBorderless();

    SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL);
    if (resizable) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_RESIZABLE);
    if (fullScreen) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_FULLSCREEN);
    if (borderless) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_BORDERLESS);

    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    assert(window != nullptr);

    // Configure the window
    if (resizable) {        
        SDL_SetWindowMinimumSize(window, 640, 480);
    }
    if (fullScreen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    if (borderless) {
        SDL_SetWindowBordered(window, SDL_FALSE);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

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
    SDL_Surface* icon = SDL_LoadBMP("assets/icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    TTF_Init();
    font = TTF_OpenFont("assets/font/vcr_osd_mono.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    // Set up OpenGL
    setupOpenGL();

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    std::cout   << "Video information:" << std::endl
                << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
                << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
                << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

    glEnable(GL_TEXTURE_2D);

    // Start to play audio from ./assets/audio/test.mp3
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Error: " << Mix_GetError() << std::endl;
        exit(1);
    }
    Mix_Music *music = Mix_LoadMUS("assets/audio/test.mp3");
    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Error: " << Mix_GetError() << std::endl;
    }
        Mix_VolumeMusic(MIX_MAX_VOLUME/5);

    return window;
}

void GraphicsHelper::setupOpenGL() {
    // Set up OpenGL
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        exit(1);
    }

    // Set up the viewport
    glViewport(0, 0, graphicsConfig->getWidth(), graphicsConfig->getHeight());

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)graphicsConfig->getWidth() / (GLfloat)graphicsConfig->getHeight(), 0.1f, 100.0f);

    // Set up the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set up the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);

    // Set up the blending function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GraphicsHelper::calculateFPS() {
    frameCount++;
    currentTime = SDL_GetTicks();
    deltaTime = (currentTime - startTime) / 1000.0f;

    if (deltaTime > 1.0f) {
        fps = frameCount;
        frameCount = 0;
        startTime = currentTime;
    }
}

void GraphicsHelper::displayFPS(SDL_Renderer* renderer) {
    std::string fpsText = "FPS: " + std::to_string(fps);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, fpsText.c_str(), textColor);

    if (textSurface == nullptr) {
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (textTexture == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        return;
    }

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    SDL_Rect renderQuad = {10, 10, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void GraphicsHelper::manageWindow(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_bool done = SDL_FALSE;
    startTime = SDL_GetTicks();
    while (!done) {
        calculateFPS();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }   

            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        graphicsConfig->setWidth(event.window.data1);
                        graphicsConfig->setHeight(event.window.data2);
                        graphicsConfig->writeConfigurationFile();
                        break;
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                done = manageInputs(window, event);
            }
        }
        SDL_RenderClear(renderer);

        displayFPS(renderer);

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
}

void GraphicsHelper::generateShaders(GLuint& programID, const char* vertexShader, const char* fragmentShader) {
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
    if (infoLogLength > 0) {
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
    if (infoLogLength > 0) {
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
    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        std::cerr << &programErrorMessage[0] << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

SDL_bool GraphicsHelper::manageInputs(SDL_Window* window, SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return SDL_TRUE;
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