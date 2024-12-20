

unsigned int create_square_y(float sx, float sz,
			     float R, float G, float B){
  float vertices[] = {
    // positions         // colors          // texture coords  //normals
    -0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.0f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.0f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.0f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f
  };
  int i;
  unsigned int VAO;
  unsigned int VBO;

  for(i = 0; i < 6; i++){
    vertices[i*11 + 0] *= sx;
    vertices[i*11 + 2] *= sz;

    vertices[i*11 + 3] = R;
    vertices[i*11 + 4] = G;
    vertices[i*11 + 5] = B;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
    
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  return VAO;
}



void draw_square_y(unsigned int VAO,
		   GLuint tex){
  glBindVertexArray(VAO);    
  glBindTexture(GL_TEXTURE_2D, tex);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}





unsigned int create_ellipse_sector_z(float a, float b,
				     float R, float G, float B,
				     int angle1, int angle2,
				     int dangle){
  float angle, cos_angle, sin_angle;
  float x, y, z = 0.0;
  float *vertices;
  int j, k, n;
  unsigned int VAO;
  unsigned int VBO;
  n = ((angle2-angle1)/dangle+2)*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(j = angle1; j <= angle2; j += dangle){
    angle = (float)j * (PI / 180.0f);
    cos_angle = cosf(angle);
    sin_angle = sinf(angle);
    x = a * cos_angle;
    y = b * sin_angle;

    //glVertex3f( x, y, z );
    vertices[k] = x; k++;
    vertices[k] = y; k++;
    vertices[k] = z; k++;
    
    //Colors:
    vertices[k] = R; k++;
    vertices[k] = G; k++;
    vertices[k] = B; k++;
    
    //Texture:
    vertices[k] = ( cos_angle + 1.0)/2.0; k++;
    vertices[k] = (-sin_angle + 1.0)/2.0; k++;
    
    //Normals:
    vertices[k] = 0.0; k++;
    vertices[k] = 0.0; k++;
    vertices[k] = 1.0; k++;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  free(vertices);
  return VAO;
}


void draw_ellipse_sector_z(unsigned int VAO,
			   int angle1, int angle2,
			   int dangle){
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, (angle2-angle1)/dangle+2);
}




unsigned int create_ellipse_z(float a, float b,
			      float R, float G, float B,
			      int dangle){
  float angle, cos_angle, sin_angle;
  float x, y, z = 0.0;
  float *vertices;
  int j, k, n;
  unsigned int VAO;
  unsigned int VBO;
  n = (360/dangle+2)*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(j = 0; j <= 360; j += dangle){
    angle = (float)j * (PI / 180.0f);
    cos_angle = cosf(angle);
    sin_angle = sinf(angle);
    x = a * cos_angle;
    y = b * sin_angle;

    //glVertex3f( x, y, z );
    vertices[k] = x; k++;
    vertices[k] = y; k++;
    vertices[k] = z; k++;
    
    //Colors:
    vertices[k] = R; k++;
    vertices[k] = G; k++;
    vertices[k] = B; k++;
    
    //Texture:
    vertices[k] = ( cos_angle + 1.0)/2.0; k++;
    vertices[k] = (-sin_angle + 1.0)/2.0; k++;
    
    //Normals:
    vertices[k] = 0.0; k++;
    vertices[k] = 0.0; k++;
    vertices[k] = 1.0; k++;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  free(vertices);
  return VAO;
}


void draw_ellipse_z(unsigned int VAO,
		    int dangle){
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 360/dangle+2);
}



unsigned int create_rectangular_cuboid(float sx, float sy, float sz,
				       float R, float G, float B){
  float vertices[] = {
    // positions         // colors          // texture coords  //normals
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f
  };
  int i;
  unsigned int VAO;
  unsigned int VBO;

  for(i = 0; i < 36; i++){
    vertices[i*11 + 0] *= sx;
    vertices[i*11 + 1] *= sy;
    vertices[i*11 + 2] *= sz;

    vertices[i*11 + 3] = R;
    vertices[i*11 + 4] = G;
    vertices[i*11 + 5] = B;
  }
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
    
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  return VAO;
}



void draw_rectangular_cuboid(unsigned int VAO,
			     GLuint tex[6]){
  int i;
  glBindVertexArray(VAO);    
  for(i = 0; i < 6; i++){
    glBindTexture(GL_TEXTURE_2D, tex[i]);
    glDrawArrays(GL_TRIANGLES, i*6, 6);
  }
}


//--------------------------------------


unsigned int create_rounded_rectangular_cuboid(float sx, float sy, float sz,
					       float radius,
					       float Rt, float Gt, float Bt,
					       float Rm, float Gm, float Bm,
					       float Rb, float Gb, float Bb){
  float vertices[] = {
    // positions         // colors          // texture coords  //normals
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f,

     //corners:
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,

     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,

     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,      

     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,

     //edges:
     //y > 0:
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,      

     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,

     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,

     //---
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     //y < 0.0:
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,

     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,      

     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,

     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,

     //---
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,

     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,

    //-----------------
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,

     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,      

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
   //------------------------  
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,

     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,      

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f
      
  };
  int i;
  unsigned int VAO;
  unsigned int VBO;
  sx -= 2*radius;
  sy -= 2*radius;
  sz -= 2*radius;
  for(i = 0; i < 36 + 8*3 + 24*3; i++){
    vertices[i*11 + 0] *= sx;
    vertices[i*11 + 1] *= sy;
    vertices[i*11 + 2] *= sz;

    vertices[i*11 + 3] = Rm;
    vertices[i*11 + 4] = Gm;
    vertices[i*11 + 5] = Bm;
  }
  for(i = 24; i < 30; i++){
    vertices[i*11 + 3] = Rb;
    vertices[i*11 + 4] = Gb;
    vertices[i*11 + 5] = Bb;
  }
  for(i = 30; i < 36; i++){
    vertices[i*11 + 3] = Rt;
    vertices[i*11 + 4] = Gt;
    vertices[i*11 + 5] = Bt;
  }
  //--------
  //corners:
  for(i = 36; i < 39; i++){
    vertices[i*11 + 3] = Rt;
    vertices[i*11 + 4] = Gt;
    vertices[i*11 + 5] = Bt;
  }
  for(i = 39; i < 42; i++){
    vertices[i*11 + 3] = Rb;
    vertices[i*11 + 4] = Gb;
    vertices[i*11 + 5] = Bb;
  }
  for(i = 42; i < 45; i++){
    vertices[i*11 + 3] = Rt;
    vertices[i*11 + 4] = Gt;
    vertices[i*11 + 5] = Bt;
  }
  for(i = 45; i < 51; i++){
    vertices[i*11 + 3] = Rb;
    vertices[i*11 + 4] = Gb;
    vertices[i*11 + 5] = Bb;
  }
  for(i = 51; i < 57; i++){
    vertices[i*11 + 3] = Rt;
    vertices[i*11 + 4] = Gt;
    vertices[i*11 + 5] = Bt;
  }
  for(i = 57; i < 60; i++){
    vertices[i*11 + 3] = Rb;
    vertices[i*11 + 4] = Gb;
    vertices[i*11 + 5] = Bb;
  }  
  //-----------
  //edges:
  for(i = 60; i < 84; i++){
    vertices[i*11 + 3] = Rt;
    vertices[i*11 + 4] = Gt;
    vertices[i*11 + 5] = Bt;
  }
  for(i = 84; i < 108; i++){
    vertices[i*11 + 3] = Rb;
    vertices[i*11 + 4] = Gb;
    vertices[i*11 + 5] = Bb;
  }
  
  
  for(i = 0; i < 6; i++)
    vertices[i*11 + 2] -= radius;
  for(i = 6; i < 12; i++)
    vertices[i*11 + 2] += radius;
  for(i = 12; i < 18; i++)
    vertices[i*11 + 0] -= radius;
  for(i = 18; i < 24; i++)
    vertices[i*11 + 0] += radius;  
  for(i = 24; i < 30; i++)
    vertices[i*11 + 1] -= radius;
  for(i = 30; i < 36; i++)
    vertices[i*11 + 1] += radius;

  //Corners:
  for(i = 36; i < 36 + 8*3; i+=3){
    vertices[i*11     + 1] += radius*vertices[i*11     +  9];
    vertices[(i+1)*11 + 2] += radius*vertices[(i+1)*11 + 10];
    vertices[(i+2)*11 + 0] += radius*vertices[(i+2)*11 +  8];
  }
  
  //Corners:
  /*
  vertices[36*11 + 1] += radius;
  vertices[37*11 + 2] += radius;
  vertices[38*11 + 0] += radius;

  vertices[39*11 + 1] -= radius;
  vertices[40*11 + 2] -= radius;
  vertices[41*11 + 0] -= radius;

  vertices[42*11 + 1] += radius;
  vertices[43*11 + 2] += radius;
  vertices[44*11 + 0] -= radius;

  vertices[45*11 + 1] -= radius;
  vertices[46*11 + 2] -= radius;
  vertices[47*11 + 0] += radius;

  vertices[48*11 + 1] -= radius;
  vertices[49*11 + 2] += radius;
  vertices[50*11 + 0] += radius;

  vertices[51*11 + 1] += radius;
  vertices[52*11 + 2] -= radius;
  vertices[53*11 + 0] -= radius;

  vertices[54*11 + 1] += radius;
  vertices[55*11 + 2] -= radius;
  vertices[56*11 + 0] += radius;

  vertices[57*11 + 1] -= radius;
  vertices[58*11 + 2] += radius;
  vertices[59*11 + 0] -= radius;
  */
  //edges:
  //y > 0:
  vertices[60*11 + 2] += radius;
  vertices[61*11 + 1] += radius;
  vertices[62*11 + 2] += radius;

  vertices[63*11 + 1] += radius;
  vertices[64*11 + 1] += radius;
  vertices[65*11 + 2] += radius;

  vertices[66*11 + 2] -= radius;
  vertices[67*11 + 1] += radius;
  vertices[68*11 + 2] -= radius;

  vertices[69*11 + 1] += radius;
  vertices[70*11 + 1] += radius;
  vertices[71*11 + 2] -= radius;

  //----
  vertices[72*11 + 0] += radius;
  vertices[73*11 + 0] += radius;
  vertices[74*11 + 1] += radius;

  vertices[75*11 + 1] += radius;
  vertices[76*11 + 0] += radius;
  vertices[77*11 + 1] += radius;

  vertices[78*11 + 0] -= radius;
  vertices[79*11 + 0] -= radius;
  vertices[80*11 + 1] += radius;

  vertices[81*11 + 1] += radius;
  vertices[82*11 + 0] -= radius;
  vertices[83*11 + 1] += radius;
  //y < 0:
  vertices[84*11 + 2] += radius;
  vertices[85*11 + 1] -= radius;
  vertices[86*11 + 2] += radius;

  vertices[87*11 + 1] -= radius;
  vertices[88*11 + 1] -= radius;
  vertices[89*11 + 2] += radius;

  vertices[90*11 + 2] -= radius;
  vertices[91*11 + 1] -= radius;
  vertices[92*11 + 2] -= radius;

  vertices[93*11 + 1] -= radius;
  vertices[94*11 + 1] -= radius;
  vertices[95*11 + 2] -= radius;

  //----
  vertices[96*11 + 0] += radius;
  vertices[97*11 + 0] += radius;
  vertices[98*11 + 1] -= radius;

  vertices[99*11 + 1] -= radius;
  vertices[100*11 + 0] += radius;
  vertices[101*11 + 1] -= radius;

  vertices[102*11 + 0] -= radius;
  vertices[103*11 + 0] -= radius;
  vertices[104*11 + 1] -= radius;

  vertices[105*11 + 1] -= radius;
  vertices[106*11 + 0] -= radius;
  vertices[107*11 + 1] -= radius;
  //------------
  vertices[108*11 + 0] += radius;
  vertices[109*11 + 2] += radius;
  vertices[110*11 + 0] += radius;

  vertices[111*11 + 2] += radius;
  vertices[112*11 + 0] += radius;
  vertices[113*11 + 2] += radius;

  vertices[114*11 + 0] -= radius;
  vertices[115*11 + 2] += radius;
  vertices[116*11 + 0] -= radius;

  vertices[117*11 + 2] += radius;
  vertices[118*11 + 0] -= radius;
  vertices[119*11 + 2] += radius;  
  //---------------
  vertices[120*11 + 0] += radius;
  vertices[121*11 + 2] -= radius;
  vertices[122*11 + 0] += radius;

  vertices[123*11 + 2] -= radius;
  vertices[124*11 + 0] += radius;
  vertices[125*11 + 2] -= radius;

  vertices[126*11 + 0] -= radius;
  vertices[127*11 + 2] -= radius;
  vertices[128*11 + 0] -= radius;

  vertices[129*11 + 2] -= radius;
  vertices[130*11 + 0] -= radius;
  vertices[131*11 + 2] -= radius;
  
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
    
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  return VAO;
}



void draw_rounded_rectangular_cuboid(unsigned int VAO,
				     GLuint tex[7]){
  int i;
  glBindVertexArray(VAO);    
  for(i = 0; i < 6; i++){
    glBindTexture(GL_TEXTURE_2D, tex[i]);
    glDrawArrays(GL_TRIANGLES, i*6, 6);
  }
  //Corners:
  glBindTexture(GL_TEXTURE_2D, tex[6]);
  glDrawArrays(GL_TRIANGLES, 36, 8*3 + 24*3);
}



//--------------------------------------

unsigned int create_ellipsoid(float a, float b, float c,
			      float R, float G, float B,
			      int slices){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, angle, x0, y0, z0, x1, y1, z1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag, a0, a1, b0, b1;
  float C[3];
  int t = 0;
  int i, j, k, n, dangle = 1; //2;

  n = slices*(360/dangle+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;

    z0 = (-c)*(1.0-w0) + c*w0;
    z1 = (-c)*(1.0-w1) + c*w1;    

    b0 = sqrtf(b*b*(1.0 - (z0*z0)/(c*c)));
    b1 = sqrtf(b*b*(1.0 - (z1*z1)/(c*c)));

    a0 = sqrtf(a*a*(1.0 - (z0*z0)/(c*c)));
    a1 = sqrtf(a*a*(1.0 - (z1*z1)/(c*c)));
    
    //glBegin(GL_TRIANGLE_STRIP);
    for(j = 0; j <= 360; j+=dangle){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      x0 = a0 * cos_angle;
      y0 = b0 * sin_angle;
      x1 = a1 * cos_angle;
      y1 = b1 * sin_angle;
      
      C[0] = (2.0*x0)/(a*a); //b0*cos_angle;
      C[1] = (2.0*y0)/(b*b); //y0;
      C[2] = (2.0*z0)/(c*c); //b0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x0, y0, z0 );
      vertices[k] = x0; k++;
      vertices[k] = y0; k++;
      vertices[k] = z0; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      C[0] = (2.0*x1)/(a*a); //b1*cos_angle;
      C[1] = (2.0*y1)/(b*b); //y1;
      C[2] = (2.0*z1)/(c*c); //b1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x1, y1, z1 );
      vertices[k] = x1; k++;
      vertices[k] = y1; k++;
      vertices[k] = z1; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      t++;
      t = t%2;
    }
    //glEnd();
  }
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  free(vertices);
  return VAO;
}




void draw_ellipsoid(unsigned int VAO,
		    int slices){
  int i,k,dangle = 1; //2;

  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (360/dangle+1)*2);
    k += (360/dangle+1)*2;
  }
}



