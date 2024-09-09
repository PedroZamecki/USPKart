#pragma once

#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <glad/glad.h>

float magnitude(const float A[3]);

unsigned int createSquareY(float sx, float sz,
                             float R, float G, float B);
unsigned int createSquareZ(float sy, float sz,
                             float R, float G, float B);

void drawSquareY(unsigned int VAO,
                   GLuint tex);
void drawSquareZ(unsigned int VAO,
                   GLuint tex);

unsigned int createEllipseSectorZ(float a, float b,
                                     float R, float G, float B,
                                     int angle1, int angle2,
                                     int dangle);

void draw_ellipse_sector_z(unsigned int VAO,
                           int angle1, int angle2,
                           int dangle);

unsigned int create_ellipse_z(float a, float b,
                              float R, float G, float B,
                              int dangle);

void draw_ellipse_z(unsigned int VAO,
                    int dangle);

unsigned int create_rectangular_cuboid(float sx, float sy, float sz,
                                       float R, float G, float B);

void draw_rectangular_cuboid(unsigned int VAO,
                             GLuint tex[6]);

//--------------------------------------

unsigned int create_rounded_rectangular_cuboid(float sx, float sy, float sz,
                                               float radius,
                                               float Rt, float Gt, float Bt,
                                               float Rm, float Gm, float Bm,
                                               float Rb, float Gb, float Bb);

void draw_rounded_rectangular_cuboid(unsigned int VAO,
                                     GLuint tex[7]);

//--------------------------------------

unsigned int create_ellipsoid(float a, float b, float c,
                              float R, float G, float B,
                              int slices);

void draw_ellipsoid(unsigned int VAO,
                    int slices);

//-------------------------------

// lune_z
unsigned int create_ellipsoid_lune_cap_z(float a, float b, float c,
                                         float z_0, float z_1,
                                         int angle1, int angle2,
                                         float R, float G, float B,
                                         int slices);

void draw_ellipsoid_lune_cap_z(unsigned int VAO,
                               int angle1, int angle2,
                               int slices);

//-------------------------------

// lune_z
unsigned int create_ellipsoid_lune_z(float a, float b, float c,
                                     int angle1, int angle2,
                                     float R, float G, float B,
                                     int slices);

void draw_ellipsoid_lune_z(unsigned int VAO,
                           int angle1, int angle2,
                           int slices);
//-----------------------------------------

unsigned int create_ellipsoid_lune_x(float a, float b, float c,
                                     int angle1, int angle2,
                                     float R, float G, float B,
                                     int slices);

void draw_ellipsoid_lune_x(unsigned int VAO,
                           int angle1, int angle2,
                           int slices);

//-----------------------------------------

unsigned int create_ellipsoid_lune_cap_x(float a, float b, float c,
                                         float x_0, float x_1,
                                         int angle1, int angle2,
                                         float R, float G, float B,
                                         int slices);

void draw_ellipsoid_lune_cap_x(unsigned int VAO,
                               int angle1, int angle2,
                               int slices);

//-----------------------------------------

// plane zy (rotation axis = x)
unsigned int create_curved_cylinder_x(float radius0,
                                      float radius1,
                                      float bend_radius,
                                      float bend_ang0,
                                      float bend_ang1,
                                      float R, float G, float B,
                                      int slices,
                                      int delta);

void draw_curved_cylinder_x(unsigned int VAO,
                            int slices,
                            int delta);

//-----------------------------------------

// plane zy (rotation axis = x)
unsigned int create_torus_x(float radius,
                            float bend_radius,
                            float R, float G, float B,
                            int slices,
                            int delta);

void draw_torus_x(unsigned int VAO,
                  int slices,
                  int delta);

//-----------------------------------------

// plane zx (rotation axis = y)
unsigned int create_curved_cylinder_y(float radius0,
                                      float radius1,
                                      float bend_radius,
                                      float bend_ang0,
                                      float bend_ang1,
                                      float R, float G, float B,
                                      int slices,
                                      int delta);

void draw_curved_cylinder_y(unsigned int VAO,
                            int slices,
                            int delta);

//-----------------------------------------------

// plane zx (rotation axis = y)
unsigned int create_torus_y(float radius,
                            float bend_radius,
                            float R, float G, float B,
                            int slices,
                            int delta);

void draw_torus_y(unsigned int VAO,
                  int slices,
                  int delta);

//-----------------------------------------

unsigned int create_cylinder_z(float radius0,
                               float radius1,
                               float z0,
                               float z1,
                               float R, float G, float B,
                               int delta);

void draw_cylinder_z(unsigned int VAO,
                     int delta);

//-----------------------------------------

unsigned int create_cylinder_y(float radius0,
                               float radius1,
                               float y0,
                               float y1,
                               float R, float G, float B,
                               int delta);

void draw_cylinder_y(unsigned int VAO,
                     int delta);

void draw_cylinder_y(unsigned int VAO,
                     int delta,
                     GLuint tex);

//--------------------------------------------

unsigned int create_elliptic_cylinder_z(float a0, float b0,
                                        float a1, float b1,
                                        float z0,
                                        float z1,
                                        float R, float G, float B,
                                        int delta);

void draw_elliptic_cylinder_z(unsigned int VAO,
                              int delta);

#endif