#include "graphicsHelper.hpp"

Configuration* graphicsConfig;

void setupOpenGL() {
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glClearColor(1.0f, 0.7f, 0.8f, 1.0f);
}

SDL_Window* createWindow(const char* title, Configuration *config)
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

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    // GLenum glewError = glewInit(); WE DON´T USE THIS ANYMORE
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(glewError) << std::endl;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
    }                

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(window, &info) == SDL_TRUE) {
        switch (info.subsystem) {
            case SDL_SYSWM_X11:
                std::cout << "Running on X11" << std::endl;
                break;
            case SDL_SYSWM_WINDOWS:
                std::cout << "Running on Windows" << std::endl;
                break;
            case SDL_SYSWM_WAYLAND:
                std::cout << "Running on Wayland" << std::endl;
                break;
            default:
                std::cout << "Running on an unknown system" << std::endl;
                break;
        }
    } else {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
    }

    // Set up OpenGL
    setupOpenGL();

    std::cout   << "Video information" << std::endl
                << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
                << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
                << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

    glEnable(GL_TEXTURE_2D);

    return window;
}

void manageWindow(SDL_Window* window) {
    SDL_bool done = SDL_FALSE;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
        SDL_GL_SwapWindow(window);
    }
}

void generateShaders(GLuint& programID, const char* vertexShader, const char* fragmentShader) {
}

static void manageInputs(SDL_Window* window) {
}