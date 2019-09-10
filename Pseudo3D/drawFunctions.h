# include <stdlib.h>
# include <stdio.h>
# include "openGlFunction.h"
# include "bmp.h"
# include <math.h>
#define GL_CLAMP_TO_EDGE 0x812F

typedef struct {float R; float G; float B;} Pixel;
typedef struct {int X; int Y; int W; int H;} Rect;
typedef struct { GLuint textureHandle; GLbyte *pixels; } Texture;
typedef struct { GLuint textureHandle; GLbyte *pixels; Rect UVrect; int columns; int rows; int w; int h; } AnimTexture;

void iniRect(Rect *rec, int x, int y, int w, int h){
    rec->X =x;
    rec->Y =y;
    rec->W =w;
    rec->H =h;
}

void drawRect(Rect *R, float r, float g, float b){
    glBegin(GL_QUADS);
        glColor3f(r,g,b);
        glVertex2i(R->X, R->Y);
        glVertex2i(R->X+R->W, R->Y);
        glVertex2i(R->X+R->W, R->Y+R->H);
        glVertex2i(R->X, R->Y+R->H);
    glEnd();

}

void BeginDrawSection(){
	glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RefreshBuffer(HDC *hdc){
    SwapBuffers(*hdc);
}

void createTexture(Texture *tex, char *path) {

	BITMAPINFOHEADER info;
	tex->pixels = LoadBitmapFile(path, &info);
	glGenTextures(1, &(tex->textureHandle));
	glBindTexture(GL_TEXTURE_2D, tex->textureHandle);
	if (info.biBitCount == 32) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, info.biWidth, info.biHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tex->pixels);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, info.biWidth, info.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tex->pixels);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}


void createTextureA(Texture *tex, char *path, int w, int h) {

	BITMAPINFOHEADER info;
	tex->pixels = LoadBitmapFile(path, &info);
	glGenTextures(1, &(tex->textureHandle));
	glBindTexture(GL_TEXTURE_2D, tex->textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tex->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void createAnimTexture(AnimTexture *tex, char *path, int w, int h,Rect UVrect) {
	tex->UVrect = UVrect;
	tex->w = w;
	tex->h = h;
	BITMAPINFOHEADER info;
	tex->pixels = LoadBitmapFile(path, &info);
	glGenTextures(1, &(tex->textureHandle));
	glBindTexture(GL_TEXTURE_2D, tex->textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tex->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}




void drawTexture(Texture tex, Rect *rec, Rect *UV) {
	Rect nullRect;
	iniRect(&nullRect, 0, 0, 1, 1);
	glBindTexture(GL_TEXTURE_2D, tex.textureHandle);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (UV == NULL) {
		UV = &nullRect;
	}

	glBegin(GL_QUADS);

	glColor4f(1.0f, 1.0f, 1.0f,1.0f);

	glTexCoord2i(UV->X, UV->Y + UV->H);
	glVertex2i(rec->X, rec->Y);

	glTexCoord2i(UV->X + UV->W, UV->Y + UV->H);
	glVertex2i(rec->X + rec->W, rec->Y);


	glTexCoord2i(UV->X + UV->W, UV->Y);
	glVertex2i(rec->X + rec->W, rec->Y + rec->H);

	glTexCoord2i(UV->X, UV->Y);
	glVertex2i(rec->X, rec->Y + rec->H);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void drawAnimTexture(AnimTexture tex, Rect rec, int framex, int framey) {

	int x=0, y=0;
	float xfi=0, yfi=0, xfe=0, yfe=0, interval;



	xfi = (1 / (float)tex.w)*(float)tex.UVrect.X;
	yfi = 1-((1 / (float)tex.h)*((float)tex.UVrect.Y+(float)(tex.UVrect.H)));
	xfe = (1 / (float)tex.w)*((float)tex.UVrect.X + (float)(tex.UVrect.W));
	yfe = 1-((1 / (float)tex.h)*(float)tex.UVrect.Y);


	xfi = (float)tex.UVrect.X/ tex.w;
	yfi = 1 - (((float)tex.UVrect.Y+ (float)tex.UVrect.H) / tex.h) ;
	xfe = (((float)tex.UVrect.X + (float)tex.UVrect.W) / tex.w);
	yfe = 1 - ((float)tex.UVrect.Y / tex.h);
	


	interval = xfe - xfi;
	xfi += (interval*framex);
	xfe += (interval*framex);

	interval = yfe - yfi;
	yfi -= (interval*framey);
	yfe -= (interval*framey);


	glBindTexture(GL_TEXTURE_2D, tex.textureHandle);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glBegin(GL_QUADS);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glTexCoord2f(xfi, yfe);
	glVertex2i(rec.X, rec.Y);

	glTexCoord2f(xfe, yfe);
	glVertex2i(rec.X+rec.W, rec.Y);


	glTexCoord2f(xfe, yfi);
	glVertex2i(rec.X + rec.W, rec.Y+ rec.H);

	glTexCoord2f(xfi, yfi);
	glVertex2i(rec.X , rec.Y+ rec.H);

	
	glEnd();
}

void drawLine(int x1, int y1, int x2, int y2, float R, float G, float B) {
	glBegin(GL_LINES);
		glColor3f(R, G, B);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void drawCircle(int x, int y, int r, float R, float G, float B) {
	float nx=0, ny=0, i;
	glBegin(GL_LINE_LOOP);
		glColor3f(R, G, B);

		for (i = 0; i < 2* 3.1415926535; i+=0.1) {
			nx = cos(i)*r;
			ny = sin(i)*r;
			nx += x;
			ny += y;
			glVertex2i(nx, ny);
		}
	glEnd();
	
}


