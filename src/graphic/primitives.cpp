#include "primitives.hpp"

#include <cmath>
#include <game/utils.hpp>

float magnitude(const float A[3]) { return sqrtf(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]); }

unsigned int createSquareY(const float sx, const float sz, const float R, const float G, const float B)
{
	float vertices[] = {// positions         // colors          // texture coords  //normals
						-0.5f, 0.0f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, -0.5f,
						1.0f,  1.0f,  1.0f,	 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
						1.0f,  1.0f,  0.0f,	 1.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
						1.0f,  0.0f,  -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f,
						0.0f,  -0.5f, 1.0f,	 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
	unsigned int VAO;
	unsigned int VBO;

	for (int i = 0; i < 6; i++)
	{
		vertices[i * 11 + 0] *= sx;
		vertices[i * 11 + 2] *= sz;

		vertices[i * 11 + 3] = R;
		vertices[i * 11 + 4] = G;
		vertices[i * 11 + 5] = B;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	return VAO;
}

void drawSquareY(const unsigned int VAO, const GLuint tex)
{
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

unsigned int createEllipseSectorZ(const float a, const float b, const float R, const float G, const float B,
								  const int angle1, const int angle2, const int dangle)
{
	unsigned int VAO;
	unsigned int VBO;
	const int n = ((angle2 - angle1) / dangle + 2) * 11;
	int k = 0;
	auto *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int j = angle1; j <= angle2; j += dangle)
	{
		constexpr float z = 0.0;
		const float angle = PI / 180.0f * static_cast<float>(j);
		const float cos_angle = cosf(angle);
		const float sin_angle = sinf(angle);
		const float x = a * cos_angle;
		const float y = b * sin_angle;

		// glVertex3f( x, y, z );
		vertices[k] = x;
		k++;
		vertices[k] = y;
		k++;
		vertices[k] = z;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = (cos_angle + 1.0) / 2.0;
		k++;
		vertices[k] = (-sin_angle + 1.0) / 2.0;
		k++;

		// Normals:
		vertices[k] = 0.0;
		k++;
		vertices[k] = 0.0;
		k++;
		vertices[k] = 1.0;
		k++;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

unsigned int createSquareZ(const float sx, const float sy, const float R, const float G, const float B)
{
	float vertices[] = {// positions         // colors          // texture coords  //normals
						-0.5f, -0.5f, 0.0f,	 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.0f,
						1.0f,  1.0f,  1.0f,	 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,  1.0f,
						1.0f,  0.0f,  0.0f,	 0.0f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,
						0.0f,  1.0f,  -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -0.5f,
						-0.5f, 0.0f,  1.0f,	 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
	unsigned int VAO;
	unsigned int VBO;

	for (int i = 0; i < 6; i++)
	{
		vertices[i * 11 + 0] *= sx;
		vertices[i * 11 + 1] *= sy;

		vertices[i * 11 + 3] = R;
		vertices[i * 11 + 4] = G;
		vertices[i * 11 + 5] = B;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	return VAO;
}


void drawSquareZ(const unsigned int VAO, const GLuint tex)
{
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void draw_ellipse_sector_z(const unsigned int VAO, const int angle1, const int angle2, const int dangle)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, (angle2 - angle1) / dangle + 2);
}

unsigned int create_ellipse_z(const float a, const float b, const float R, const float G, const float B,
							  const int dangle)
{
	constexpr float z = 0.0;
	unsigned int VAO;
	unsigned int VBO;
	const int n = (360 / dangle + 2) * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int j = 0; j <= 360; j += dangle)
	{
		const float angle = static_cast<float>(j) * (PI / 180.0f);
		const float cos_angle = cosf(angle);
		const float sin_angle = sinf(angle);
		const float x = a * cos_angle;
		const float y = b * sin_angle;

		// glVertex3f( x, y, z );
		vertices[k] = x;
		k++;
		vertices[k] = y;
		k++;
		vertices[k] = z;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = (cos_angle + 1.0) / 2.0;
		k++;
		vertices[k] = (-sin_angle + 1.0) / 2.0;
		k++;

		// Normals:
		vertices[k] = 0.0;
		k++;
		vertices[k] = 0.0;
		k++;
		vertices[k] = 1.0;
		k++;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_ellipse_z(const unsigned int VAO, const int dangle)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 360 / dangle + 2);
}

unsigned int create_rectangular_cuboid(const float sx, const float sy, const float sz, const float R, const float G,
									   const float B)
{
	float vertices[] = {
		// positions         // colors          // texture coords  //normals
		-0.5f, -0.5f, -0.5f, 1.0f,	1.0f,  1.0f,  1.0f,	 1.0f,	0.0f,  0.0f,  -1.0f, 0.5f,	-0.5f, -0.5f,
		1.0f,  1.0f,  1.0f,	 0.0f,	1.0f,  0.0f,  0.0f,	 -1.0f, 0.5f,  0.5f,  -0.5f, 1.0f,	1.0f,  1.0f,
		0.0f,  0.0f,  0.0f,	 0.0f,	-1.0f, 0.5f,  0.5f,	 -0.5f, 1.0f,  1.0f,  1.0f,	 0.0f,	0.0f,  0.0f,
		0.0f,  -1.0f, -0.5f, 0.5f,	-0.5f, 1.0f,  1.0f,	 1.0f,	1.0f,  0.0f,  0.0f,	 0.0f,	-1.0f, -0.5f,
		-0.5f, -0.5f, 1.0f,	 1.0f,	1.0f,  1.0f,  1.0f,	 0.0f,	0.0f,  -1.0f,

		-0.5f, -0.5f, 0.5f,	 1.0f,	1.0f,  1.0f,  0.0f,	 1.0f,	0.0f,  0.0f,  1.0f,	 0.5f,	-0.5f, 0.5f,
		1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  0.0f,  0.0f,	 1.0f,	0.5f,  0.5f,  0.5f,	 1.0f,	1.0f,  1.0f,
		1.0f,  0.0f,  0.0f,	 0.0f,	1.0f,  0.5f,  0.5f,	 0.5f,	1.0f,  1.0f,  1.0f,	 1.0f,	0.0f,  0.0f,
		0.0f,  1.0f,  -0.5f, 0.5f,	0.5f,  1.0f,  1.0f,	 1.0f,	0.0f,  0.0f,  0.0f,	 0.0f,	1.0f,  -0.5f,
		-0.5f, 0.5f,  1.0f,	 1.0f,	1.0f,  0.0f,  1.0f,	 0.0f,	0.0f,  1.0f,

		-0.5f, 0.5f,  0.5f,	 1.0f,	1.0f,  1.0f,  1.0f,	 0.0f,	-1.0f, 0.0f,  0.0f,	 -0.5f, 0.5f,  -0.5f,
		1.0f,  1.0f,  1.0f,	 0.0f,	0.0f,  -1.0f, 0.0f,	 0.0f,	-0.5f, -0.5f, -0.5f, 1.0f,	1.0f,  1.0f,
		0.0f,  1.0f,  -1.0f, 0.0f,	0.0f,  -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  1.0f,	 0.0f,	1.0f,  -1.0f,
		0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,  -1.0f, 0.0f,	0.0f,  -0.5f,
		0.5f,  0.5f,  1.0f,	 1.0f,	1.0f,  1.0f,  0.0f,	 -1.0f, 0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,	 1.0f,	1.0f,  1.0f,  0.0f,	 0.0f,	1.0f,  0.0f,  0.0f,	 0.5f,	0.5f,  -0.5f,
		1.0f,  1.0f,  1.0f,	 1.0f,	0.0f,  1.0f,  0.0f,	 0.0f,	0.5f,  -0.5f, -0.5f, 1.0f,	1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,	 0.0f,	0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,
		0.0f,  0.0f,  0.5f,	 -0.5f, 0.5f,  1.0f,  1.0f,	 1.0f,	0.0f,  1.0f,  1.0f,	 0.0f,	0.0f,  0.5f,
		0.5f,  0.5f,  1.0f,	 1.0f,	1.0f,  0.0f,  0.0f,	 1.0f,	0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f,	1.0f,  1.0f,  0.0f,	 0.0f,	0.0f,  -1.0f, 0.0f,	 0.5f,	-0.5f, -0.5f,
		1.0f,  1.0f,  1.0f,	 1.0f,	0.0f,  0.0f,  -1.0f, 0.0f,	0.5f,  -0.5f, 0.5f,	 1.0f,	1.0f,  1.0f,
		1.0f,  1.0f,  0.0f,	 -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,	1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  0.0f,
		-1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  1.0f,	 1.0f,	0.0f,  1.0f,  0.0f,	 -1.0f, 0.0f,  -0.5f,
		-0.5f, -0.5f, 1.0f,	 1.0f,	1.0f,  0.0f,  0.0f,	 0.0f,	-1.0f, 0.0f,

		-0.5f, 0.5f,  -0.5f, 1.0f,	1.0f,  1.0f,  0.0f,	 0.0f,	0.0f,  1.0f,  0.0f,	 0.5f,	0.5f,  -0.5f,
		1.0f,  1.0f,  1.0f,	 1.0f,	0.0f,  0.0f,  1.0f,	 0.0f,	0.5f,  0.5f,  0.5f,	 1.0f,	1.0f,  1.0f,
		1.0f,  1.0f,  0.0f,	 1.0f,	0.0f,  0.5f,  0.5f,	 0.5f,	1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  0.0f,
		1.0f,  0.0f,  -0.5f, 0.5f,	0.5f,  1.0f,  1.0f,	 1.0f,	0.0f,  1.0f,  0.0f,	 1.0f,	0.0f,  -0.5f,
		0.5f,  -0.5f, 1.0f,	 1.0f,	1.0f,  0.0f,  0.0f,	 0.0f,	1.0f,  0.0f};
	unsigned int VAO;
	unsigned int VBO;

	for (int i = 0; i < 36; i++)
	{
		vertices[i * 11 + 0] *= sx;
		vertices[i * 11 + 1] *= sy;
		vertices[i * 11 + 2] *= sz;

		vertices[i * 11 + 3] = R;
		vertices[i * 11 + 4] = G;
		vertices[i * 11 + 5] = B;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	return VAO;
}

void draw_rectangular_cuboid(const unsigned int VAO, GLuint tex[6])
{
	glBindVertexArray(VAO);
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}
}

//--------------------------------------

unsigned int create_rounded_rectangular_cuboid(float sx, float sy, float sz, const float radius, const float Rt,
											   const float Gt, const float Bt, const float Rm, const float Gm,
											   const float Bm, const float Rb, const float Gb, const float Bb)
{
	float vertices[] = {
		// positions         // colors          // texture coords  //normals
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f,
		-0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f,
		-0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f,
		0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		// corners:
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		// edges:
		// y > 0:
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		//---
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		// y < 0.0:
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		//---
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		//-----------------
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		//------------------------
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f

	};
	int i;
	unsigned int VAO;
	unsigned int VBO;
	sx -= 2 * radius;
	sy -= 2 * radius;
	sz -= 2 * radius;
	for (i = 0; i < 36 + 8 * 3 + 24 * 3; i++)
	{
		vertices[i * 11 + 0] *= sx;
		vertices[i * 11 + 1] *= sy;
		vertices[i * 11 + 2] *= sz;

		vertices[i * 11 + 3] = Rm;
		vertices[i * 11 + 4] = Gm;
		vertices[i * 11 + 5] = Bm;
	}
	for (i = 24; i < 30; i++)
	{
		vertices[i * 11 + 3] = Rb;
		vertices[i * 11 + 4] = Gb;
		vertices[i * 11 + 5] = Bb;
	}
	for (i = 30; i < 36; i++)
	{
		vertices[i * 11 + 3] = Rt;
		vertices[i * 11 + 4] = Gt;
		vertices[i * 11 + 5] = Bt;
	}
	//--------
	// corners:
	for (i = 36; i < 39; i++)
	{
		vertices[i * 11 + 3] = Rt;
		vertices[i * 11 + 4] = Gt;
		vertices[i * 11 + 5] = Bt;
	}
	for (i = 39; i < 42; i++)
	{
		vertices[i * 11 + 3] = Rb;
		vertices[i * 11 + 4] = Gb;
		vertices[i * 11 + 5] = Bb;
	}
	for (i = 42; i < 45; i++)
	{
		vertices[i * 11 + 3] = Rt;
		vertices[i * 11 + 4] = Gt;
		vertices[i * 11 + 5] = Bt;
	}
	for (i = 45; i < 51; i++)
	{
		vertices[i * 11 + 3] = Rb;
		vertices[i * 11 + 4] = Gb;
		vertices[i * 11 + 5] = Bb;
	}
	for (i = 51; i < 57; i++)
	{
		vertices[i * 11 + 3] = Rt;
		vertices[i * 11 + 4] = Gt;
		vertices[i * 11 + 5] = Bt;
	}
	for (i = 57; i < 60; i++)
	{
		vertices[i * 11 + 3] = Rb;
		vertices[i * 11 + 4] = Gb;
		vertices[i * 11 + 5] = Bb;
	}
	//-----------
	// edges:
	for (i = 60; i < 84; i++)
	{
		vertices[i * 11 + 3] = Rt;
		vertices[i * 11 + 4] = Gt;
		vertices[i * 11 + 5] = Bt;
	}
	for (i = 84; i < 108; i++)
	{
		vertices[i * 11 + 3] = Rb;
		vertices[i * 11 + 4] = Gb;
		vertices[i * 11 + 5] = Bb;
	}

	for (i = 0; i < 6; i++)
		vertices[i * 11 + 2] -= radius;
	for (i = 6; i < 12; i++)
		vertices[i * 11 + 2] += radius;
	for (i = 12; i < 18; i++)
		vertices[i * 11 + 0] -= radius;
	for (i = 18; i < 24; i++)
		vertices[i * 11 + 0] += radius;
	for (i = 24; i < 30; i++)
		vertices[i * 11 + 1] -= radius;
	for (i = 30; i < 36; i++)
		vertices[i * 11 + 1] += radius;

	// Corners:
	for (i = 36; i < 36 + 8 * 3; i += 3)
	{
		vertices[i * 11 + 1] += radius * vertices[i * 11 + 9];
		vertices[(i + 1) * 11 + 2] += radius * vertices[(i + 1) * 11 + 10];
		vertices[(i + 2) * 11 + 0] += radius * vertices[(i + 2) * 11 + 8];
	}

	// edges:
	// y > 0:
	vertices[60 * 11 + 2] += radius;
	vertices[61 * 11 + 1] += radius;
	vertices[62 * 11 + 2] += radius;

	vertices[63 * 11 + 1] += radius;
	vertices[64 * 11 + 1] += radius;
	vertices[65 * 11 + 2] += radius;

	vertices[66 * 11 + 2] -= radius;
	vertices[67 * 11 + 1] += radius;
	vertices[68 * 11 + 2] -= radius;

	vertices[69 * 11 + 1] += radius;
	vertices[70 * 11 + 1] += radius;
	vertices[71 * 11 + 2] -= radius;

	//----
	vertices[72 * 11 + 0] += radius;
	vertices[73 * 11 + 0] += radius;
	vertices[74 * 11 + 1] += radius;

	vertices[75 * 11 + 1] += radius;
	vertices[76 * 11 + 0] += radius;
	vertices[77 * 11 + 1] += radius;

	vertices[78 * 11 + 0] -= radius;
	vertices[79 * 11 + 0] -= radius;
	vertices[80 * 11 + 1] += radius;

	vertices[81 * 11 + 1] += radius;
	vertices[82 * 11 + 0] -= radius;
	vertices[83 * 11 + 1] += radius;
	// y < 0:
	vertices[84 * 11 + 2] += radius;
	vertices[85 * 11 + 1] -= radius;
	vertices[86 * 11 + 2] += radius;

	vertices[87 * 11 + 1] -= radius;
	vertices[88 * 11 + 1] -= radius;
	vertices[89 * 11 + 2] += radius;

	vertices[90 * 11 + 2] -= radius;
	vertices[91 * 11 + 1] -= radius;
	vertices[92 * 11 + 2] -= radius;

	vertices[93 * 11 + 1] -= radius;
	vertices[94 * 11 + 1] -= radius;
	vertices[95 * 11 + 2] -= radius;

	//----
	vertices[96 * 11 + 0] += radius;
	vertices[97 * 11 + 0] += radius;
	vertices[98 * 11 + 1] -= radius;

	vertices[99 * 11 + 1] -= radius;
	vertices[100 * 11 + 0] += radius;
	vertices[101 * 11 + 1] -= radius;

	vertices[102 * 11 + 0] -= radius;
	vertices[103 * 11 + 0] -= radius;
	vertices[104 * 11 + 1] -= radius;

	vertices[105 * 11 + 1] -= radius;
	vertices[106 * 11 + 0] -= radius;
	vertices[107 * 11 + 1] -= radius;
	//------------
	vertices[108 * 11 + 0] += radius;
	vertices[109 * 11 + 2] += radius;
	vertices[110 * 11 + 0] += radius;

	vertices[111 * 11 + 2] += radius;
	vertices[112 * 11 + 0] += radius;
	vertices[113 * 11 + 2] += radius;

	vertices[114 * 11 + 0] -= radius;
	vertices[115 * 11 + 2] += radius;
	vertices[116 * 11 + 0] -= radius;

	vertices[117 * 11 + 2] += radius;
	vertices[118 * 11 + 0] -= radius;
	vertices[119 * 11 + 2] += radius;
	//---------------
	vertices[120 * 11 + 0] += radius;
	vertices[121 * 11 + 2] -= radius;
	vertices[122 * 11 + 0] += radius;

	vertices[123 * 11 + 2] -= radius;
	vertices[124 * 11 + 0] += radius;
	vertices[125 * 11 + 2] -= radius;

	vertices[126 * 11 + 0] -= radius;
	vertices[127 * 11 + 2] -= radius;
	vertices[128 * 11 + 0] -= radius;

	vertices[129 * 11 + 2] -= radius;
	vertices[130 * 11 + 0] -= radius;
	vertices[131 * 11 + 2] -= radius;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	return VAO;
}

void draw_rounded_rectangular_cuboid(const unsigned int VAO, GLuint tex[7])
{
	glBindVertexArray(VAO);
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}
	// Corners:
	glBindTexture(GL_TEXTURE_2D, tex[6]);
	glDrawArrays(GL_TRIANGLES, 36, 8 * 3 + 24 * 3);
}

//--------------------------------------

unsigned int create_ellipsoid(const float a, const float b, const float c, const float R, const float G, const float B,
							  const int slices)
{
	unsigned int VAO;
	unsigned int VBO;
	float xb, yb, zb;
	float C[3];
	int t = 0;
	constexpr int dangle = 1; // 2;

	const int n = slices * (360 / dangle + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int i = 0; i < slices; i++)
	{
		const float w0 = static_cast<float>(i) / static_cast<float>(slices);
		const float w1 = static_cast<float>(i + 1) / static_cast<float>(slices);

		const float z0 = (-c) * (1.0 - w0) + c * w0;
		const float z1 = (-c) * (1.0 - w1) + c * w1;

		const float b0 = sqrtf(b * b * (1.0 - (z0 * z0) / (c * c)));
		const float b1 = sqrtf(b * b * (1.0 - (z1 * z1) / (c * c)));

		const float a0 = sqrtf(a * a * (1.0 - (z0 * z0) / (c * c)));
		const float a1 = sqrtf(a * a * (1.0 - (z1 * z1) / (c * c)));

		// glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= 360; j += dangle)
		{
			const float angle = static_cast<float>(j) * (PI / 180.0f);
			const float cos_angle = cosf(angle);
			const float sin_angle = sinf(angle);
			const float x0 = a0 * cos_angle;
			const float y0 = b0 * sin_angle;
			const float x1 = a1 * cos_angle;
			const float y1 = b1 * sin_angle;

			C[0] = (2.0 * x0) / (a * a); // b0*cos_angle;
			C[1] = (2.0 * y0) / (b * b); // y0;
			C[2] = (2.0 * z0) / (c * c); // b0*cos_angle;
			float mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x0, y0, z0 );
			vertices[k] = x0;
			k++;
			vertices[k] = y0;
			k++;
			vertices[k] = z0;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = (2.0 * x1) / (a * a); // b1*cos_angle;
			C[1] = (2.0 * y1) / (b * b); // y1;
			C[2] = (2.0 * z1) / (c * c); // b1*cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x1, y1, z1 );
			vertices[k] = x1;
			k++;
			vertices[k] = y1;
			k++;
			vertices[k] = z1;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_ellipsoid(const unsigned int VAO, const int slices)
{
	constexpr int dangle = 1; // 2;

	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (360 / dangle + 1) * 2);
		k += (360 / dangle + 1) * 2;
	}
}

//-------------------------------

// lune_z
unsigned int create_ellipsoid_lune_cap_z(const float a, const float b, const float c, const float z_0, const float z_1,
										 const int angle1, const int angle2, const float R, const float G,
										 const float B, const int slices)
{
	unsigned int VAO;
	unsigned int VBO;
	float xb, yb, zb;
	float C[3];
	int t = 0;

	const int n = slices * (angle2 - angle1 + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int i = 0; i < slices; i++)
	{
		const float w0 = static_cast<float>(i) / static_cast<float>(slices);
		const float w1 = static_cast<float>(i + 1) / static_cast<float>(slices);

		const float z0 = z_0 * (1.0 - w0) + z_1 * w0;
		const float z1 = z_0 * (1.0 - w1) + z_1 * w1;

		const float b0 = sqrtf(b * b * (1.0 - (z0 * z0) / (c * c)));
		const float b1 = sqrtf(b * b * (1.0 - (z1 * z1) / (c * c)));

		const float a0 = sqrtf(a * a * (1.0 - (z0 * z0) / (c * c)));
		const float a1 = sqrtf(a * a * (1.0 - (z1 * z1) / (c * c)));

		// glBegin(GL_QUAD_STRIP);
		for (int j = angle1; j <= angle2; j++)
		{
			const float angle = static_cast<float>(j) * (PI / 180.0f);
			const float cos_angle = cosf(angle);
			const float sin_angle = sinf(angle);
			const float x0 = a0 * cos_angle;
			const float y0 = b0 * sin_angle;
			const float x1 = a1 * cos_angle;
			const float y1 = b1 * sin_angle;

			C[0] = (2.0 * x0) / (a * a); // b0*cos_angle;
			C[1] = (2.0 * y0) / (b * b); // y0;
			C[2] = (2.0 * z0) / (c * c); // b0*cos_angle;
			float mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x0, y0, z0 );
			vertices[k] = x0;
			k++;
			vertices[k] = y0;
			k++;
			vertices[k] = z0;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = (2.0 * x1) / (a * a); // b1*cos_angle;
			C[1] = (2.0 * y1) / (b * b); // y1;
			C[2] = (2.0 * z1) / (c * c); // b1*cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x1, y1, z1 );
			vertices[k] = x1;
			k++;
			vertices[k] = y1;
			k++;
			vertices[k] = z1;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_ellipsoid_lune_cap_z(const unsigned int VAO, const int angle1, const int angle2, const int slices)
{

	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2 - angle1 + 1) * 2);
		k += (angle2 - angle1 + 1) * 2;
	}
}

//-------------------------------

// lune_z
unsigned int create_ellipsoid_lune_z(const float a, const float b, const float c, const int angle1, const int angle2,
									 const float R, const float G, const float B, const int slices)
{
	unsigned int VAO;
	unsigned int VBO;
	float xb, yb, zb;
	float C[3];
	int t = 0;

	const int n = slices * (angle2 - angle1 + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int i = 0; i < slices; i++)
	{
		const float w0 = static_cast<float>(i) / static_cast<float>(slices);
		const float w1 = static_cast<float>(i + 1) / static_cast<float>(slices);

		const float z0 = (-c) * (1.0 - w0) + c * w0;
		const float z1 = (-c) * (1.0 - w1) + c * w1;

		const float b0 = sqrtf(b * b * (1.0 - (z0 * z0) / (c * c)));
		const float b1 = sqrtf(b * b * (1.0 - (z1 * z1) / (c * c)));

		const float a0 = sqrtf(a * a * (1.0 - (z0 * z0) / (c * c)));
		const float a1 = sqrtf(a * a * (1.0 - (z1 * z1) / (c * c)));

		// glBegin(GL_QUAD_STRIP);
		for (int j = angle1; j <= angle2; j++)
		{
			const float angle = static_cast<float>(j) * (PI / 180.0f);
			const float cos_angle = cosf(angle);
			const float sin_angle = sinf(angle);
			const float x0 = a0 * cos_angle;
			const float y0 = b0 * sin_angle;
			const float x1 = a1 * cos_angle;
			const float y1 = b1 * sin_angle;

			C[0] = (2.0 * x0) / (a * a); // b0*cos_angle;
			C[1] = (2.0 * y0) / (b * b); // y0;
			C[2] = (2.0 * z0) / (c * c); // b0*cos_angle;
			float mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x0, y0, z0 );
			vertices[k] = x0;
			k++;
			vertices[k] = y0;
			k++;
			vertices[k] = z0;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = (2.0 * x1) / (a * a); // b1*cos_angle;
			C[1] = (2.0 * y1) / (b * b); // y1;
			C[2] = (2.0 * z1) / (c * c); // b1*cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x1, y1, z1 );
			vertices[k] = x1;
			k++;
			vertices[k] = y1;
			k++;
			vertices[k] = z1;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_ellipsoid_lune_z(const unsigned int VAO, const int angle1, const int angle2, const int slices)
{

	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2 - angle1 + 1) * 2);
		k += (angle2 - angle1 + 1) * 2;
	}
}

//-----------------------------------------

unsigned int create_ellipsoid_lune_x(const float a, const float b, const float c, const int angle1, const int angle2,
									 const float R, const float G, const float B, const int slices)
{
	unsigned int VAO;
	unsigned int VBO;
	float xb, yb, zb;
	float C[3];
	int t = 0;

	const int n = slices * (angle2 - angle1 + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int i = 0; i < slices; i++)
	{
		const float w0 = static_cast<float>(i) / static_cast<float>(slices);
		const float w1 = static_cast<float>(i + 1) / static_cast<float>(slices);

		const float x0 = (-a) * (1.0 - w0) + a * w0;
		const float x1 = (-a) * (1.0 - w1) + a * w1;

		const float b0 = sqrtf(b * b * (1.0 - (x0 * x0) / (a * a)));
		const float b1 = sqrtf(b * b * (1.0 - (x1 * x1) / (a * a)));

		const float c0 = sqrtf(c * c * (1.0 - (x0 * x0) / (a * a)));
		const float c1 = sqrtf(c * c * (1.0 - (x1 * x1) / (a * a)));

		// glBegin(GL_QUAD_STRIP);
		for (int j = angle1; j <= angle2; j++)
		{
			const float angle = static_cast<float>(j) * (PI / 180.0f);
			const float cos_angle = cosf(angle);
			const float sin_angle = sinf(angle);
			const float z0 = c0 * cos_angle;
			const float y0 = b0 * sin_angle;
			const float z1 = c1 * cos_angle;
			const float y1 = b1 * sin_angle;

			C[0] = (2.0 * x0) / (a * a); // b0*cos_angle;
			C[1] = (2.0 * y0) / (b * b); // y0;
			C[2] = (2.0 * z0) / (c * c); // b0*cos_angle;
			float mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x0, y0, z0 );
			vertices[k] = x0;
			k++;
			vertices[k] = y0;
			k++;
			vertices[k] = z0;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = (2.0 * x1) / (a * a); // b1*cos_angle;
			C[1] = (2.0 * y1) / (b * b); // y1;
			C[2] = (2.0 * z1) / (c * c); // b1*cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x1, y1, z1 );
			vertices[k] = x1;
			k++;
			vertices[k] = y1;
			k++;
			vertices[k] = z1;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_ellipsoid_lune_x(const unsigned int VAO, const int angle1, const int angle2, const int slices)
{

	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2 - angle1 + 1) * 2);
		k += (angle2 - angle1 + 1) * 2;
	}
}

