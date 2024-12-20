
float get_player_angle(struct GameData *GD){
  return GD->p_angle;
}



unsigned int create_hairy_ellipsoid(float R, float G, float B){
  float th0, th1;
  float hair_R = 8.0;
  th0 = 90.0;
  th1 = 180.0;
  return create_curved_cylinder_x(3.0,
				  0.0,
				  hair_R,
				  th0, th1,
				  R, G, B,
				  20, 4);
}



void draw_hairy_ellipsoid(unsigned int VAO,
			  glm::mat4 std_model, glm::mat4 std_normal,
			  unsigned int modelLoc, unsigned int normalLoc,
			  float a, float b, float c,
			  int slices){
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 normal = glm::mat4(1.0f);
  float w0, w1, angle, x0, y0, z0, x1, y1, z1, xb, yb, zb;
  float cos_angle, sin_angle, hair_theta, hair_R;
  float mag, a0, a1, b0, b1, th0, th1, c_th0, s_th0;
  float C[3];
  int i, j, dj = 0, di = 3;
  hair_R = 8.0;
  th0 = 90.0;
  th1 = 180.0;
  c_th0 = cosf(th0*(PI / 180.0f));
  s_th0 = sinf(th0*(PI / 180.0f));

  for(i = 1; i < (int)(0.8*slices+0.5); i++){
    if(i%di != 0) continue;
    w0 = (float)i / (float)slices;
    w1 = (float)(i+1) / (float)slices;

    z0 = (-c)*(1.0-w0) + c*w0;
    z1 = (-c)*(1.0-w1) + c*w1;    

    b0 = sqrtf(b*b*(1.0 - (z0*z0)/(c*c)));
    b1 = sqrtf(b*b*(1.0 - (z1*z1)/(c*c)));

    a0 = sqrtf(a*a*(1.0 - (z0*z0)/(c*c)));
    a1 = sqrtf(a*a*(1.0 - (z1*z1)/(c*c)));

    if(dj == 0)
      dj = 10;
    else
      dj = 0;
    
    for(j = dj; j <= 360; j+=20){ 
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

      hair_theta = -(atan2f(x0, -z0)*(180.0f/PI))/2.0;
      //hair_theta = -60;
      //hair_theta = 0;      

      model = std_model;
      normal = std_normal;
      model = glm::translate(model,
			     glm::vec3(x0 - C[0]*2,
				       y0 - C[1]*2 - s_th0*hair_R,
				       z0 - C[2]*2 - c_th0*hair_R));
      model = glm::rotate(model, glm::radians(hair_theta),
			  glm::vec3(0.0, 1.0, 0.0));
      normal = glm::rotate(normal, glm::radians(hair_theta),
			   glm::vec3(0.0, 1.0, 0.0));
      glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
      draw_curved_cylinder_x(VAO, 20, 4);
      
      C[0] = (2.0*x1)/(a*a); //b1*cos_angle;
      C[1] = (2.0*y1)/(b*b); //y1;
      C[2] = (2.0*z1)/(c*c); //b1*cos_angle;
      mag = magnitude(C);
      C[0] /= mag;
      C[1] /= mag;
      C[2] /= mag;
      //glNormal3f(C[0], C[1], C[2]);
      //glVertex3f( x1, y1, z1 );
    }
  }
}



void DrawFluffyProfessorSkin(struct GameData *GD,
			     float a, float b, float c,
			     unsigned int modelLoc,
			     unsigned int normalLoc,
			     glm::mat4 std_model,
			     glm::mat4 std_normal,
			     float deltatime){
  static bool firsttime = true;
  glm::mat4 model;
  glm::mat4 normal;
  float thickness = b*0.04;
  //------------Glasses:--------------------------
  static unsigned int glasses_VAO[4];
  if(firsttime){
    glasses_VAO[0] = create_curved_cylinder_x(thickness, thickness, b*0.2,
					      75.0, 105.0,
					      0.0, 0.0, 0.5,
					      20, 4);
    glasses_VAO[1] = create_curved_cylinder_x(thickness, thickness, b*0.2,
					      0.0, 360.0,
					      0.0, 0.0, 0.5,
					      20, 4);
    glasses_VAO[2] = create_cylinder_z(thickness, thickness,
				       0, b/2.0,
				       0.0, 0.0, 0.5,
				       5);
    glasses_VAO[3] = create_curved_cylinder_x(thickness, thickness, b*0.2,
					      90.0, 180.0,
					      0.0, 0.0, 0.5,
					      20, 4);
  }
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, b*0.15, c*1.05));
  model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(glasses_VAO[0], 20, 4);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35, b*0.35, c*1.05));
  model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(glasses_VAO[1], 20, 4);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35, b*0.35, c*1.05));
  model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(glasses_VAO[1], 20, 4);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35+b*0.2, b*0.35, c*1.05));
  model = glm::rotate(model,  glm::radians(155.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(155.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(glasses_VAO[2], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35-b*0.2, b*0.35, c*1.05));
  model = glm::rotate(model,  glm::radians(-155.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-155.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(glasses_VAO[2], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35-b*0.2, b*0.15, c*1.05));
  model = glm::rotate(model,  glm::radians(180.0f-155.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(180.0f-155.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(0.0, 0.0, -b/2.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(glasses_VAO[3], 20, 4);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35+b*0.2, b*0.15, c*1.05));
  model = glm::rotate(model,  glm::radians(-180.0f+155.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-180.0f+155.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(0.0, 0.0, -b/2.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(glasses_VAO[3], 20, 4);
  //------------Moustache:------------------------
  static unsigned int moustache_VAO[2];
  if(firsttime){
    moustache_VAO[0] = create_curved_cylinder_x(0.0, b*0.1, b*0.3,
						0.0, 90.0,
						1.0, 1.0, 1.0,
						20, 4);
    moustache_VAO[1] = create_ellipsoid(b*0.1, b*0.1, b*0.1,
					1.0, 1.0, 1.0,
					10);
  }

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-b*0.10, -b*0.30, c*1.05));
  model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(moustache_VAO[0], 20, 4);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(b*0.10, -b*0.30, c*1.05));
  model = glm::rotate(model,  glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(moustache_VAO[0], 20, 4);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-b*0.10, b*0.00, c*1.05));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(moustache_VAO[1], 10);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(b*0.10, b*0.00, c*1.05));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(moustache_VAO[1], 10);
  //----------------------------------------------
  static unsigned int top_hat_VAO[4];
  if(firsttime){
    top_hat_VAO[0] = create_cylinder_z(0.4*b, 0.4*b,
				       0, b*0.4,
				       0.2, 0.2, 0.2,
				       5);
    top_hat_VAO[1] = create_cylinder_z(0.41*b, 0.4*b,
				       0, b*0.1,
				       1.0, 1.0, 0.0,
				       5);
    top_hat_VAO[2] = create_cylinder_z(0.7*b, 0.4*b,
				       0, b*0.05,
				       0.2, 0.2, 0.2,
				       5);
    top_hat_VAO[3] = create_cylinder_z(0.4*b, 0.001*b,
				       0, b*0.05,
				       0.2, 0.2, 0.2,
				       5);
  }

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, b*0.9, 0.1*c));
  model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(top_hat_VAO[0], 5); 
  draw_cylinder_z(top_hat_VAO[1], 5); 
  draw_cylinder_z(top_hat_VAO[2], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, b*(0.9+0.4), 0.1*c));
  model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal,  glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(top_hat_VAO[3], 5);
  //----------------------------------------------  
  firsttime = false;
}



