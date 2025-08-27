#ifndef __3D_GENERAL_H
#define __3D_GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <map>

// Include GLEW
#include <GL/glew.h>
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vboindexer.hpp"
#include "Timer.hpp"

using namespace std;
using namespace glm;

#define CLIP_WIDTH 100
#define CLIP_HEIGHT 100

#define PATH_IMAGE_1 "Images\\Bubble.png"
#define PATH_IMAGE_2 "Images\\Background_2.png"
#define PATH_IMAGE_3 "Images\\Clip.png"

#define PATH_DLL "DLL\\"

#define PATH_TTF_1 "TTF\\Beatles_Light_Light.ttf"
#define PATH_TTF_2 "TTF\\ClearSans_Medium.ttf"

#define FONT_SIZE 20
#define FONT_COLOR { 255, 255, 255 }



#define WIN_SLASH "\\"

#define VALUE_TYPE float

enum
{
	 White = 0,
	 Black = 1,
	 Yellow = 2,
	 Blue = 3,
	 Red = 4,
	 Green = 5
};

enum
{
	LogWindow = 0,
	MainWindow = 1
};

// ---- STRUCTS ----

struct iXY
{
	int X;
	int Y;
};

struct XY
{
	VALUE_TYPE X;
	VALUE_TYPE Y;
};

struct dXY
{
	double X;
	double Y;
};

struct iXYZ
{
	int X;
	int Y;
	int Z;
};

struct XYZ
{
	VALUE_TYPE X;
	VALUE_TYPE Y;
	VALUE_TYPE Z;
};

struct iLimit
{
	int Min;
	int Max;
};

struct Limit
{
	VALUE_TYPE Min;
	VALUE_TYPE Max;
};

struct ImageSize
{
	int Width;
	int Height;
};

// ---- FUNCTIONS ----

string GetDirectory(string sPath);

string Int2Str(int iValue);


#endif //__3D_GENERAL_H