//-----------------------------------------

unsigned int create_ellipsoid_lune_cap_x(const float a, const float b, const float c, const float x_0, const float x_1,
										 const int angle1, const int angle2, const float R, const float G,
										 const float B, const int slices)
{
	unsigned int VAO;
	unsigned int VBO;
	float xb, yb, zb;
	float C[3];
	int t = 0;

	const int n = slices * (angle2 - angle1 + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));
	for (int i = 0; i < slices; i++)
	{
		const float w0 = static_cast<float>(i) / static_cast<float>(slices);
		const float w1 = static_cast<float>(i + 1) / static_cast<float>(slices);

		const float x0 = x_0 * (1.0 - w0) + x_1 * w0;
		const float x1 = x_0 * (1.0 - w1) + x_1 * w1;

		const float b0 = sqrtf(b * b * (1.0 - (x0 * x0) / (a * a)));
		const float b1 = sqrtf(b * b * (1.0 - (x1 * x1) / (a * a)));

		const float c0 = sqrtf(c * c * (1.0 - (x0 * x0) / (a * a)));
		const float c1 = sqrtf(c * c * (1.0 - (x1 * x1) / (a * a)));

		// glBegin(GL_QUAD_STRIP);
		for (int j = angle1; j <= angle2; j++)
		{
			const float angle = static_cast<float>(j) * (PI / 180.0f);
			const float cos_angle = cosf(angle);
			const float sin_angle = sinf(angle);
			const float z0 = c0 * cos_angle;
			const float y0 = b0 * sin_angle;
			const float z1 = c1 * cos_angle;
			const float y1 = b1 * sin_angle;

			C[0] = (2.0 * x0) / (a * a); // b0*cos_angle;
			C[1] = (2.0 * y0) / (b * b); // y0;
			C[2] = (2.0 * z0) / (c * c); // b0*cos_angle;
			float mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x0, y0, z0 );
			vertices[k] = x0;
			k++;
			vertices[k] = y0;
			k++;
			vertices[k] = z0;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = (2.0 * x1) / (a * a); // b1*cos_angle;
			C[1] = (2.0 * y1) / (b * b); // y1;
			C[2] = (2.0 * z1) / (c * c); // b1*cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);

			// glVertex3f( x1, y1, z1 );
			vertices[k] = x1;
			k++;
			vertices[k] = y1;
			k++;
			vertices[k] = z1;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_ellipsoid_lune_cap_x(const unsigned int VAO, const int angle1, const int angle2, const int slices)
{

	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (angle2 - angle1 + 1) * 2);
		k += (angle2 - angle1 + 1) * 2;
	}
}