void DrawFluffyBody(struct GameData *GD,
		    float a, float b, float c,
		    unsigned int modelLoc,
		    unsigned int normalLoc,
		    glm::mat4 std_model,
		    glm::mat4 std_normal,
		    float deltatime){
  static bool firsttime = true;
  glm::mat4 model;
  glm::mat4 normal;
  static float eye_angle = -10.0; //Intervalo: [-10.0, -80.0]
  static float eye_angle_step = 150.0*deltatime;
  eye_angle_step = SIGN(eye_angle_step)*150.0*deltatime;
  //------------Body hair:--------------------------
  static unsigned int hair_VAO;
  if(firsttime)
    hair_VAO = create_hairy_ellipsoid(1.0, 1.0, 1.0);

  glBindTexture(GL_TEXTURE_2D, GD->null_tex);

  if(GD->skin == SKIN_Professor)
    DrawFluffyProfessorSkin(GD, a, b, c, modelLoc, normalLoc,
			    std_model, std_normal, deltatime);

  draw_hairy_ellipsoid(hair_VAO,
		       std_model, std_normal,
		       modelLoc, normalLoc,
		       a, b, c,
		       40);
  //------------Body and mouth:---------------------------
  static unsigned int body_VAO[4];
  if(firsttime){
    body_VAO[0] = create_ellipsoid_lune_z(a, b, c,
					  -5, 340,
					  1.0, 1.0, 1.0,
					  40);
    body_VAO[1] = create_ellipsoid_lune_z(a*0.95, b*0.95, c*0.95,
					  -16,   0,
					  1.0, 1.0, 1.0,
					  40);
    body_VAO[2] = create_ellipsoid_lune_z(a*0.95, b*0.95, c*0.95,
					  -40, -18,
					  1.0, 1.0, 1.0,
					  40);
    body_VAO[3] = create_ellipsoid_lune_z(a*0.94, b*0.94, c*0.94,
					  -40, 0,
					  0.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  model = std_model;
  model  = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(body_VAO[0], -5, 340, 40);
  draw_ellipsoid_lune_z(body_VAO[1], -16, 0, 40);
  draw_ellipsoid_lune_z(body_VAO[2], -40, -18, 40);
  draw_ellipsoid_lune_z(body_VAO[3], -40, 0, 40);
  //------------Fix cheek:-------------------------------
  static unsigned int cheek_VAO[4];
  if(firsttime){
    cheek_VAO[0] = create_ellipsoid_lune_z(a, c, b,
					   0,  60,
					   1.0, 1.0, 1.0,
					   40);
    cheek_VAO[1] = create_ellipsoid_lune_z(a, c, b,
					   120,  180,
					   1.0, 1.0, 1.0,
					   40);
    cheek_VAO[2] = create_ellipsoid_lune_z(a*0.97, c*0.97, b*0.97,
					   55,  65,
					   0.0, 0.0, 0.0,
					   40);
    cheek_VAO[3] = create_ellipsoid_lune_z(a*0.97, c*0.97, b*0.97,
					   115, 125,
					   0.0, 0.0, 0.0,
					   40);
  }
  model = std_model;
  model = glm::rotate(model,  glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = std_normal;
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(cheek_VAO[0], 0, 60, 40);
  draw_ellipsoid_lune_z(cheek_VAO[1], 120, 180, 40);
  draw_ellipsoid_lune_z(cheek_VAO[2], 55, 65, 40);
  draw_ellipsoid_lune_z(cheek_VAO[3], 115, 125, 40);
  //------------Tuft of hair:------------------------
  static unsigned int tuft_VAO;
  if(firsttime){
    tuft_VAO = create_curved_cylinder_x(2.0, 0.0, 8.0,
					-20.0, 130.0,
					1.0, 1.0, 1.0,
					20, 4);
  }
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, b*0.9, c*0.15));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(tuft_VAO, 20, 4);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-5.0, b*0.9, c*0.15));
  model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(tuft_VAO, 20, 4);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(5.0, b*0.9, c*0.15));
  model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_x(tuft_VAO, 20, 4);
  
  //------------Tail:--------------------------------
  static unsigned int tail_VAO;
  if(firsttime)
    tail_VAO = create_ellipsoid(b*0.30, b*0.30, b*0.30,
				1.0, 1.0, 1.0,
				10);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3( 0.0, 0.0, -c*1.05));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(tail_VAO, 10);
  //------------Eyebrow:-----------------------------
  static unsigned int eyebrow_VAO[2];
  if(firsttime)
    eyebrow_VAO[0] = create_ellipsoid(b*0.25, b*0.15, b*0.15,
				      1.0, 1.0, 1.0,
				      10);
  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(-35.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.15, b*0.55, c*0.75));
  model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(eyebrow_VAO[0], 10);

  if(firsttime)
    eyebrow_VAO[1] = create_ellipsoid(b*0.25, b*0.15, b*0.15,
				      1.0, 1.0, 1.0,
				      10);
  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(35.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(a*0.15, b*0.55, c*0.75));
  model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(eyebrow_VAO[1], 10);
  //------------Eyes:--------------------------------
  static unsigned int eyes_VAO[2];
  int eye_opening = 180;
  int angle_r1, angle_r2;
  int angle_l1, angle_l2;
  angle_r1 = 270-eye_opening/2;
  angle_r2 = 270+eye_opening/2;
  angle_l1 = 270+eye_opening/2-360;
  angle_l2 = 270-eye_opening/2;
  if(firsttime){
    eyes_VAO[0] = create_ellipsoid_lune_z(b*0.45, b*0.45, b*0.45,
					  angle_r1, angle_r2,
					  0.65, 0.65, 0.65,
					  10);
    eyes_VAO[1] = create_ellipsoid_lune_z(b*0.45, b*0.45, b*0.45,
					  angle_l1, angle_l2,
					  1.0, 1.0, 1.0,
					  10);
  }
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, b*0.25, c*0.5));
  model  = glm::rotate(model, glm::radians(eye_angle), glm::vec3(1.0, 0.0, 0.0));
  model  = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(eye_angle), glm::vec3(1.0, 0.0, 0.0));  
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(eyes_VAO[0], angle_r1, angle_r2, 10);
  draw_ellipsoid_lune_z(eyes_VAO[1], angle_l1, angle_l2, 10);

  //Intervalo: [-10.0, -80.0]
  eye_angle += eye_angle_step;
  if(eye_angle < -80.0){
    eye_angle = -80.0;
    eye_angle_step = -eye_angle_step;
  }
  else if(eye_angle > -10.0){
    eye_angle = -10.0;
    eye_angle_step = -eye_angle_step;
  }
  //------------Nose:--------------------------------
  static unsigned int nose_VAO;
  if(firsttime)
    nose_VAO = create_ellipsoid(b*0.20, b*0.10, b*0.15,
				0.0, 0.0, 0.0,
				10);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3( 0.0, b*0.15, c*1.05));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(nose_VAO, 10);
  //-----------------------------------------------
  firsttime = false;
}