//-------------------------------

//lune_z
unsigned int create_ellipsoid_lune_cap_z(float a, float b, float c,
					 float z_0, float z_1,
					 int angle1, int angle2,
					 float R, float G, float B,
					 int slices){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, angle, x0, y0, z0, x1, y1, z1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag, a0, a1, b0, b1;
  float C[3];
  int t = 0;
  int i, j, k, n;

  n = slices*(angle2-angle1+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;

    z0 = z_0*(1.0-w0) + z_1*w0;
    z1 = z_0*(1.0-w1) + z_1*w1;

    b0 = sqrtf(b*b*(1.0 - (z0*z0)/(c*c)));
    b1 = sqrtf(b*b*(1.0 - (z1*z1)/(c*c)));

    a0 = sqrtf(a*a*(1.0 - (z0*z0)/(c*c)));
    a1 = sqrtf(a*a*(1.0 - (z1*z1)/(c*c)));
    
    //glBegin(GL_QUAD_STRIP);
    for(j = angle1; j <= angle2; j++){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      x0 = a0 * cos_angle;
      y0 = b0 * sin_angle;
      x1 = a1 * cos_angle;
      y1 = b1 * sin_angle;
      
      C[0] = (2.0*x0)/(a*a); //b0*cos_angle;
      C[1] = (2.0*y0)/(b*b); //y0;
      C[2] = (2.0*z0)/(c*c); //b0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x0, y0, z0 );
      vertices[k] = x0; k++;
      vertices[k] = y0; k++;
      vertices[k] = z0; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
     
      C[0] = (2.0*x1)/(a*a); //b1*cos_angle;
      C[1] = (2.0*y1)/(b*b); //y1;
      C[2] = (2.0*z1)/(c*c); //b1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x1, y1, z1 );
      vertices[k] = x1; k++;
      vertices[k] = y1; k++;
      vertices[k] = z1; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      t++;
      t = t%2;
    }
    //glEnd();
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  free(vertices);
  return VAO;
}



void draw_ellipsoid_lune_cap_z(unsigned int VAO,
			       int angle1, int angle2,
			       int slices){
  int i,k;

  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2-angle1+1)*2);
    k += (angle2-angle1+1)*2;
  }
}