//-----------------------------------------

// plane zy (rotation axis = x)
unsigned int create_curved_cylinder_x(float radius0, float radius1, float bend_radius, float bend_ang0, float bend_ang1,
									  float R, float G, float B, int slices, int delta)
{
	unsigned int VAO;
	unsigned int VBO;
	float *vertices;
	float w0, w1, ang0, ang1, angle, x0, y0, x1, y1, xb, yb, zb;
	float cos_angle, sin_angle, cos_ang0, sin_ang0, cos_ang1, sin_ang1;
	float rad0, rad1, mag;
	float C[3];
	int t = 0;
	int i, j, k, n;

	n = slices * (360 / delta + 1) * 2 * 11;
	k = 0;
	vertices = static_cast<float *>(calloc(n, sizeof(float)));

	bend_ang0 *= (PI / 180.0f);
	bend_ang1 *= (PI / 180.0f);
	for (i = 0; i < slices; i++)
	{
		w0 = static_cast<float>(i) / static_cast<float>(slices);
		w1 = static_cast<float>(i + 1) / static_cast<float>(slices);
		ang0 = bend_ang0 * (1.0 - w0) + bend_ang1 * w0;
		ang1 = bend_ang0 * (1.0 - w1) + bend_ang1 * w1;

		rad0 = radius0 * (1.0 - w0) + radius1 * w0;
		rad1 = radius0 * (1.0 - w1) + radius1 * w1;

		sin_ang0 = sinf(ang0);
		cos_ang0 = cosf(ang0);
		sin_ang1 = sinf(ang1);
		cos_ang1 = cosf(ang1);

		// glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= 360; j += delta)
		{
			angle = static_cast<float>(j) * (PI / 180.0f);
			cos_angle = cosf(angle);
			sin_angle = sinf(angle);
			y0 = rad0 * cos_angle + bend_radius;
			x0 = rad0 * sin_angle;
			y1 = rad1 * cos_angle + bend_radius;
			x1 = rad1 * sin_angle;

			C[0] = x0;
			C[1] = sin_ang0 * rad0 * cos_angle;
			C[2] = cos_ang0 * rad0 * cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);
			yb = sin_ang0 * y0;
			xb = x0;
			zb = cos_ang0 * y0;
			// glVertex3f( xb, yb, zb );
			vertices[k] = xb;
			k++;
			vertices[k] = yb;
			k++;
			vertices[k] = zb;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = x1;
			C[1] = sin_ang1 * rad1 * cos_angle;
			C[2] = cos_ang1 * rad1 * cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);
			yb = sin_ang1 * y1;
			xb = x1;
			zb = cos_ang1 * y1;
			// glVertex3f( xb, yb, zb );
			vertices[k] = xb;
			k++;
			vertices[k] = yb;
			k++;
			vertices[k] = zb;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_curved_cylinder_x(const unsigned int VAO, const int slices, const int delta)
{
	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (360 / delta + 1) * 2);
		k += (360 / delta + 1) * 2;
	}
}

