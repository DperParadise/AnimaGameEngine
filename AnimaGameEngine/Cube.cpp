#include "Cube.h"
#include "Globals.h"
#include <devil/ilu.h>

Cube::Cube(){}
Cube::~Cube() {}

void Cube::Init()
{
	//Checkered texture
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkeredImage[i][j][0] = (GLubyte)c;
			checkeredImage[i][j][1] = (GLubyte)c;
			checkeredImage[i][j][2] = (GLubyte)c;
			checkeredImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkeredImage);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Image texture Lenna
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoad(IL_TGA, "images/Lenna.tga");
	if (ilGetError() != IL_NO_ERROR)
		MYLOG(iluErrorString(ilGetError()));

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	uint img_widht = ilGetInteger(IL_IMAGE_WIDTH);
	uint img_height = ilGetInteger(IL_IMAGE_HEIGHT);
	image_data = ilGetData();

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_widht, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	iluDeleteImage(image);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cube::Draw()
{
	//glBindTexture(GL_TEXTURE_2D, textures[0]); //checkered texture
	glBindTexture(GL_TEXTURE_2D, textures[1]);  //Lenna texture
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colour);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texture);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-0.5f, -0.5f, -0.5f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPopMatrix();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cube::Clear()
{
	
}