void DrawFluffy(struct GameData *GD,
		unsigned int shaderProgram,
		float deltatime,
		bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);
  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95, 0.0,  c*0.75));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95, 0.0,  c*0.75));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}





void DrawFluffy_walk(struct GameData *GD,
		     unsigned int shaderProgram,
		     float deltatime,
		     bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float foot_angle = 0.0;
  static float foot_angle_step = 300.0*deltatime;
  foot_angle_step = SIGN(foot_angle_step)*300.0*deltatime;

  if(restart){
    foot_angle = 0.0;
  }
  
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);

  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(-foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);

  foot_angle += foot_angle_step;
  if(foot_angle > 45.0){
    foot_angle = 45.0;
    foot_angle_step = -foot_angle_step;
  }
  else if(foot_angle < -45.0){
    foot_angle = -45.0;
    foot_angle_step = -foot_angle_step;
  }

  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95, 0.0,  c*0.75));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95, 0.0,  c*0.75));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}




void DrawFluffy_push(struct GameData *GD,
		     unsigned int shaderProgram,
		     float deltatime,
		     bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float length_forearm = 0.0;
  static float step_forearm = 5.0*deltatime;
  step_forearm = SIGN(step_forearm)*5.0*deltatime;

  if(restart){
    length_forearm = 0.0;
  }
  
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);
  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];
  //------------Left arm:---------------------------
  static unsigned int larm_VAO;
  if(firsttime)
    larm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 90.0, 180.0,
					1.0, 1.0, 1.0,
					20, 4);
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95 - 0.27*d/2.0, -c*0.25,  c*0.75 - a*0.10));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  draw_curved_cylinder_y(larm_VAO, 20, 4);
  //------------Left forearm:-----------------------
  static unsigned int lforearm_VAO;
  if(firsttime)
    lforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(a*0.95, -c*0.25,  c*0.75 - a*0.10));
    model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(lforearm_VAO, 5);
  }
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95, 0.0,  c*0.75 + (d/2.0)*length_forearm));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right arm:---------------------------
  static unsigned int rarm_VAO;
  if(firsttime)
    rarm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 180.0, 270.0,
					1.0, 1.0, 1.0,
					20, 4);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95 + 0.27*d/2.0, -c*0.25,  c*0.75 - a*0.10));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(rarm_VAO, 20, 4);
  //------------Right forearm:-----------------------
  static unsigned int rforearm_VAO;
  if(firsttime)
    rforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(-a*0.95, -c*0.25,  c*0.75 - a*0.10));
    model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(rforearm_VAO, 5);
  }
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95, 0.0,  c*0.75 + (d/2.0)*length_forearm));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);

  length_forearm += step_forearm;
  if(length_forearm > 1.0){
    step_forearm = -step_forearm;
    length_forearm = 1.0;
  }
  else if(length_forearm < 0.0){
    step_forearm = -step_forearm;
    length_forearm = 0.0;
  }
  //-------------------------------------------------
  firsttime = false;
}




