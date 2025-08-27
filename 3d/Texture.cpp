#include "texture.hpp"

GLuint CTexture::LoadBMP(const char* pImagePath)
{
	printf("Reading image %s\n", pImagePath);

	// Data read from the header of the BMP file
	unsigned char aHeader[54];
	unsigned int iDataPos;
	unsigned int iImageSize;
	unsigned int iWidth, iHeight;

	// Actual RGB data
	unsigned char* pData;

	// Open the file
	FILE* pFile = fopen(pImagePath,"rb");
	if (!pFile)
	{
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", pImagePath);
		getchar();

		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(aHeader, 1, 54, pFile) != 54)
	{ 
		printf("Not a correct BMP file\n");
		return 0;
	}

	// A BMP files always begins with "BM"
	if (aHeader[0] != 'B' || aHeader[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Make sure this is a 24bpp file
	if (*(int*)&( aHeader[0x1E]) != 0)
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	if (*(int*)&(aHeader[0x1C]) != 24)
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read the information about the image
	iDataPos = *(int*)&(aHeader[0x0A]);
	iImageSize = *(int*)&(aHeader[0x22]);
	iWidth = *(int*)&(aHeader[0x12]);
	iHeight = *(int*)&(aHeader[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (iImageSize == 0)
		iImageSize = iWidth*iHeight*3; // 3 : one byte for each Red, Green and Blue component

	if (iDataPos == 0)
		iDataPos = 54; // The BMP header is done that way

	// Create a buffer
	pData = new unsigned char [iImageSize];

	// Read the actual data from the file into the buffer
	fread(pData,1,iImageSize,pFile);

	// Everything is in memory now, the file wan be closed
	fclose (pFile);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pData);

	// OpenGL has now copied the data. Free our own version
	delete [] pData;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
// or do it yourself (just like loadBMP_custom and loadDDS)
//GLuint loadTGA_glfw(const char * imagepath){
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Read the file, call glTexImage2D with the right parameters
//	glfwLoadTexture2D(imagepath, 0);
//
//	// Nice trilinear filtering.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Return the ID of the texture we just created
//	return textureID;
//}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint CTexture::LoadDDS(const char* pImagePath)
{
	unsigned char aHeader[124];

	FILE* pFile; 

	/* try to open the file */ 
	pFile = fopen(pImagePath, "rb"); 
	if (pFile == NULL)
	{
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", pImagePath);
		getchar(); 

		return 0;
	}

	/* verify the type of file */ 
	char aFilecode[4]; 
	fread(aFilecode, 1, 4, pFile); 
	if (strncmp(aFilecode, "DDS ", 4) != 0)
	{ 
		fclose(pFile); 

		return 0; 
	}

	/* get the surface desc */ 
	fread(&aHeader, 124, 1, pFile); 

	unsigned int iHeight = *(unsigned int*)&(aHeader[8]);
	unsigned int iWidth	= *(unsigned int*)&(aHeader[12]);
	unsigned int iLinearSize = *(unsigned int*)&(aHeader[16]);
	unsigned int iMipMapCount = *(unsigned int*)&(aHeader[24]);
	unsigned int iFourCC = *(unsigned int*)&(aHeader[80]);

	unsigned char* pBuffer;
	unsigned int iBufferSize;

	/* how big is it going to be including all mipmaps? */ 
	iBufferSize = iMipMapCount > 1 ? iLinearSize * 2 : iLinearSize; 
	pBuffer = (unsigned char*) malloc(iBufferSize * sizeof(unsigned char)); 
	fread(pBuffer, 1, iBufferSize, pFile); 

	/* close the file pointer */ 
	fclose(pFile);

	unsigned int iComponents = (iFourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int iFormat;
	
	switch(iFourCC) 
	{ 
	case FOURCC_DXT1: 
		iFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		iFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		iFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(pBuffer); 
		return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	

	unsigned int iBlockSize = (iFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int iOffset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < iMipMapCount && (iWidth || iHeight); ++level) 
	{ 
		unsigned int iSize = ((iWidth+3)/4)*((iHeight+3)/4)*iBlockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, iFormat, iWidth, iHeight, 0, iSize, pBuffer + iOffset); 

		iOffset += iSize; 
		iWidth  /= 2; 
		iHeight /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(iWidth < 1)
			iWidth = 1;
		
		if(iHeight < 1)
			iHeight = 1;
	} 

	free(pBuffer); 

	return textureID;
}

GLuint CTexture::LoadShaders(const char* pVertexFilePath, const char* pFragmentFilePath)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string sVertexShaderCode;
	ifstream VertexShaderStream(pVertexFilePath, ios::in);

	if (VertexShaderStream.is_open())
	{
		string Line = "";

		while (getline(VertexShaderStream, Line))
			sVertexShaderCode += "\n" + Line;

		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", pVertexFilePath);
		getchar();

		return 0;
	}

	// Read the Fragment Shader code from the file
	string sFragmentShaderCode;
	ifstream FragmentShaderStream(pFragmentFilePath, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			sFragmentShaderCode += "\n" + Line;

		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int iInfoLogLength;

	// Compile Vertex Shader --------------------------------------------------
	printf("Compiling shader : %s\n", pFragmentFilePath);

	char const* pVertexSourcePointer = sVertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &pVertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &iInfoLogLength);
	if (iInfoLogLength > 0)
	{
		vector<char> VertexShaderErrorMessage(iInfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, iInfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader --------------------------------------------------
	printf("Compiling shader : %s\n", pFragmentFilePath);

	char const* pFragmentSourcePointer = sFragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &pFragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &iInfoLogLength);
	if (iInfoLogLength > 0)
	{
		vector<char> FragmentShaderErrorMessage(iInfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, iInfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &iInfoLogLength);
	if (iInfoLogLength > 0)
	{
		vector<char> ProgramErrorMessage(iInfoLogLength+1);
		glGetProgramInfoLog(ProgramID, iInfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

bool CTexture::LoadObject(const char* pPath, vector<vec3>& qaVertices, vector<vec2>& qaUvs, vector<vec3>& qaNormals)
{
	printf("Loading OBJ file %s...\n", pPath);

	vector<unsigned int> aVertexIndices, aUvIndices, aNormalIndices;
	vector<vec3> aVerticesTmp; 
	vector<vec2> aUvsTmp;
	vector<vec3> aNormalsTmp;

	FILE* pFile = fopen(pPath, "r");

	if (pFile == NULL)
	{
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();

		return false;
	}

	while (1)
	{
		char aLineHeader[128];
		// read the first word of the line
		int res = fscanf(pFile, "%s", aLineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(aLineHeader, "v") == 0)
		{
			vec3 vertex;
			fscanf(pFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			aVerticesTmp.push_back(vertex);
		}
		else if (strcmp(aLineHeader, "vt") == 0)
		{
			vec2 uv;
			fscanf(pFile, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			aUvsTmp.push_back(uv);
		}
		else if (strcmp(aLineHeader, "vn") == 0)
		{
			vec3 normal;
			fscanf(pFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			aNormalsTmp.push_back(normal);
		}
		else if (strcmp(aLineHeader, "f") == 0)
		{
			string sVertex1, sVertex2, sVertex3;
			unsigned int iVertexIndex[3], iUvIndex[3], iNormalIndex[3];
			
			int iMatches = fscanf(pFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &iVertexIndex[0], &iUvIndex[0], &iNormalIndex[0], &iVertexIndex[1], &iUvIndex[1], &iNormalIndex[1], &iVertexIndex[2], &iUvIndex[2], &iNormalIndex[2] );
			if (iMatches != 9)
			{
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}

			aVertexIndices.push_back(iVertexIndex[0]);
			aVertexIndices.push_back(iVertexIndex[1]);
			aVertexIndices.push_back(iVertexIndex[2]);
			aUvIndices    .push_back(iUvIndex[0]);
			aUvIndices    .push_back(iUvIndex[1]);
			aUvIndices    .push_back(iUvIndex[2]);
			aNormalIndices.push_back(iNormalIndex[0]);
			aNormalIndices.push_back(iNormalIndex[1]);
			aNormalIndices.push_back(iNormalIndex[2]);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char aStupidBuffer[1000];
			fgets(aStupidBuffer, 1000, pFile);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i=0; i<aVertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int iVertexIndex = aVertexIndices[i];
		unsigned int iUvIndex = aUvIndices[i];
		unsigned int iNormalIndex = aNormalIndices[i];

		// Get the attributes thanks to the index
		vec3 vertex = aVerticesTmp[ iVertexIndex-1 ];
		vec2 uv = aUvsTmp[ iUvIndex-1 ];
		vec3 normal = aNormalsTmp[ iNormalIndex-1 ];

		// Put the attributes in buffers
		qaVertices.push_back(vertex);
		qaUvs.push_back(uv);
		qaNormals.push_back(normal);
	}

	return true;
}


