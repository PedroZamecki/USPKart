#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <sys/time.h>
#include <time.h>

#define SQUARE(x) ((x) * (x))
#define ROUND(x) ((x < 0) ? (int)(x - 0.5) : (int)(x + 0.5))
#define SIGN(x) ((x >= 0) ? 1 : -1)

#define MAX_Z 6 // 4
#define MAX_Y 30
#define MAX_X 9 // 7

#define PI 3.142857

#define EPS 0.00001

enum animationType
{
  A_Normal,
  A_Pushing,
  A_Walking,
  A_Pulling,
  A_Hanging,
  A_HangingLeft,
  A_HangingRight,
  A_Seated
};

enum skinType
{
  SKIN_None = 0,
  SKIN_Professor,
  SKIN_Last
};

struct GameData
{
  // screen size:
  int width;
  int height;
  GLuint null_tex;
  GLuint ime_tex;
  GLuint fluffy_tex;
  GLuint wheel_cap_tex;
  GLuint track_tex;
  // Player position:
  float steering_angle;
  float p_angle;
  float px;
  float py;
  float pz;
  int p_dirx;
  int p_dirz;
  float p_vel;
  enum animationType anim;
  enum skinType skin;
  // Move destination position:
  float dest_x;
  float dest_y;
  float dest_z;
  // Camera position:
  float camTarget_x;
  float camTarget_y;
  float camTarget_z;
  float camTarget_dist;
  float yaw;
  float pitch;
  // Camera velocity:
  float c_vel;
};

SDL_Surface *load_image(char *filename)
{
  // The image that's loaded
  SDL_Surface *loadedImage = NULL;
  // The optimized image that will be used
  // SDL_Surface* optimizedImage = NULL;
  // Load the image using SDL_image
  loadedImage = IMG_Load(filename);

  if (loadedImage == NULL)
  {
    printf("Error: Load Image %s\n", filename);
    exit(1);
  }
  // If the image loaded
  // if( loadedImage != NULL ) {
  // Create an optimized image
  // optimizedImage = SDL_DisplayFormat( loadedImage );
  // Free the old image
  // SDL_FreeSurface( loadedImage );
  //}
  // Return the optimized image
  return loadedImage; // optimizedImage;
}

void crossproduct(float A[3], float B[3], float C[3])
{
  C[0] = A[1] * B[2] - A[2] * B[1];
  C[1] = A[2] * B[0] - A[0] * B[2];
  C[2] = A[0] * B[1] - A[1] * B[0];
}

float magnitude(float A[3])
{
  return sqrtf(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
}

static void quit_game(int code)
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  SDL_Quit();

  /* Exit program. */
  exit(code);
}

void left_direction(int *dirx, int *dirz)
{
  if (*dirx == 1 && *dirz == 0)
  {
    *dirx = 0;
    *dirz = -1;
  }
  else if (*dirx == -1 && *dirz == 0)
  {
    *dirx = 0;
    *dirz = 1;
  }
  else if (*dirx == 0 && *dirz == 1)
  {
    *dirx = 1;
    *dirz = 0;
  }
  else if (*dirx == 0 && *dirz == -1)
  {
    *dirx = -1;
    *dirz = 0;
  }
}

void right_direction(int *dirx, int *dirz)
{
  if (*dirx == 1 && *dirz == 0)
  {
    *dirx = 0;
    *dirz = 1;
  }
  else if (*dirx == -1 && *dirz == 0)
  {
    *dirx = 0;
    *dirz = -1;
  }
  else if (*dirx == 0 && *dirz == 1)
  {
    *dirx = -1;
    *dirz = 0;
  }
  else if (*dirx == 0 && *dirz == -1)
  {
    *dirx = 1;
    *dirz = 0;
  }
}

void rotate_clockwise(struct GameData *GD)
{
  if (GD->p_dirx == 1 && GD->p_dirz == 0)
  {
    GD->p_dirx = 0;
    GD->p_dirz = 1;
  }
  else if (GD->p_dirx == -1 && GD->p_dirz == 0)
  {
    GD->p_dirx = 0;
    GD->p_dirz = -1;
  }
  else if (GD->p_dirx == 0 && GD->p_dirz == 1)
  {
    GD->p_dirx = -1;
    GD->p_dirz = 0;
  }
  else if (GD->p_dirx == 0 && GD->p_dirz == -1)
  {
    GD->p_dirx = 1;
    GD->p_dirz = 0;
  }
}