//-------------------------------

//lune_z
unsigned int create_ellipsoid_lune_z(float a, float b, float c,
				     int angle1, int angle2,
				     float R, float G, float B,
				     int slices){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, angle, x0, y0, z0, x1, y1, z1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag, a0, a1, b0, b1;
  float C[3];
  int t = 0;
  int i, j, k, n;

  n = slices*(angle2-angle1+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;

    z0 = (-c)*(1.0-w0) + c*w0;
    z1 = (-c)*(1.0-w1) + c*w1;    

    b0 = sqrtf(b*b*(1.0 - (z0*z0)/(c*c)));
    b1 = sqrtf(b*b*(1.0 - (z1*z1)/(c*c)));

    a0 = sqrtf(a*a*(1.0 - (z0*z0)/(c*c)));
    a1 = sqrtf(a*a*(1.0 - (z1*z1)/(c*c)));
    
    //glBegin(GL_QUAD_STRIP);
    for(j = angle1; j <= angle2; j++){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      x0 = a0 * cos_angle;
      y0 = b0 * sin_angle;
      x1 = a1 * cos_angle;
      y1 = b1 * sin_angle;
      
      C[0] = (2.0*x0)/(a*a); //b0*cos_angle;
      C[1] = (2.0*y0)/(b*b); //y0;
      C[2] = (2.0*z0)/(c*c); //b0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x0, y0, z0 );
      vertices[k] = x0; k++;
      vertices[k] = y0; k++;
      vertices[k] = z0; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
     
      C[0] = (2.0*x1)/(a*a); //b1*cos_angle;
      C[1] = (2.0*y1)/(b*b); //y1;
      C[2] = (2.0*z1)/(c*c); //b1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x1, y1, z1 );
      vertices[k] = x1; k++;
      vertices[k] = y1; k++;
      vertices[k] = z1; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      t++;
      t = t%2;
    }
    //glEnd();
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  free(vertices);
  return VAO;
}



void draw_ellipsoid_lune_z(unsigned int VAO,
			   int angle1, int angle2,
			   int slices){
  int i,k;

  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2-angle1+1)*2);
    k += (angle2-angle1+1)*2;
  }
}