//-----------------------------------------

// plane zy (rotation axis = x)
unsigned int create_torus_x(const float radius, const float bend_radius, const float R, const float G, const float B,
							const int slices, const int delta)
{
	return create_curved_cylinder_x(radius, radius, bend_radius, 0.0, 360.0, R, G, B, slices, delta);
}

void draw_torus_x(const unsigned int VAO, const int slices, const int delta)
{
	draw_curved_cylinder_x(VAO, slices, delta);
}

//-----------------------------------------

// plane zx (rotation axis = y)
unsigned int create_curved_cylinder_y(float radius0, float radius1, float bend_radius, float bend_ang0, float bend_ang1,
									  float R, float G, float B, int slices, int delta)
{
	unsigned int VAO;
	unsigned int VBO;
	float *vertices;
	float w0, w1, ang0, ang1, angle, x0, y0, x1, y1, xb, yb, zb;
	float cos_angle, sin_angle, cos_ang0, sin_ang0, cos_ang1, sin_ang1;
	float rad0, rad1, mag;
	float C[3];
	int t = 0;
	int i, j, k, n;

	n = slices * (360 / delta + 1) * 2 * 11;
	k = 0;
	vertices = static_cast<float *>(calloc(n, sizeof(float)));

	bend_ang0 *= (PI / 180.0f);
	bend_ang1 *= (PI / 180.0f);
	for (i = 0; i < slices; i++)
	{
		w0 = static_cast<float>(i) / static_cast<float>(slices);
		w1 = static_cast<float>(i + 1) / static_cast<float>(slices);
		ang0 = bend_ang0 * (1.0 - w0) + bend_ang1 * w0;
		ang1 = bend_ang0 * (1.0 - w1) + bend_ang1 * w1;

		rad0 = radius0 * (1.0 - w0) + radius1 * w0;
		rad1 = radius0 * (1.0 - w1) + radius1 * w1;

		sin_ang0 = sinf(ang0);
		cos_ang0 = cosf(ang0);
		sin_ang1 = sinf(ang1);
		cos_ang1 = cosf(ang1);

		// glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= 360; j += delta)
		{
			angle = static_cast<float>(j) * (PI / 180.0f);
			cos_angle = cosf(angle);
			sin_angle = sinf(angle);
			x0 = rad0 * cos_angle + bend_radius;
			y0 = rad0 * sin_angle;
			x1 = rad1 * cos_angle + bend_radius;
			y1 = rad1 * sin_angle;

			C[0] = sin_ang0 * rad0 * cos_angle;
			C[1] = y0;
			C[2] = cos_ang0 * rad0 * cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);
			xb = sin_ang0 * x0;
			yb = y0;
			zb = cos_ang0 * x0;
			// glVertex3f( xb, yb, zb );
			vertices[k] = xb;
			k++;
			vertices[k] = yb;
			k++;
			vertices[k] = zb;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 0.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			C[0] = sin_ang1 * rad1 * cos_angle;
			C[1] = y1;
			C[2] = cos_ang1 * rad1 * cos_angle;
			mag = magnitude(C);
			C[0] /= mag;
			C[1] /= mag;
			C[2] /= mag;
			// glNormal3f(C[0], C[1], C[2]);
			xb = sin_ang1 * x1;
			yb = y1;
			zb = cos_ang1 * x1;
			// glVertex3f( xb, yb, zb );
			vertices[k] = xb;
			k++;
			vertices[k] = yb;
			k++;
			vertices[k] = zb;
			k++;

			// Colors:
			vertices[k] = R;
			k++;
			vertices[k] = G;
			k++;
			vertices[k] = B;
			k++;

			// Texture:
			vertices[k] = t;
			k++;
			vertices[k] = 1.0;
			k++;

			// Normals:
			vertices[k] = C[0];
			k++;
			vertices[k] = C[1];
			k++;
			vertices[k] = C[2];
			k++;

			t++;
			t = t % 2;
		}
		// glEnd();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_curved_cylinder_y(const unsigned int VAO, const int slices, const int delta)
{
	glBindVertexArray(VAO);
	int k = 0;
	for (int i = 0; i < slices; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, k, (360 / delta + 1) * 2);
		k += (360 / delta + 1) * 2;
	}
}