void rotate_counter_clockwise(struct GameData *GD)
{
  if (GD->p_dirx == 1 && GD->p_dirz == 0)
  {
    GD->p_dirx = 0;
    GD->p_dirz = -1;
  }
  else if (GD->p_dirx == -1 && GD->p_dirz == 0)
  {
    GD->p_dirx = 0;
    GD->p_dirz = 1;
  }
  else if (GD->p_dirx == 0 && GD->p_dirz == 1)
  {
    GD->p_dirx = 1;
    GD->p_dirz = 0;
  }
  else if (GD->p_dirx == 0 && GD->p_dirz == -1)
  {
    GD->p_dirx = -1;
    GD->p_dirz = 0;
  }
}

void update_player(struct GameData *GD, float deltatime)
{
  float d = 60.0;
  float D = GD->p_vel * deltatime;
  float cos_angle;
  float sin_angle;
  float l = 1.25 * d;
  cos_angle = cosf(GD->p_angle * (PI / 180.0));
  sin_angle = sinf(GD->p_angle * (PI / 180.0));
  GD->px += sin_angle * D;
  GD->pz += cos_angle * D;
  printf("D*d: %f\n", D * d);
  printf("l: %f\n", l);
  printf("dtheta: %f\n", (180.0 / PI) * ((D * d) / l) * tanf(GD->steering_angle * (PI / 180.0)));
  GD->p_angle += (180.0 / PI) * ((D * d) / l) * tanf(GD->steering_angle * (PI / 180.0));
}

static void handle_key(SDL_KeyboardEvent *key, struct GameData *GD, bool down)
{
  static bool hold_ctrl = false;
  switch (key->keysym.sym)
  {
  case SDLK_ESCAPE:
    if (down)
      quit_game(0);
    break;
  case SDLK_SPACE:
    break;
  case SDLK_LEFT:
    if (down)
    {
      GD->steering_angle += 1.0;
      // move_player_left(GD);
    }
    break;
  case SDLK_RIGHT:
    if (down)
    {
      GD->steering_angle -= 1.0;
      // move_player_right(GD);
    }
    break;
  case SDLK_DOWN:
    if (down)
    {
      GD->p_vel -= 0.5;
      // move_player_back(GD, hold_ctrl);
    }
    break;
  case SDLK_UP:
    if (down)
    {
      GD->p_vel += 0.5;
      // move_player_front(GD, hold_ctrl);
    }
    break;
  case SDLK_s:
    if (down)
    {
      GD->skin = (enum skinType)((((int)GD->skin) + 1) % ((int)SKIN_Last));
    }
    break;
  case SDLK_z:
    if (down)
      GD->yaw -= 1.0;
    break;
  case SDLK_x:
    if (down)
      GD->yaw += 1.0;
    break;
  case SDLK_LCTRL: // Left Ctrl
    if (down)
      hold_ctrl = true;
    else
      hold_ctrl = false;
    break;
  case SDLK_RALT: // Right Alt
    break;
  case SDLK_0:
    GD->anim = A_Normal;
    break;
  case SDLK_1:
    GD->anim = A_Pushing;
    break;
  case SDLK_2:
    GD->anim = A_Walking;
    break;
  case SDLK_3:
    GD->anim = A_Pulling;
    break;
  case SDLK_4:
    GD->anim = A_Hanging;
    break;
  case SDLK_5:
    GD->anim = A_HangingRight;
    break;
  case SDLK_6:
    GD->anim = A_HangingLeft;
    break;
  case SDLK_7:
    GD->anim = A_Seated;
    break;
  default:
    break;
  }
}

void ResizeWindow(int width, int height,
                  SDL_Window *Window,
                  struct GameData *GD)
{
  float Sratio, Wratio;
  int w, h, x, y;

  Sratio = (float)GD->width / (float)GD->height;
  Wratio = (float)width / (float)height;