//-----------------------------------------

unsigned int create_ellipsoid_lune_x(float a, float b, float c,
				     int angle1, int angle2,
				     float R, float G, float B,
				     int slices){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, angle, x0, y0, z0, x1, y1, z1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag, c0, c1, b0, b1;
  float C[3];
  int t = 0;
  int i, j, k, n;

  n = slices*(angle2-angle1+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;

    x0 = (-a)*(1.0-w0) + a*w0;
    x1 = (-a)*(1.0-w1) + a*w1;    

    b0 = sqrtf(b*b*(1.0 - (x0*x0)/(a*a)));
    b1 = sqrtf(b*b*(1.0 - (x1*x1)/(a*a)));

    c0 = sqrtf(c*c*(1.0 - (x0*x0)/(a*a)));
    c1 = sqrtf(c*c*(1.0 - (x1*x1)/(a*a)));
    
    //glBegin(GL_QUAD_STRIP);
    for(j = angle1; j <= angle2; j++){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      z0 = c0 * cos_angle;
      y0 = b0 * sin_angle;
      z1 = c1 * cos_angle;
      y1 = b1 * sin_angle;
      
      C[0] = (2.0*x0)/(a*a); //b0*cos_angle;
      C[1] = (2.0*y0)/(b*b); //y0;
      C[2] = (2.0*z0)/(c*c); //b0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x0, y0, z0 );
      vertices[k] = x0; k++;
      vertices[k] = y0; k++;
      vertices[k] = z0; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
     
      C[0] = (2.0*x1)/(a*a); //b1*cos_angle;
      C[1] = (2.0*y1)/(b*b); //y1;
      C[2] = (2.0*z1)/(c*c); //b1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x1, y1, z1 );
      vertices[k] = x1; k++;
      vertices[k] = y1; k++;
      vertices[k] = z1; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      t++;
      t = t%2;
    }
    //glEnd();
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  free(vertices);
  return VAO;
}



void draw_ellipsoid_lune_x(unsigned int VAO,
			   int angle1, int angle2,
			   int slices){
  int i,k;

  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2-angle1+1)*2);
    k += (angle2-angle1+1)*2;
  }
}