//-----------------------------------------------

// plane zx (rotation axis = y)
unsigned int create_torus_y(const float radius, const float bend_radius, const float R, const float G, const float B,
							const int slices, const int delta)
{
	return create_curved_cylinder_y(radius, radius, bend_radius, 0.0, 360.0, R, G, B, slices, delta);
}

void draw_torus_y(const unsigned int VAO, const int slices, const int delta)
{
	draw_curved_cylinder_y(VAO, slices, delta);
}

//-----------------------------------------

unsigned int create_cylinder_z(const float radius0, const float radius1, const float z0, const float z1, const float R,
							   const float G, const float B, const int delta)
{
	unsigned int VAO;
	unsigned int VBO;
	float C[3];

	const int n = (360 / delta + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));

	// glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += delta)
	{
		const float angle = static_cast<float>(j) * (PI / 180.0f);
		const float cos_angle = cosf(angle);
		const float sin_angle = sinf(angle);
		const float y0 = radius0 * cos_angle;
		const float x0 = radius0 * sin_angle;
		const float y1 = radius1 * cos_angle;
		const float x1 = radius1 * sin_angle;

		C[0] = x0;
		C[1] = y0;
		C[2] = 0;
		float mag = magnitude(C);
		C[0] /= mag;
		C[1] /= mag;
		C[2] /= mag;
		// glNormal3f(C[0], C[1], C[2]);
		float yb = y0;
		float xb = x0;
		float zb = z0;
		// glVertex3f( xb, yb, zb );
		vertices[k] = xb;
		k++;
		vertices[k] = yb;
		k++;
		vertices[k] = zb;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = j / 360.0;
		k++;
		vertices[k] = 0.0;
		k++;

		// Normals:
		vertices[k] = C[0];
		k++;
		vertices[k] = C[1];
		k++;
		vertices[k] = C[2];
		k++;

		C[0] = x1;
		C[1] = y1;
		C[2] = 0;
		mag = magnitude(C);
		C[0] /= mag;
		C[1] /= mag;
		C[2] /= mag;
		// glNormal3f(C[0], C[1], C[2]);
		yb = y1;
		xb = x1;
		zb = z1;
		// glVertex3f( xb, yb, zb );
		vertices[k] = xb;
		k++;
		vertices[k] = yb;
		k++;
		vertices[k] = zb;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = j / 360.0;
		k++;
		vertices[k] = 1.0;
		k++;

		// Normals:
		vertices[k] = C[0];
		k++;
		vertices[k] = C[1];
		k++;
		vertices[k] = C[2];
		k++;
	}
	// glEnd();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_cylinder_z(const unsigned int VAO, const int delta)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (360 / delta + 1) * 2);
}