void DrawFluffy_pull(struct GameData *GD,
		     unsigned int shaderProgram,
		     float deltatime,
		     bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float length_forearm = 0.3;
  static float step_forearm = 0.0;
  static float foot_angle = 0.0;
  static float foot_angle_step = 300.0*deltatime;
  foot_angle_step = SIGN(foot_angle_step)*300.0*deltatime;

  if(restart){
    foot_angle = 0.0;
  }
  
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);

  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(-foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);

  foot_angle += foot_angle_step;
  if(foot_angle > 45.0){
    foot_angle = 45.0;
    foot_angle_step = -foot_angle_step;
  }
  else if(foot_angle < -45.0){
    foot_angle = -45.0;
    foot_angle_step = -foot_angle_step;
  }

  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];
  //------------Left arm:---------------------------
  static unsigned int larm_VAO;
  if(firsttime)
    larm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 90.0, 180.0,
					1.0, 1.0, 1.0,
					20, 4);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*1.0 - 0.27*d/2.0, -c*0.25,  c*0.75 - a*0.10));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(larm_VAO, 20, 4);
  //------------Left forearm:-----------------------
  static unsigned int lforearm_VAO;
  if(firsttime)
    lforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(a*1.0, -c*0.25,  c*0.75 - a*0.10));
    model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(lforearm_VAO, 5);
  }
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*1.15, -b*0.05,  c*0.75 + (d/2.0)*length_forearm));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  //normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right arm:---------------------------
  static unsigned int rarm_VAO;
  if(firsttime)
    rarm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 180.0, 270.0,
					1.0, 1.0, 1.0,
					20, 4);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*1.0 + 0.27*d/2.0, -c*0.25,  c*0.75 - a*0.10));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(rarm_VAO, 20, 4);
  //------------Right forearm:-----------------------
  static unsigned int rforearm_VAO;
  if(firsttime)
    rforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(-a*1.0, -c*0.25,  c*0.75 - a*0.10));
    model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(rforearm_VAO, 5);
  }
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*1.15, -b*0.05,  c*0.75 + (d/2.0)*length_forearm));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::rotate(model, glm::radians( 90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians( 90.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  //normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}




void DrawTrack(struct GameData *GD,
	       unsigned int shaderProgram,
	       float deltatime){
  float d = 60.0;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);

  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(0.0*d, GD->py*d, 0.0*d));
  //std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  //std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));
  //-------------------------------------------------
  static unsigned int track_VAO;
  if(firsttime)
    track_VAO = create_square_y(d*50.0, d*50.0,
				1.0, 1.0, 1.0);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.5, d*0.0));
  //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  //normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_square_y(track_VAO, GD->track_tex);
  //-------------------------------------------------
  firsttime = false;  
}