//-----------------------------------------

unsigned int create_ellipsoid_lune_cap_x(float a, float b, float c,
					 float x_0, float x_1,
					 int angle1, int angle2,
					 float R, float G, float B,
					 int slices){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, angle, x0, y0, z0, x1, y1, z1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag, c0, c1, b0, b1;
  float C[3];
  int t = 0;
  int i, j, k, n;

  n = slices*(angle2-angle1+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;

    x0 = x_0*(1.0-w0) + x_1*w0;
    x1 = x_0*(1.0-w1) + x_1*w1;    

    b0 = sqrtf(b*b*(1.0 - (x0*x0)/(a*a)));
    b1 = sqrtf(b*b*(1.0 - (x1*x1)/(a*a)));

    c0 = sqrtf(c*c*(1.0 - (x0*x0)/(a*a)));
    c1 = sqrtf(c*c*(1.0 - (x1*x1)/(a*a)));
    
    //glBegin(GL_QUAD_STRIP);
    for(j = angle1; j <= angle2; j++){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      z0 = c0 * cos_angle;
      y0 = b0 * sin_angle;
      z1 = c1 * cos_angle;
      y1 = b1 * sin_angle;
      
      C[0] = (2.0*x0)/(a*a); //b0*cos_angle;
      C[1] = (2.0*y0)/(b*b); //y0;
      C[2] = (2.0*z0)/(c*c); //b0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x0, y0, z0 );
      vertices[k] = x0; k++;
      vertices[k] = y0; k++;
      vertices[k] = z0; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
     
      C[0] = (2.0*x1)/(a*a); //b1*cos_angle;
      C[1] = (2.0*y1)/(b*b); //y1;
      C[2] = (2.0*z1)/(c*c); //b1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      
      //glVertex3f( x1, y1, z1 );
      vertices[k] = x1; k++;
      vertices[k] = y1; k++;
      vertices[k] = z1; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      t++;
      t = t%2;
    }
    //glEnd();
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  
  free(vertices);
  return VAO;
}



void draw_ellipsoid_lune_cap_x(unsigned int VAO,
			       int angle1, int angle2,
			       int slices){
  int i,k;

  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2-angle1+1)*2);
    k += (angle2-angle1+1)*2;
  }
}



