#ifndef __3D_TEXTURE_HPP
#define __3D_TEXTURE_HPP

#include "General.hpp"

class CTexture
{

public:

	CTexture() {}
	~CTexture() {}

	GLuint LoadShaders(const char* pVertexFilePath, const char* pFragmentFilePath);

	// Load a .BMP file using our custom loader
	GLuint LoadBMP(const char* pImagePath);

	//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
	//// or do it yourself (just like loadBMP_custom and loadDDS)
	//// Load a .TGA file using GLFW's own loader
	//GLuint loadTGA_glfw(const char * imagepath);

	// Load a .DDS file using GLFW's own loader
	GLuint LoadDDS(const char* pImagePath);

	bool LoadObject(const char* pPath, vector<vec3>& qaVertices, vector<vec2>& qaUvs, vector<vec3>& qaNormals);

private:


};


#endif  __3D_TEXTURE_HPP