void DrawKart(struct GameData *GD,
	      unsigned int shaderProgram,
	      float deltatime,
	      bool restart){
  float d = 60.0;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);

  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d + 0.20*d, GD->pz*d + d*0.60));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));
  //----------------wheel-----------------------
  static unsigned int wheel_VAO[5];
  if(firsttime)
    wheel_VAO[0] = create_cylinder_z(d/4.0, d/4.0,
				     0, d/3.0,
				     0.2, 0.2, 0.2,
				     5);
  if(firsttime)
    wheel_VAO[1] = create_cylinder_z(d/5.0, d/5.0,
				     0, d/3.0,
				     0.2, 0.2, 0.2,
				     5);
  if(firsttime)
    wheel_VAO[2] = create_ellipse_z(d/4.0, d/4.0,
				    1.0, 1.0, 1.0,
				    5);
  if(firsttime)
    wheel_VAO[3] = create_ellipse_z(d/5.0, d/5.0,
				    1.0, 1.0, 1.0,
				    5);
  
  if(firsttime)
    wheel_VAO[4] = create_cylinder_z(d*0.03, d*0.03,
				     -d*0.7, d*0.7,
				     0.2, 0.2, 0.2,
				     15);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.4, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(wheel_VAO[0], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.4 - d/3.0, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(wheel_VAO[0], 5);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.4, -d*0.5,  d*0.65));
  model = glm::rotate(model, glm::radians(90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(wheel_VAO[1], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.4, -d*0.5,  d*0.65));
  model = glm::rotate(model, glm::radians(-90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(wheel_VAO[1], 5);

  glBindTexture(GL_TEXTURE_2D, GD->wheel_cap_tex);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.4, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[2], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.4 + d/3.0, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[2], 5);  
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.4 - d/3.0, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[2], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.4, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[2], 5);


  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.4, -d*0.5,  d*0.65));
  model = glm::rotate(model, glm::radians(-90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[3], 5);

  float cos_sa;
  float sin_sa;
  cos_sa = cosf(GD->steering_angle*(PI/180.0));
  sin_sa = sinf(GD->steering_angle*(PI/180.0));
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.4 + cos_sa*d/3.0, -d*0.5,  d*0.65 - sin_sa*d/3.0));
  model = glm::rotate(model, glm::radians(90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[3], 5);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.4 - cos_sa*d/3.0, -d*0.5,  d*0.65 + sin_sa*d/3.0));
  model = glm::rotate(model, glm::radians(-90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[3], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.4, -d*0.5,  d*0.65));
  model = glm::rotate(model, glm::radians(90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f + GD->steering_angle), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(wheel_VAO[3], 5);

  glBindTexture(GL_TEXTURE_2D, GD->null_tex);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.5 + (d/4.0-d/5.0), -d*0.60));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(wheel_VAO[4], 15);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.5, d*0.65));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(wheel_VAO[4], 15);
  
  //-----------------exhaust-----------------------------
  static unsigned int exhaust_VAO[2];
  if(firsttime)
    exhaust_VAO[0] = create_cylinder_z(d/8.0, d/12.0,
				       0, d/3.0,
				       0.8, 0.8, 0.8,
				       5);

  if(firsttime)
    exhaust_VAO[1] = create_curved_cylinder_x(d/12.0, d/12.0,
					      d*0.3,
					      0.0, 90.0,
					      0.8, 0.8, 0.8,
					      40, 5);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.2, -d*0.10, -d*1.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(exhaust_VAO[0], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.2, -d*0.10, -d*1.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(exhaust_VAO[0], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.2, -d*0.10 - d*0.3, -d*1.20 + d/3.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(exhaust_VAO[1], 40, 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.2, -d*0.10 - d*0.3, -d*1.20 + d/3.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(exhaust_VAO[1], 40, 5);
  //-----------steering wheel--------------------
  static unsigned int steering_VAO[2];
  if(firsttime)
    steering_VAO[0] = create_torus_y(d/25.0,
				     d/8.0,
				     0.4, 0.4, 0.4,
				     40, 5);
  
  if(firsttime)
    steering_VAO[1] = create_cylinder_z(d/12.0, d/24.0,
					0.0*d, 0.1*d,
					0.4, 0.4, 0.4,
					15);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, d*0.0, d*0.60));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_torus_y(steering_VAO[0], 40, 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, d*0.0, d*0.60));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(steering_VAO[1], 15);
  //----------------front------------------------
  static unsigned int front_VAO[8];
  if(firsttime)
    front_VAO[0] = create_ellipsoid_lune_x(d*0.60, d*0.20, d*0.20,
					   0,  90,
					   //1.0, 0.0, 0.0,
					   1.0, 1.0, 1.0,
					   40);

  if(firsttime)
    front_VAO[1] = create_ellipsoid_lune_x(d*0.60, d*0.20, d*0.20,
					   90,  180,
					   //1.0, 0.0, 0.0,
					   1.0, 1.0, 1.0,
					   40);

  if(firsttime)
    front_VAO[2] = create_ellipsoid_lune_z(d*0.30, d*0.10, d*0.40,
					   0,  180,
					   1.0, 0.0, 0.0,
					   40);
  
  if(firsttime)
    front_VAO[3] = create_ellipse_z(d*0.20, d*0.20,
				    1.0, 1.0, 1.0,
				    15);

  if(firsttime)
    front_VAO[4] = create_cylinder_z(d*0.20, d*0.20,
				     d*0.01, d*0.114,
				     1.0, 1.0, 1.0,
				     15);
  
  if(firsttime)
    front_VAO[5] = create_elliptic_cylinder_z(d*0.60, d*0.20,
					      d*0.60, d*0.20,
					      0, d*0.1,
					      0.0, 0.0, 1.0,
					      5);

  if(firsttime)
    front_VAO[6] = create_curved_cylinder_x(d/12.0, d/12.0,
					    d*0.3,
					    0.0, 90.0,
					    1.0, 1.0, 1.0,
					    40, 5);

  if(firsttime)
    front_VAO[7] = create_ellipse_z(d*0.30, d*0.40,
				    1.0, 0.0, 0.0,
				    5);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.45, d*1.05));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_x(front_VAO[0], 0, 90, 40);
  draw_ellipsoid_lune_x(front_VAO[1], 90, 180, 40);  
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.1, d*1.00));
  model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(35.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(front_VAO[2], 0, 180, 40);


  glBindTexture(GL_TEXTURE_2D, GD->fluffy_tex);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.1 + 0.81915*d*0.105, d*1.00 + d*0.105*0.57357));
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(front_VAO[3], 15);
  
  glBindTexture(GL_TEXTURE_2D, GD->null_tex);  

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.1, d*1.00));
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_cylinder_z(front_VAO[4], 15);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.45, d*1.05));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_elliptic_cylinder_z(front_VAO[5], 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.30, d*0.70));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(front_VAO[6], 40, 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.1, d*1.00));
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_z(front_VAO[7], 5);
  
  //-----------------side--------------------------------
  GLuint tex[7];
  int i;
  for(i = 0; i < 7; i++)
    tex[i] = GD->null_tex;
  tex[2] = GD->ime_tex;
  tex[3] = GD->ime_tex;
  
  static unsigned int side_VAO[1];
  if(firsttime)
    side_VAO[0] = create_rounded_rectangular_cuboid(d*0.3, d*0.3, d*0.7,
						    d*0.05,
						    1.0, 0.0, 0.0,
						    1.0, 1.0, 1.0,
						    0.0, 0.0, 1.0);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-d*0.55, -d*0.4, d*0.05));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_rounded_rectangular_cuboid(side_VAO[0], tex);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.55, -d*0.4, d*0.05));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_rounded_rectangular_cuboid(side_VAO[0], tex);

  //-----------------back--------------------------------
  tex[2] = GD->null_tex;
  tex[3] = GD->null_tex;

  static unsigned int back_VAO;
  if(firsttime)
    back_VAO = create_rectangular_cuboid(d*0.6, d*0.3, d*0.3,
					 1.0, 1.0, 1.0);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.4, -d*0.7));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_rectangular_cuboid(back_VAO, tex);
  //-----------------seat----------------------------
  static unsigned int seat_VAO[2];
  if(firsttime)
    seat_VAO[0] = create_curved_cylinder_y(d*0.05, d*0.05,
					   d*0.3,
					   90.0, 270.0,
					   0.2, 0.2, 0.2,
					   40, 5);

  if(firsttime)
    seat_VAO[1] = create_ellipse_sector_z(d*0.3, d*0.3,
					  0.2, 0.2, 0.2,
					  0, 180,
					  5);
  
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.3, -d*0.40));
  model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(50.0f), glm::vec3(1.0, 0.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(seat_VAO[0], 40, 5);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(0.0, -d*0.3 -0.6427876*d*0.05, -d*0.40 -0.7660444*d*0.05));
  model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipse_sector_z(seat_VAO[1], 0, 180, 5);
  //------------------bottom-------------------------
  static unsigned int bottom_VAO[3];
  if(firsttime)
    bottom_VAO[0] = create_rectangular_cuboid(d*0.6, d*0.1, d*2.0,
					      1.0, 1.0, 1.0);

  if(firsttime)
    bottom_VAO[1] = create_rectangular_cuboid(d*0.8, d*0.1, d*0.5,
					      1.0, 1.0, 1.0);

  if(firsttime)
    bottom_VAO[2] = create_rectangular_cuboid(d*0.4, d*0.1, d*0.3,
					      0.2, 0.2, 0.2);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.5, d*0.15));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_rectangular_cuboid(bottom_VAO[0], tex);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.5, d*0.05));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_rectangular_cuboid(bottom_VAO[1], tex);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(d*0.0, -d*0.35, d*0.45));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_rectangular_cuboid(bottom_VAO[2], tex);  

  //-------------------------------------------------
  firsttime = false;
}