//-----------------------------------------

//plane zy (rotation axis = x)
unsigned int create_curved_cylinder_x(float radius0,
				      float radius1,
				      float bend_radius,
				      float bend_ang0,
				      float bend_ang1,
				      float R, float G, float B,
				      int slices,
				      int dtheta){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, ang0, ang1, angle, x0, y0, x1, y1, xb, yb, zb;
  float cos_angle, sin_angle, cos_ang0, sin_ang0, cos_ang1, sin_ang1;
  float rad0, rad1, mag;
  float C[3];
  int t = 0;
  int i, j, k, n;

  n = slices*(360/dtheta+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));

  bend_ang0 *= (PI / 180.0f);
  bend_ang1 *= (PI / 180.0f);
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;
    ang0 = bend_ang0*(1.0-w0) + bend_ang1*w0;
    ang1 = bend_ang0*(1.0-w1) + bend_ang1*w1;

    rad0 = radius0*(1.0-w0) + radius1*w0;
    rad1 = radius0*(1.0-w1) + radius1*w1;    

    sin_ang0 = sinf(ang0);
    cos_ang0 = cosf(ang0);
    sin_ang1 = sinf(ang1);
    cos_ang1 = cosf(ang1);
    
    //glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+= dtheta){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      y0 = rad0 * cos_angle + bend_radius;
      x0 = rad0 * sin_angle;
      y1 = rad1 * cos_angle + bend_radius;
      x1 = rad1 * sin_angle;
      
      C[0] = x0;
      C[1] = sin_ang0*rad0*cos_angle;
      C[2] = cos_ang0*rad0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      yb = sin_ang0 * y0;
      xb = x0;
      zb = cos_ang0 * y0;
      //glVertex3f( xb, yb, zb );
      vertices[k] = xb; k++;
      vertices[k] = yb; k++;
      vertices[k] = zb; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      C[0] = x1;
      C[1] = sin_ang1*rad1*cos_angle;
      C[2] = cos_ang1*rad1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      yb = sin_ang1 * y1;
      xb = x1;
      zb = cos_ang1 * y1;
      //glVertex3f( xb, yb, zb );
      vertices[k] = xb; k++;
      vertices[k] = yb; k++;
      vertices[k] = zb; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;

      t++;
      t = t%2;
    }
    //glEnd();
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  free(vertices);
  return VAO;
}




void draw_curved_cylinder_x(unsigned int VAO,
			    int slices,
			    int dtheta){
  int i,k;
  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (360/dtheta+1)*2);
    k += (360/dtheta+1)*2;
  }
}

//-----------------------------------------

//plane zy (rotation axis = x)
unsigned int create_torus_x(float radius,
			    float bend_radius,
			    float R, float G, float B,
			    int slices,
			    int dtheta){
  return create_curved_cylinder_x(radius, radius,
				  bend_radius,
				  0.0, 360.0,
				  R, G, B,
				  slices, dtheta);
}

void draw_torus_x(unsigned int VAO,
		  int slices,
		  int dtheta){
  draw_curved_cylinder_x(VAO,
			 slices,
			 dtheta);
}

//-----------------------------------------