  if (Sratio > Wratio)
  {
    w = width;
    h = ROUND(width / Sratio);
    x = 0;
    y = height / 2 - h / 2;
    glViewport(x, y, w, h);
  }
  else
  {
    w = ROUND(height * Sratio);
    h = height;
    x = width / 2 - w / 2;
    y = 0;
    glViewport(x, y, w, h);
  }
  // glViewport( 0, 0, width, height );
}

static void process_events(SDL_Window *Window,
                           struct GameData *GD)
{
  /* Our SDL event placeholder. */
  SDL_Event event;

  /* Grab all the events off the queue. */
  while (SDL_PollEvent(&event))
  {

    switch (event.type)
    {
    case SDL_KEYDOWN:
      /* Handle key presses. */
      handle_key(&event.key, GD, true);
      break;
    case SDL_KEYUP:
      handle_key(&event.key, GD, false);
      break;
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        // printf("MESSAGE:Resizing window...\n");
        ResizeWindow(event.window.data1, event.window.data2,
                     Window, GD);
      }
      break;
    case SDL_QUIT:
      /* Handle quit requests (like Ctrl-c). */
      quit_game(0);
      break;
    }
  }
}

void inverte_vetor(int L[4][2])
{
  int N = 4, tmp, i, j;
  for (i = 0; i < N / 2; i++)
  {
    for (j = 0; j < 2; j++)
    {
      tmp = L[i][j];
      L[i][j] = L[N - 1 - i][j];
      L[N - 1 - i][j] = tmp;
    }
  }
}

void SetInitialView()
{
  /*
  glLoadIdentity( );
  glTranslatef( 0.0, 0.0, -300.0);
  glRotatef(35.0, 1.0, 0.0, 0.0 );
  glRotatef(theta_y, 0.0, 1.0, 0.0 );
  */
}

#include "drawing_primitives.cpp"
#include "drawing.cpp"

void update_camera(struct GameData *GD,
                   float deltatime)
{
  // y direction (vertical):
  if (fabs(GD->py - GD->camTarget_y) <= GD->c_vel * deltatime)
    GD->camTarget_y = GD->py;
  else if (GD->py < GD->camTarget_y)
    GD->camTarget_y -= GD->c_vel * deltatime;
  else
    GD->camTarget_y += GD->c_vel * deltatime;

  // x direction:
  float tolerance = 1.0;
  if (fabs(GD->px - GD->camTarget_x) > tolerance)
  {
    if (GD->px < GD->camTarget_x)
      GD->camTarget_x -= GD->c_vel * deltatime;
    else
      GD->camTarget_x += GD->c_vel * deltatime;
  }

  // z direction:
  if (fabs(GD->pz - GD->camTarget_z) > tolerance)
  {
    if (GD->pz < GD->camTarget_z)
      GD->camTarget_z -= GD->c_vel * deltatime;
    else
      GD->camTarget_z += GD->c_vel * deltatime;
  }
}

void draw_screen(SDL_Window *Window,
                 struct GameData *GD,
                 unsigned int shaderProgram,
                 float deltatime)
{
  static enum animationType last_anim = A_Normal;
  float d = 60.0;
  int px, py, pz, i, j, k, b, f;
  float di, dj, dk;

  // Camera movement:
  update_camera(GD, deltatime);

  // Clear the color and depth buffers.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //--------------------------------------------------

  glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                          (float)GD->width / (float)GD->height,
                                          1.0f, 1024.0f);

  float dist = GD->camTarget_dist * cos(glm::radians(GD->pitch));

  float cam_z = d * GD->camTarget_z - sin(glm::radians(GD->yaw)) * dist;
  float cam_x = d * GD->camTarget_x - cos(glm::radians(GD->yaw)) * dist;
  float cam_y = d * GD->camTarget_y - sin(glm::radians(GD->pitch)) * GD->camTarget_dist;

  glm::vec3 cameraPos = glm::vec3(cam_x, cam_y, cam_z);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 view;

  glm::vec3 direction;
  direction.x = cos(glm::radians(GD->yaw)) * cos(glm::radians(GD->pitch));
  direction.y = sin(glm::radians(GD->pitch));
  direction.z = sin(glm::radians(GD->yaw)) * cos(glm::radians(GD->pitch));
  glm::vec3 cameraFront = glm::normalize(direction);

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);

  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

  unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
  //--------------------------------------------------
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram, "normal");
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  //--------------------------------------------------
  glm::vec3 lightPos = cameraPos;
  int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
  glUniform3f(lightPosLocation, lightPos[0], lightPos[1], lightPos[2]);

  int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
  glUniform3f(viewPosLocation, cameraPos[0], cameraPos[1], cameraPos[2]);

  int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
  glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

  //--------------------------------------------------

  // DrawRedDragon();
  SetInitialView();

  DrawTrack(GD, shaderProgram, deltatime);

  bool restart = last_anim != GD->anim;
  last_anim = GD->anim;
  if (GD->anim == A_Normal)
    DrawFluffy(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_Walking)
    DrawFluffy_walk(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_Pushing)
    DrawFluffy_push(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_Pulling)
    DrawFluffy_pull(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_Hanging)
    DrawFluffy_hang(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_HangingRight)
    DrawFluffy_hangright(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_HangingLeft)
    DrawFluffy_hangleft(GD, shaderProgram, deltatime, restart);
  else if (GD->anim == A_Seated)
  {
    DrawFluffy_seated(GD, shaderProgram, deltatime, restart);
    DrawKart(GD, shaderProgram, deltatime, restart);
  }
  else
    DrawFluffy(GD, shaderProgram, deltatime, restart);

  glEnable(GL_TEXTURE_2D);
  SDL_GL_SwapWindow(Window);
}