void DrawFluffy_seated(struct GameData *GD,
		       unsigned int shaderProgram,
		       float deltatime,
		       bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float length_forearm = 0.3;
  static float step_forearm = 0.0;
  static float foot_angle = -45.0;
  static float foot_angle_step = 0.0;

  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d + 0.20*d, GD->pz*d + d*0.60));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);

  normal = std_normal;
  model = std_model;
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);

  /*
  foot_angle += foot_angle_step;
  if(foot_angle > 45.0){
    foot_angle = 45.0;
    foot_angle_step = -foot_angle_step;
  }
  else if(foot_angle < -45.0){
    foot_angle = -45.0;
    foot_angle_step = -foot_angle_step;
  }
  */
  float R = 0.60*d/2.0; //0.27*d/2.0;
  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];
  //------------Left arm:---------------------------
  static unsigned int larm_VAO;
  if(firsttime)
    larm_VAO = create_curved_cylinder_y(3.0, 3.0,
					R, 90.0, 180.0,
					1.0, 1.0, 1.0,
					20, 4);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*1.0 - R, -c*0.25,  c*0.75 - a*0.10));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(larm_VAO, 20, 4);
  //------------Left forearm:-----------------------
  static unsigned int lforearm_VAO;
  if(firsttime)
    lforearm_VAO = create_curved_cylinder_y(3.0, 3.0,
					    R, 30.0, 90.0,
					    1.0, 1.0, 1.0,
					    20, 4);
    /*
    lforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
    */
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(a*1.0 - R, -c*0.25,  c*0.75 - a*0.10));
    //model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_curved_cylinder_y(lforearm_VAO, 20, 4);
    //draw_cylinder_z(lforearm_VAO, 5);
  }
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95 - R*0.60, -b*0.10,  c*0.75 + R));
  model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  //normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right arm:---------------------------
  static unsigned int rarm_VAO;
  if(firsttime)
    rarm_VAO = create_curved_cylinder_y(3.0, 3.0,
					R, 180.0, 270.0,
					1.0, 1.0, 1.0,
					20, 4);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*1.0 + R, -c*0.25,  c*0.75 - a*0.10));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(rarm_VAO, 20, 4);
  //------------Right forearm:-----------------------
  static unsigned int rforearm_VAO;
  if(firsttime)
    rforearm_VAO = create_curved_cylinder_y(3.0, 3.0,
					    R, 270.0, 330.0,
					    1.0, 1.0, 1.0,
					    20, 4);
    /*
    rforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
    */
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(-a*1.0 + R, -c*0.25,  c*0.75 - a*0.10));
    //model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    //draw_cylinder_z(rforearm_VAO, 5);
    draw_curved_cylinder_y(rforearm_VAO, 20, 4);
  }
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  //model = glm::translate(model, glm::vec3(-a*1.15, -b*0.05,  c*0.75 + (d/2.0)*length_forearm));
  model = glm::translate(model, glm::vec3(-a*0.95 + R*0.60, -b*0.10,  c*0.75 + R));
  model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  //normal = std_normal;
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}







void DrawFluffy_hang(struct GameData *GD,
		     unsigned int shaderProgram,
		     float deltatime,
		     bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float length_forearm = 0.75;
  static float foot_angle = 25.0;
  
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::translate(std_model, glm::vec3(0.0, (d/2.0)*0.20, 0.0));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);
  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];
  //------------Left arm:---------------------------
  static unsigned int larm_VAO;
  if(firsttime)
    larm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 90.0, 180.0,
					1.0, 1.0, 1.0,
					20, 5);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95 - 0.27*d/2.0,
					  -c*0.10,
					  c*0.75 - a*0.25));
  model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(larm_VAO, 20, 5);
  //------------Left forearm:-----------------------  
  static unsigned int lforearm_VAO;
  if(firsttime)
    lforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(a*0.95,
					    -c*0.10,
					    c*0.75 - a*0.25));
    model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(lforearm_VAO, 5);
  }
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95, b*0.7,  c*0.75 + (d/2.0)*0.3));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right arm:---------------------------
  static unsigned int rarm_VAO;
  if(firsttime)
    rarm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 180.0, 270.0,
					1.0, 1.0, 1.0,
					20, 5);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95 + 0.27*d/2.0,
					  -c*0.10,
					  c*0.75 - a*0.25));
  model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));  
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(rarm_VAO, 20, 5);
  //------------Right forearm:-----------------------
  static unsigned int rforearm_VAO;
  if(firsttime)
    rforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    normal = std_normal;
    model = std_model;
    model = glm::translate(model, glm::vec3(-a*0.95,
					    -c*0.10,
					    c*0.75 - a*0.25));
    model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(-50.0f), glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0, 1.0, length_forearm));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(rforearm_VAO, 5);
  }
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95, b*0.7,  c*0.75 + (d/2.0)*0.3));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}