//plane zx (rotation axis = y)
unsigned int create_curved_cylinder_y(float radius0,
				      float radius1,
				      float bend_radius,
				      float bend_ang0,
				      float bend_ang1,
				      float R, float G, float B,
				      int slices,
				      int dtheta){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float w0, w1, ang0, ang1, angle, x0, y0, x1, y1, xb, yb, zb;
  float cos_angle, sin_angle, cos_ang0, sin_ang0, cos_ang1, sin_ang1;
  float rad0, rad1, mag;
  float C[3];
  int t = 0;
  int i, j, k, n;

  n = slices*(360/dtheta+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));

  bend_ang0 *= (PI / 180.0f);
  bend_ang1 *= (PI / 180.0f);
  for(i = 0; i < slices; i++){
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;
    ang0 = bend_ang0*(1.0-w0) + bend_ang1*w0;
    ang1 = bend_ang0*(1.0-w1) + bend_ang1*w1;

    rad0 = radius0*(1.0-w0) + radius1*w0;
    rad1 = radius0*(1.0-w1) + radius1*w1;    

    sin_ang0 = sinf(ang0);
    cos_ang0 = cosf(ang0);
    sin_ang1 = sinf(ang1);
    cos_ang1 = cosf(ang1);
    
    //glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= 360; j+= dtheta){
      angle = (float)j * (PI / 180.0f);
      cos_angle = cosf(angle);
      sin_angle = sinf(angle);
      x0 = rad0 * cos_angle + bend_radius;
      y0 = rad0 * sin_angle;
      x1 = rad1 * cos_angle + bend_radius;
      y1 = rad1 * sin_angle;
      
      C[0] = sin_ang0*rad0*cos_angle;
      C[1] = y0;
      C[2] = cos_ang0*rad0*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      xb = sin_ang0 * x0;
      yb = y0;
      zb = cos_ang0 * x0;
      //glVertex3f( xb, yb, zb );
      vertices[k] = xb; k++;
      vertices[k] = yb; k++;
      vertices[k] = zb; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 0.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;
      
      C[0] = sin_ang1*rad1*cos_angle;
      C[1] = y1;
      C[2] = cos_ang1*rad1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      xb = sin_ang1 * x1;
      yb = y1;
      zb = cos_ang1 * x1;
      //glVertex3f( xb, yb, zb );
      vertices[k] = xb; k++;
      vertices[k] = yb; k++;
      vertices[k] = zb; k++;

      //Colors:
      vertices[k] = R; k++;
      vertices[k] = G; k++;
      vertices[k] = B; k++;

      //Texture:
      vertices[k] = t;   k++;
      vertices[k] = 1.0; k++;

      //Normals:
      vertices[k] = C[0]; k++;
      vertices[k] = C[1]; k++;
      vertices[k] = C[2]; k++;

      t++;
      t = t%2;
    }
    //glEnd();
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  free(vertices);
  return VAO;
}




void draw_curved_cylinder_y(unsigned int VAO,
			    int slices,
			    int dtheta){
  int i,k;
  glBindVertexArray(VAO);
  k = 0;
  for(i = 0; i < slices; i++){
    glDrawArrays(GL_TRIANGLE_STRIP, k, (360/dtheta+1)*2);
    k += (360/dtheta+1)*2;
  }
}


//-----------------------------------------------

//plane zx (rotation axis = y)
unsigned int create_torus_y(float radius,
			    float bend_radius,
			    float R, float G, float B,
			    int slices,
			    int dtheta){
  return create_curved_cylinder_y(radius, radius,
				  bend_radius,
				  0.0, 360.0,
				  R, G, B,
				  slices, dtheta);
}

void draw_torus_y(unsigned int VAO,
		  int slices,
		  int dtheta){
  draw_curved_cylinder_y(VAO,
			 slices,
			 dtheta);
}

//-----------------------------------------