char *ReadShader(char *filename)
{
  char *Source;
  FILE *fp;
  int k = 0, n = 0, sz;
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("Error reading shader\n");
    exit(1);
  }
  fseek(fp, 0L, SEEK_END);
  sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  Source = (char *)malloc(sizeof(char) * sz * 2);

  while (fscanf(fp, "%[^\n]%n\n", &Source[n], &k) != EOF)
  {
    Source[n + k] = '\n';
    k++;
    Source[n + k] = '\0';
    // printf("k: %3d|", k);
    // printf("%s", &Source[n]);
    n += k;
  }
  fclose(fp);
  return Source;
}

static void setup_opengl(int width, int height)
{
  float ratio = (float)width / (float)height;
  static GLfloat light_pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
  static GLfloat light_amb[] = {0.7f, 0.7f, 0.7f, 1.0f};
  static GLfloat light_dif[] = {0.8f, 0.8f, 0.8f, 1.0f};
  static GLfloat light_spe[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_att[] = {0.00002f};
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  /* Our shading model--Gouraud (smooth). */
  // glShadeModel( GL_SMOOTH );

  /*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos );
  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);
  glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light_att);
  */

  /* Culling. */
  // CullFace( GL_BACK );
  // FrontFace( GL_CCW );
  // Enable( GL_CULL_FACE );

  /* Set the clear color. */
  glClearColor(0.5, 0.5, 0, 0);

  /* Setup our viewport. */
  glViewport(0, 0, width, height);

  /*
   * Change to the projection matrix and set
   * our viewing volume.
   */
  /*
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  */

  // gluPerspective( 60.0, ratio, 1.0, 1024.0 );
  // glFrustum(-width/2, width/2, -height/2, height/2, 200.0, 1024.0);
}

int main(int argc, char *argv[])
{
  // int Level[MAX_Z][MAX_Y][MAX_X];
  struct GameData GD;

  /* Dimensions of our window. */
  // int width =  1366; //1067; //512; //640;
  // int height = 768; //600; //288; //480;
  /* Flags we will pass into SDL_SetVideoMode. */
  int flags = 0;
  // GLuint tex[28];
  SDL_Window *Window;
  SDL_Surface *img = NULL;
  char filename[512];
  int i;

  /* First, initialize SDL's video subsystem. */
  /*
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    fprintf( stderr, "Video initialization failed: %s\n",
    SDL_GetError( ) );
    quit_game( 1 );
    }
  */

  GD.width = 1366; // 1067; //512; //640;
  GD.height = 768; // 600; //288; //480;

  Window = SDL_CreateWindow("VW1",
                            0, 0,
                            GD.width, GD.height,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  assert(Window);
  SDL_GLContext Context = SDL_GL_CreateContext(Window);
  // SDL_SetWindowFullscreen(Window, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

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

  /*
   * At this point, we should have a properly setup
   * double-buffered window for use with OpenGL.
   */
  gladLoadGLLoader(SDL_GL_GetProcAddress);
  printf("Vendor:   %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("Version:  %s\n", glGetString(GL_VERSION));

  setup_opengl(GD.width, GD.height);

  glEnable(GL_TEXTURE_2D);
  // glGenTextures(5+7+6+10, tex);

  glGenTextures(1, &GD.null_tex);
  glGenTextures(1, &GD.ime_tex);
  glGenTextures(1, &GD.fluffy_tex);
  glGenTextures(1, &GD.wheel_cap_tex);
  glGenTextures(1, &GD.track_tex);

  //-------------------------------------------
  glBindTexture(GL_TEXTURE_2D, GD.null_tex);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR);
  img = load_image((char *)"./textures/null.png");
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGB, 1, 1, 0,
               GL_RGB, GL_UNSIGNED_BYTE,
               img->pixels);
  // glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(img);
  //-------------------------------------------
  glBindTexture(GL_TEXTURE_2D, GD.ime_tex);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR);
  img = load_image((char *)"./textures/IME/ime_usp.png");
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGB, 668, 209, 0,
               GL_RGB, GL_UNSIGNED_BYTE,
               img->pixels);
  // glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(img);
  //-------------------------------------------
  glBindTexture(GL_TEXTURE_2D, GD.fluffy_tex);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR);
  img = load_image((char *)"./textures/IME/fluffy_4.png");
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGB, 378, 378, 0,
               GL_RGB, GL_UNSIGNED_BYTE,
               img->pixels);
  // glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(img);
  //-------------------------------------------
  glBindTexture(GL_TEXTURE_2D, GD.wheel_cap_tex);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR);
  img = load_image((char *)"./textures/wheel_cap.png");
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGB, 512, 512, 0,
               GL_RGB, GL_UNSIGNED_BYTE,
               img->pixels);
  // glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(img);
  //-------------------------------------------
  glBindTexture(GL_TEXTURE_2D, GD.track_tex);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR);
  img = load_image((char *)"./textures/kart/track.png");
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGB, 1024, 1024, 0,
               GL_RGB, GL_UNSIGNED_BYTE,
               img->pixels);
  // glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(img);
  //-------------------------------------------
  GD.anim = A_Seated; // A_Normal;
  GD.skin = SKIN_None;
  GD.steering_angle = 0.0;
  GD.p_angle = 0.0;
  GD.p_vel = 0.0;

  GD.pz = 5;
  GD.py = 1;
  GD.px = 2;
  GD.camTarget_z = GD.pz;
  GD.camTarget_y = GD.py;
  GD.camTarget_x = GD.px;
  GD.camTarget_dist = 300;
  GD.p_dirx = 0;
  GD.p_dirz = 1;
  GD.c_vel = 1.0;

  GD.yaw = -90.0 - 30.0;
  GD.pitch = -35.0;

  //================VERTEX_SHADER=====================
  char *Source = NULL;

  Source = ReadShader((char *)"shader.vs");
  const char *vertexShaderSource = Source;
  /*
  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  */
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  if (Source != NULL)
    free(Source);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    exit(1);
  }
  //===============FRAGMENT_SHADER================
  Source = ReadShader((char *)"shader.fs");

  const char *fragmentShaderSource = Source;

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  if (Source != NULL)
    free(Source);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    exit(1);
  }
  //===============Shader program========================
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
    exit(1);
  }

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  //=====================================================
  /*
   * Now we want to begin our normal app process--
   * an event loop with a lot of redrawing.
   */
  float deltatime = 0.00001;
  clock_t lastFrame, currentFrame;
  lastFrame = clock();

  while (1)
  {
    /* Process incoming events. */
    process_events(Window, &GD);

    // update_tower(&GD, deltatime);

    update_player(&GD, deltatime);

    /* Draw the screen. */
    draw_screen(Window, &GD, shaderProgram, deltatime);

    currentFrame = clock();
    deltatime = ((float)(currentFrame - lastFrame)) / CLOCKS_PER_SEC;
    lastFrame = currentFrame;
    // printf("Time: %f sec\n", deltatime);
  }

  return 0;
}