//-----------------------------------------

unsigned int create_cylinder_y(const float radius0, const float radius1, const float y0, const float y1, const float R,
							   const float G, const float B, const int delta)
{
	unsigned int VAO;
	unsigned int VBO;
	float C[3];

	const int n = (360 / delta + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));

	// glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += delta)
	{
		const float angle = static_cast<float>(j) * (PI / 180.0f);
		const float cos_angle = cosf(angle);
		const float sin_angle = sinf(angle);
		const float z0 = radius0 * cos_angle;
		const float x0 = radius0 * sin_angle;
		const float z1 = radius1 * cos_angle;
		const float x1 = radius1 * sin_angle;

		C[0] = x0;
		C[1] = 0;
		C[2] = z0;
		float mag = magnitude(C);
		C[0] /= mag;
		C[1] /= mag;
		C[2] /= mag;
		// glNormal3f(C[0], C[1], C[2]);
		float yb = y0;
		float xb = x0;
		float zb = z0;
		// glVertex3f( xb, yb, zb );
		vertices[k] = xb;
		k++;
		vertices[k] = yb;
		k++;
		vertices[k] = zb;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = j / 360.0;
		k++;
		vertices[k] = 0.0;
		k++;

		// Normals:
		vertices[k] = C[0];
		k++;
		vertices[k] = C[1];
		k++;
		vertices[k] = C[2];
		k++;

		C[0] = x1;
		C[1] = 0;
		C[2] = z1;
		mag = magnitude(C);
		C[0] /= mag;
		C[1] /= mag;
		C[2] /= mag;
		// glNormal3f(C[0], C[1], C[2]);
		yb = y1;
		xb = x1;
		zb = z1;
		// glVertex3f( xb, yb, zb );
		vertices[k] = xb;
		k++;
		vertices[k] = yb;
		k++;
		vertices[k] = zb;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = j / 360.0;
		k++;
		vertices[k] = 1.0;
		k++;

		// Normals:
		vertices[k] = C[0];
		k++;
		vertices[k] = C[1];
		k++;
		vertices[k] = C[2];
		k++;
	}
	// glEnd();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_cylinder_y(const unsigned int VAO, const int delta)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (360 / delta + 1) * 2);
}