void DrawFluffy_hangright(struct GameData *GD,
			  unsigned int shaderProgram,
			  float deltatime,
			  bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float length_forearm = 0.75;
  static float foot_angle = 25.0;
  static float arm_angle_left  = 0.0; //20.0;
  static float arm_angle_right = 0.0;
  static float step_arm_angle;
  static int   turn_arm = 0;
  float displacement, L_proj, L_right, gamma_right, L_left, gamma_left;
  step_arm_angle = 150.0*deltatime;
  
  if(restart){
    arm_angle_left = 0.0;
    arm_angle_right = 0.0;
    turn_arm = 0;
  }

  glm::mat4 tmp_model = glm::mat4(1.0f);
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::translate(std_model, glm::vec3(0.0, (d/2.0)*0.20, 0.0));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);
  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];

  if(turn_arm == 0){
    arm_angle_right += step_arm_angle;
    if(arm_angle_right > 20.0){
      arm_angle_right = 20.0;
      turn_arm = 1;
    }
  }
  else if(turn_arm == 1){
    arm_angle_right -= step_arm_angle;
    arm_angle_left  -= step_arm_angle;
    if(arm_angle_right < 0.0 || arm_angle_left < -20.0){
      arm_angle_right = 0.0;
      arm_angle_left  = -20.0;
      turn_arm = 2;
    }
  }
  else{
    arm_angle_left += step_arm_angle;
    if(arm_angle_left > 0.0){
      arm_angle_left = 0.0;
      turn_arm = 0;
    }
  }
  
  L_proj = length_forearm*sinf(PI*50.0/180.0);
  displacement = 4.0*L_proj*(1.0/cosf(PI*arm_angle_right/180.0) - 1.0);
  displacement += L_proj; //+ 3.0/d;
  L_right = sqrtf(SQUARE(length_forearm*cosf(PI*50.0/180.0)) + SQUARE(displacement));
  gamma_right = 180.0*asinf(displacement/L_right)/PI - 50;

  //printf("right = L: %f, gamma: %f, displacement: %f\n",L_right,gamma_right,displacement);

  L_proj = length_forearm*sinf(PI*50.0/180.0);
  displacement = 4.0*L_proj*(1.0/cosf(PI*arm_angle_left/180.0) - 1.0);
  displacement += L_proj; //+ 3.0/d;
  L_left = sqrtf(SQUARE(length_forearm*cosf(PI*50.0/180.0)) + SQUARE(displacement));
  gamma_left = 180.0*asinf(displacement/L_left)/PI - 50;

  //printf("left = L: %f, gamma: %f, displacement: %f\n",L_left,gamma_left,displacement);
  //------------Left arm:---------------------------
  static unsigned int larm_VAO;
  if(firsttime)
    larm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 90.0, 180.0,
					1.0, 1.0, 1.0,
					20, 5);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95 - 0.27*d/2.0,
					  -c*0.10,
					  c*0.75 - a*0.25));
  model = glm::rotate(model, glm::radians(arm_angle_left), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(arm_angle_left), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-(50.0f+gamma_left)), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_left)), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(larm_VAO, 20, 5);
  //------------Left forearm:-----------------------  
  static unsigned int lforearm_VAO;
  if(firsttime)
    lforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    model = glm::rotate(model, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    model = glm::translate(model, glm::vec3(0.27*d/2.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    tmp_model = model;
    model = glm::scale(model, glm::vec3(1.0, 1.0, L_left));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(lforearm_VAO, 5);
    model = tmp_model;
  }
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  model = glm::translate(model, glm::vec3(0.0, 0.0, (d/2.0)*L_left));
  model = glm::rotate(model, glm::radians(50.0f+gamma_left), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(50.0f+gamma_left), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(0.0, c*0.26, a*0.02));

  model = glm::rotate(model, glm::radians(-arm_angle_left), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-arm_angle_left), glm::vec3(0.0, 0.0, 1.0));  

  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right arm:---------------------------
  static unsigned int rarm_VAO;
  if(firsttime)
    rarm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 180.0, 270.0,
					1.0, 1.0, 1.0,
					20, 5);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95 + 0.27*d/2.0,
					  -c*0.10,
					  c*0.75 - a*0.25));
  model = glm::rotate(model, glm::radians(arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(rarm_VAO, 20, 5);
  //------------Right forearm:-----------------------
  static unsigned int rforearm_VAO;
  if(firsttime)
    rforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    model = glm::rotate(model, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    model = glm::translate(model, glm::vec3(-0.27*d/2.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    tmp_model = model;
    model = glm::scale(model, glm::vec3(1.0, 1.0, L_right));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(rforearm_VAO, 5);
    model = tmp_model;
  }
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  model = glm::translate(model, glm::vec3(0.0, 0.0, (d/2.0)*L_right));
  model = glm::rotate(model, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(0.0, c*0.26, a*0.02));
  model = glm::rotate(model, glm::radians(-arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}





void DrawFluffy_hangleft(struct GameData *GD,
			 unsigned int shaderProgram,
			 float deltatime,
			 bool restart){
  float d = 60.0, a, b, c;
  static bool firsttime = true;
  glm::mat4 model = glm::mat4(1.0f);
  unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
  glm::mat4 normal = glm::mat4(1.0f);
  unsigned int normalLoc = glGetUniformLocation(shaderProgram,"normal");
  float player_angle = get_player_angle(GD);
  static float length_forearm = 0.75;
  static float foot_angle = 25.0;
  static float arm_angle_left  = 0.0; //20.0;
  static float arm_angle_right = 0.0;
  static float step_arm_angle;
  static int   turn_arm = 0;
  float displacement, L_proj, L_right, gamma_right, L_left, gamma_left;
  step_arm_angle = 150.0*deltatime;
  
  if(restart){
    arm_angle_left = 0.0;
    arm_angle_right = 0.0;
    turn_arm = 0;
  }

  glm::mat4 tmp_model = glm::mat4(1.0f);
  glm::mat4 std_model = glm::mat4(1.0f);
  std_model = glm::translate(std_model, glm::vec3(GD->px*d, GD->py*d, GD->pz*d));
  std_model = glm::translate(std_model, glm::vec3(0.0, (d/2.0)*0.20, 0.0));
  std_model = glm::rotate(std_model, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 std_normal = glm::mat4(1.0f);
  std_normal = glm::rotate(std_normal, glm::radians(player_angle), glm::vec3(0.0, 1.0, 0.0));

  a = 0.8*d/2.0;
  b = d/2.0;
  c = 0.8*d/2.0;
  //------------Body:------------------------------
  DrawFluffyBody(GD, a, b, c, modelLoc, normalLoc,
		 std_model, std_normal, deltatime);
  //------------Feet:------------------------------
  static unsigned int feet_VAO[2];
  if(firsttime){
    feet_VAO[0] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
    feet_VAO[1] = create_ellipsoid_lune_z(a*0.30, b*0.10, c*0.70,
					  0,  180,
					  1.0, 0.0, 0.0,
					  40);
  }
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.35, -b,  c*0.2));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[0], 0, 180, 40);

  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.35, -b,  c*0.2));
  model = glm::rotate(model, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(foot_angle), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid_lune_z(feet_VAO[1], 0, 180, 40);
  //------------Hands:------------------------------
  static unsigned int hands_VAO[6];

  if(turn_arm == 0){
    arm_angle_left -= step_arm_angle;
    if(arm_angle_left < -20.0){
      arm_angle_left = -20.0;
      turn_arm = 1;
    }
  }
  else if(turn_arm == 1){
    arm_angle_right += step_arm_angle;
    arm_angle_left  += step_arm_angle;
    if(arm_angle_left > 0.0 || arm_angle_right > 20.0){
      arm_angle_left = 0.0;
      arm_angle_right  = 20.0;
      turn_arm = 2;
    }
  }
  else{
    arm_angle_right -= step_arm_angle;
    if(arm_angle_right < 0.0){
      arm_angle_right = 0.0;
      turn_arm = 0;
    }
  }
  
  L_proj = length_forearm*sinf(PI*50.0/180.0);
  displacement = 4.0*L_proj*(1.0/cosf(PI*arm_angle_right/180.0) - 1.0);
  displacement += L_proj; //+ 3.0/d;
  L_right = sqrtf(SQUARE(length_forearm*cosf(PI*50.0/180.0)) + SQUARE(displacement));
  gamma_right = 180.0*asinf(displacement/L_right)/PI - 50;

  //printf("right = L: %f, gamma: %f, displacement: %f\n",L_right,gamma_right,displacement);

  L_proj = length_forearm*sinf(PI*50.0/180.0);
  displacement = 4.0*L_proj*(1.0/cosf(PI*arm_angle_left/180.0) - 1.0);
  displacement += L_proj; //+ 3.0/d;
  L_left = sqrtf(SQUARE(length_forearm*cosf(PI*50.0/180.0)) + SQUARE(displacement));
  gamma_left = 180.0*asinf(displacement/L_left)/PI - 50;

  //printf("left = L: %f, gamma: %f, displacement: %f\n",L_left,gamma_left,displacement);
  //------------Left arm:---------------------------
  static unsigned int larm_VAO;
  if(firsttime)
    larm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 90.0, 180.0,
					1.0, 1.0, 1.0,
					20, 5);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(a*0.95 - 0.27*d/2.0,
					  -c*0.10,
					  c*0.75 - a*0.25));
  model = glm::rotate(model, glm::radians(arm_angle_left), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(arm_angle_left), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-(50.0f+gamma_left)), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_left)), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(larm_VAO, 20, 5);
  //------------Left forearm:-----------------------  
  static unsigned int lforearm_VAO;
  if(firsttime)
    lforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    model = glm::rotate(model, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    model = glm::translate(model, glm::vec3(0.27*d/2.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    tmp_model = model;
    model = glm::scale(model, glm::vec3(1.0, 1.0, L_left));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(lforearm_VAO, 5);
    model = tmp_model;
  }
  //------------Left hand:--------------------------
  if(firsttime)
    hands_VAO[0] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  model = glm::translate(model, glm::vec3(0.0, 0.0, (d/2.0)*L_left));
  model = glm::rotate(model, glm::radians(50.0f+gamma_left), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(50.0f+gamma_left), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(0.0, c*0.26, a*0.02));

  model = glm::rotate(model, glm::radians(-arm_angle_left), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-arm_angle_left), glm::vec3(0.0, 0.0, 1.0));  

  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1,  GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[0], 40);

  if(firsttime)
    hands_VAO[1] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[1], 40);  
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[2] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3(-a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[2], 40);
  //------------Right arm:---------------------------
  static unsigned int rarm_VAO;
  if(firsttime)
    rarm_VAO = create_curved_cylinder_y(3.0, 3.0,
					0.27*d/2.0, 180.0, 270.0,
					1.0, 1.0, 1.0,
					20, 5);
  normal = std_normal;
  model = std_model;
  model = glm::translate(model, glm::vec3(-a*0.95 + 0.27*d/2.0,
					  -c*0.10,
					  c*0.75 - a*0.25));
  model = glm::rotate(model, glm::radians(arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
  glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_curved_cylinder_y(rarm_VAO, 20, 5);
  //------------Right forearm:-----------------------
  static unsigned int rforearm_VAO;
  if(firsttime)
    rforearm_VAO = create_cylinder_z(3.0, 3.0,
				     0, d/2.0,
				     1.0, 1.0, 1.0,
				     5);
  if(length_forearm > 0.0){
    model = glm::rotate(model, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
    model = glm::translate(model, glm::vec3(-0.27*d/2.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    normal = glm::rotate(normal, glm::radians(-(50.0f+gamma_right)), glm::vec3(1.0, 0.0, 0.0));
    tmp_model = model;
    model = glm::scale(model, glm::vec3(1.0, 1.0, L_right));
    glUniformMatrix4fv(modelLoc,  1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    draw_cylinder_z(rforearm_VAO, 5);
    model = tmp_model;
  }
  //------------Right hand:--------------------------
  if(firsttime)
    hands_VAO[3] = create_ellipsoid(a*0.15, b*0.30, c*0.40,
				    1.0, 0.0, 0.0,
				    40);
  model = glm::translate(model, glm::vec3(0.0, 0.0, (d/2.0)*L_right));
  model = glm::rotate(model, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(50.0f+gamma_right), glm::vec3(1.0, 0.0, 0.0));
  model = glm::translate(model, glm::vec3(0.0, c*0.26, a*0.02));
  model = glm::rotate(model, glm::radians(-arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-arm_angle_right), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
  normal = glm::rotate(normal, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[3], 40);

  if(firsttime)
    hands_VAO[4] = create_ellipsoid(a*0.18, b*0.26, c*0.20,
				    1.0, 0.0, 0.0,
				    40);    
  model = glm::translate(model, glm::vec3(a*0.06, 0.0,  c*0.21));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  draw_ellipsoid(hands_VAO[4], 40);  
  model = glm::translate(model, glm::vec3(-a*0.06, 0.0,  -c*0.21));  

  if(firsttime)
    hands_VAO[5] = create_ellipsoid(a*0.10, b*0.10, c*0.30,
				    1.0, 0.0, 0.0,
				    40);
  normal = glm::rotate(normal, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  normal = glm::rotate(normal, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate(model, glm::vec3( a*0.045, b*0.2, -c*0.1));
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
  draw_ellipsoid(hands_VAO[5], 40);
  //-------------------------------------------------
  firsttime = false;
}