unsigned int create_cylinder_z(float radius0,
			       float radius1,
			       float z0,
			       float z1,
			       float R, float G, float B,
			       int dtheta){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float angle, x0, y0, x1, y1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag;
  float C[3];
  int t = 0;
  int j,n,k;

  n = (360/dtheta+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));

  //glBegin(GL_QUAD_STRIP);
  for(j = 0; j <= 360; j+= dtheta){
    angle = (float)j * (PI / 180.0f);
    cos_angle = cosf(angle);
    sin_angle = sinf(angle);
    y0 = radius0 * cos_angle;
    x0 = radius0 * sin_angle;
    y1 = radius1 * cos_angle;
    x1 = radius1 * sin_angle;
    
    C[0] = x0;
    C[1] = y0;
    C[2] = 0;
    mag = magnitude(C);
    C[0] /= mag;
    C[1] /= mag;
    C[2] /= mag;
    //glNormal3f(C[0], C[1], C[2]);
    yb = y0;
    xb = x0;
    zb = z0;
    //glVertex3f( xb, yb, zb );
    vertices[k] = xb; k++;
    vertices[k] = yb; k++;
    vertices[k] = zb; k++;
    
    //Colors:
    vertices[k] = R; k++;
    vertices[k] = G; k++;
    vertices[k] = B; k++;
    
    //Texture:
    vertices[k] = t;   k++;
    vertices[k] = 0.0; k++;
    
    //Normals:
    vertices[k] = C[0]; k++;
    vertices[k] = C[1]; k++;
    vertices[k] = C[2]; k++;
    
    C[0] = x1;
    C[1] = y1;
    C[2] = 0;
    mag = magnitude(C);
    C[0] /= mag;
    C[1] /= mag;
    C[2] /= mag;
    //glNormal3f(C[0], C[1], C[2]);
    yb = y1;
    xb = x1;
    zb = z1;
    //glVertex3f( xb, yb, zb );
    vertices[k] = xb; k++;
    vertices[k] = yb; k++;
    vertices[k] = zb; k++;
    
    //Colors:
    vertices[k] = R; k++;
    vertices[k] = G; k++;
    vertices[k] = B; k++;

    //Texture:
    vertices[k] = t;   k++;
    vertices[k] = 1.0; k++;

    //Normals:
    vertices[k] = C[0]; k++;
    vertices[k] = C[1]; k++;
    vertices[k] = C[2]; k++;    

    t++;
    t = t%2;
  }
  //glEnd();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  free(vertices);
  return VAO;
}



void draw_cylinder_z(unsigned int VAO,
		     int dtheta){
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, (360/dtheta+1)*2);
}


//--------------------------------------------

unsigned int create_elliptic_cylinder_z(float a0, float b0,
					float a1, float b1,
					float z0,
					float z1,
					float R, float G, float B,
					int dtheta){
  unsigned int VAO;
  unsigned int VBO;
  float *vertices;
  float angle, x0, y0, x1, y1, xb, yb, zb;
  float cos_angle, sin_angle;
  float mag;
  float C[3];
  int t = 0;
  int j,n,k;

  n = (360/dtheta+1)*2*11;
  k = 0;
  vertices = (float *)calloc(n, sizeof(float));

  //glBegin(GL_QUAD_STRIP);
  for(j = 0; j <= 360; j+= dtheta){
    angle = (float)j * (PI / 180.0f);
    cos_angle = cosf(angle);
    sin_angle = sinf(angle);
    y0 = b0 * cos_angle;
    x0 = a0 * sin_angle;
    y1 = b1 * cos_angle;
    x1 = a1 * sin_angle;
    
    C[0] = x0/(a0*a0);
    C[1] = y0/(b0*b0);
    C[2] = 0;
    mag = magnitude(C);
    C[0] /= mag;
    C[1] /= mag;
    C[2] /= mag;
    //glNormal3f(C[0], C[1], C[2]);
    yb = y0;
    xb = x0;
    zb = z0;
    //glVertex3f( xb, yb, zb );
    vertices[k] = xb; k++;
    vertices[k] = yb; k++;
    vertices[k] = zb; k++;
    
    //Colors:
    vertices[k] = R; k++;
    vertices[k] = G; k++;
    vertices[k] = B; k++;
    
    //Texture:
    vertices[k] = t;   k++;
    vertices[k] = 0.0; k++;
    
    //Normals:
    vertices[k] = C[0]; k++;
    vertices[k] = C[1]; k++;
    vertices[k] = C[2]; k++;
    
    C[0] = x1/(a1*a1);
    C[1] = y1/(b1*b1);
    C[2] = 0;
    mag = magnitude(C);
    C[0] /= mag;
    C[1] /= mag;
    C[2] /= mag;
    //glNormal3f(C[0], C[1], C[2]);
    yb = y1;
    xb = x1;
    zb = z1;
    //glVertex3f( xb, yb, zb );
    vertices[k] = xb; k++;
    vertices[k] = yb; k++;
    vertices[k] = zb; k++;
    
    //Colors:
    vertices[k] = R; k++;
    vertices[k] = G; k++;
    vertices[k] = B; k++;

    //Texture:
    vertices[k] = t;   k++;
    vertices[k] = 1.0; k++;

    //Normals:
    vertices[k] = C[0]; k++;
    vertices[k] = C[1]; k++;
    vertices[k] = C[2]; k++;    

    t++;
    t = t%2;
  }
  //glEnd();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, n*sizeof(float), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
			(void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  free(vertices);
  return VAO;
}



void draw_elliptic_cylinder_z(unsigned int VAO,
			      int dtheta){
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, (360/dtheta+1)*2);
}