void draw_cylinder_y(const unsigned int VAO, const int delta, const GLuint tex)
{
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (360 / delta + 1) * 2);
}

//--------------------------------------------

unsigned int create_elliptic_cylinder_z(const float a0, const float b0, const float a1, const float b1, const float z0,
										const float z1, const float R, const float G, const float B, const int delta)
{
	unsigned int VAO;
	unsigned int VBO;
	float C[3];
	int t = 0;

	const int n = (360 / delta + 1) * 2 * 11;
	int k = 0;
	float *vertices = static_cast<float *>(calloc(n, sizeof(float)));

	// glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 360; j += delta)
	{
		const float angle = static_cast<float>(j) * (PI / 180.0f);
		const float cos_angle = cosf(angle);
		const float sin_angle = sinf(angle);
		const float y0 = b0 * cos_angle;
		const float x0 = a0 * sin_angle;
		const float y1 = b1 * cos_angle;
		const float x1 = a1 * sin_angle;

		C[0] = x0 / (a0 * a0);
		C[1] = y0 / (b0 * b0);
		C[2] = 0;
		float mag = magnitude(C);
		C[0] /= mag;
		C[1] /= mag;
		C[2] /= mag;
		// glNormal3f(C[0], C[1], C[2]);
		float yb = y0;
		float xb = x0;
		float zb = z0;
		// glVertex3f( xb, yb, zb );
		vertices[k] = xb;
		k++;
		vertices[k] = yb;
		k++;
		vertices[k] = zb;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = t;
		k++;
		vertices[k] = 0.0;
		k++;

		// Normals:
		vertices[k] = C[0];
		k++;
		vertices[k] = C[1];
		k++;
		vertices[k] = C[2];
		k++;

		C[0] = x1 / (a1 * a1);
		C[1] = y1 / (b1 * b1);
		C[2] = 0;
		mag = magnitude(C);
		C[0] /= mag;
		C[1] /= mag;
		C[2] /= mag;
		// glNormal3f(C[0], C[1], C[2]);
		yb = y1;
		xb = x1;
		zb = z1;
		// glVertex3f( xb, yb, zb );
		vertices[k] = xb;
		k++;
		vertices[k] = yb;
		k++;
		vertices[k] = zb;
		k++;

		// Colors:
		vertices[k] = R;
		k++;
		vertices[k] = G;
		k++;
		vertices[k] = B;
		k++;

		// Texture:
		vertices[k] = t;
		k++;
		vertices[k] = 1.0;
		k++;

		// Normals:
		vertices[k] = C[0];
		k++;
		vertices[k] = C[1];
		k++;
		vertices[k] = C[2];
		k++;

		t++;
		t = t % 2;
	}
	// glEnd();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	free(vertices);
	return VAO;
}

void draw_elliptic_cylinder_z(const unsigned int VAO, const int delta)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (360 / delta + 1) * 2);
}
