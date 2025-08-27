#include "Window.hpp"
#include "Texture.hpp"
#include "EventHandler.hpp"


void CWindow::InitGlew(GLFWwindow* pWindow, int iWidth, int iHeight)
{
	// Initialize GLEW
	if (glewInit() != GLEW_OK)
		fprintf(stderr, "Failed to initialize GLEW\n");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(pWindow, iWidth/2, iHeight/2);
}

bool CWindow::InitGlfw()
{
	bool bResult = glfwInit();

	// Initialise GLFW
	if (!bResult)
		fprintf(stderr, "Failed to initialize GLFW\n");
	
	return bResult;
}

GLFWwindow* CWindow::InitWindow(string sName, int iWidth, int iHeight, int iPosX, int iPosY)
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	GLFWwindow* pWindow = glfwCreateWindow(iWidth, iHeight, sName.c_str(), NULL, NULL);
	if (pWindow == NULL)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}

	glfwMakeContextCurrent(pWindow);	
	glfwSetWindowPos(pWindow, iPosX, iPosY);

	return pWindow;
}

void CWindow::InitGl()
{
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

}

void CWindow::Load(CTexture* pTexture)
{
	// Create and compile our GLSL program from the shaders
	m_xDrawData.xProgramID = pTexture->LoadShaders("StandardShading.vertexshader","StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_xDrawData.xMatrixID = glGetUniformLocation(m_xDrawData.xProgramID, "MVP");
	m_xDrawData.xViewMatrixID = glGetUniformLocation(m_xDrawData.xProgramID, "V");
	m_xDrawData.xModelMatrixID = glGetUniformLocation(m_xDrawData.xProgramID, "M");

	// Get a handle for our buffers
	m_xDrawData.xVertexPosition_modelspaceID = glGetAttribLocation(m_xDrawData.xProgramID, "vertexPosition_modelspace");
	m_xDrawData.xVertexUVID = glGetAttribLocation(m_xDrawData.xProgramID, "vertexUV");
	m_xDrawData.xVertexNormal_modelspaceID = glGetAttribLocation(m_xDrawData.xProgramID, "vertexNormal_modelspace");

	// Load the texture
	m_xDrawData.xTexture = pTexture->LoadDDS("uvmap.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	m_xDrawData.xTextureID  = glGetUniformLocation(m_xDrawData.xProgramID, "myTextureSampler");

	// Read our .obj file
	bool bResult = pTexture->LoadObject("suzanne.obj", m_xDrawData.aVertices, m_xDrawData.aUvs, m_xDrawData.aNormals);

	indexVBO(m_xDrawData.aVertices, m_xDrawData.aUvs, m_xDrawData.aNormals, m_xDrawData.aIndices, m_xDrawData.aIndexed_vertices, m_xDrawData.aIndexed_uvs, m_xDrawData.aIndexed_normals);

	// Load it into a VBO
	glGenBuffers(1, &m_xDrawData.xVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_xDrawData.aIndexed_vertices.size() * sizeof(vec3), &m_xDrawData.aIndexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_xDrawData.xUvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xUvbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_xDrawData.aIndexed_uvs.size() * sizeof(vec2), &m_xDrawData.aIndexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_xDrawData.xNormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xNormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_xDrawData.aIndexed_normals.size() * sizeof(vec3), &m_xDrawData.aIndexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &m_xDrawData.xElementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_xDrawData.xElementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_xDrawData.aIndices.size() * sizeof(unsigned short), &m_xDrawData.aIndices[0] , GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(m_xDrawData.xProgramID);
	m_xDrawData.xLightID = glGetUniformLocation(m_xDrawData.xProgramID, "LightPosition_worldspace");
}

/*
void CWindow::OpenWindow(GLFWwindow* pWindow, string sName, int iWidth, int iHeight, int iPosX, int iPosY);
{
	InitWindow(pWindow, sName, iWidth, iHeight, iPosX, iPosY);

	InitGlew(pWindow, iWidth, iHeight);

	InitGl();

	Load();

	m_iFrame = 0;
	m_pTimer->Start();
}

*/

void CWindow::CleanUp()
{
	// Cleanup VBO and shader
	glDeleteBuffers(1, &m_xDrawData.xVertexbuffer);
	glDeleteBuffers(1, &m_xDrawData.xUvbuffer);
	glDeleteBuffers(1, &m_xDrawData.xNormalbuffer);
	glDeleteBuffers(1, &m_xDrawData.xElementbuffer);
	glDeleteProgram(m_xDrawData.xProgramID);
	glDeleteTextures(1, &m_xDrawData.xTexture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void CWindow::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CWindow::Draw(GLFWwindow* pWindow, CEventHandler* pEventHandler)
{
	// Speed calculations
	//m_iFrame++;
	//m_pTimer->CalcFps(m_iFrame);

	// Clear the screen
	Clear();

	// Compute the MVP matrix from keyboard and mouse input
	pEventHandler->GetUserInput()->GetUserControls()->ComputeMatricesFromInputs(pWindow);
	mat4 xProjectionMatrix = pEventHandler->GetUserInput()->GetUserControls()->GetProjectionMatrix();
	mat4 xViewMatrix = pEventHandler->GetUserInput()->GetUserControls()->GetViewMatrix();
		
	////// Start of the rendering of the first object //////
		
	// Use our shader
	glUseProgram(m_xDrawData.xProgramID);
	
	vec3 xLightPos = vec3(4,4,4);
	glUniform3f(m_xDrawData.xLightID, xLightPos.x, xLightPos.y, xLightPos.z);
	glUniformMatrix4fv(m_xDrawData.xViewMatrixID, 1, GL_FALSE, &xViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "xProgramID"
		
	mat4 xModelMatrix1 = mat4(1.0);
	mat4 xMVP1 = xProjectionMatrix * xViewMatrix * xModelMatrix1;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(m_xDrawData.xMatrixID, 1, GL_FALSE, &xMVP1[0][0]);
	glUniformMatrix4fv(m_xDrawData.xModelMatrixID, 1, GL_FALSE, &xModelMatrix1[0][0]);
		
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_xDrawData.xTexture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_xDrawData.xTextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(m_xDrawData.xVertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xVertexbuffer);
	glVertexAttribPointer(
		m_xDrawData.xVertexPosition_modelspaceID, // The attribute we want to configure
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(m_xDrawData.xVertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xUvbuffer);
	glVertexAttribPointer(
		m_xDrawData.xVertexUVID,                       // The attribute we want to configure
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(m_xDrawData.xVertexNormal_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xNormalbuffer);
	glVertexAttribPointer(
		m_xDrawData.xVertexNormal_modelspaceID,        // The attribute we want to configure
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_xDrawData.xElementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_xDrawData.aIndices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
		
	////// End of rendering of the first object //////
	////// Start of the rendering of the second object //////

	// In our very specific case, the 2 objects use the same shader.
	// So it's useless to re-bind the "xProgramID" shader, since it's already the current one.
	//glUseProgram(xProgramID);
		
	// Similarly : don't re-set the light position and camera matrix in xProgramID,
	// it's still valid !
	// *** You would have to do it if you used another shader ! ***
	//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "xProgramID"
				
	// Again : this is already done, but this only works because we use the same shader.
	//// Bind our texture in Texture Unit 0
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	//// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(TextureID, 0);
				
	// BUT the Model matrix is different (and the MVP too)
	mat4 xModelMatrix2 = mat4(1.0);
	xModelMatrix2 = translate(xModelMatrix2, vec3(2.0f, 0.0f, 0.0f));
	mat4 xMVP2 = xProjectionMatrix * xViewMatrix * xModelMatrix2;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(m_xDrawData.xMatrixID, 1, GL_FALSE, &xMVP2[0][0]);
	glUniformMatrix4fv(m_xDrawData.xModelMatrixID, 1, GL_FALSE, &xModelMatrix2[0][0]);
		
	// The rest is exactly the same as the first object
		
	// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(xVertexPosition_modelspaceID); // Already enabled
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xVertexbuffer);
	glVertexAttribPointer(m_xDrawData.xVertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	//glEnableVertexAttribArray(vertexUVID); // Already enabled
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xUvbuffer);
	glVertexAttribPointer(m_xDrawData.xVertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : normals
	//glEnableVertexAttribArray(xVertexNormal_modelspaceID); // Already enabled
	glBindBuffer(GL_ARRAY_BUFFER, m_xDrawData.xNormalbuffer);
	glVertexAttribPointer(m_xDrawData.xVertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_xDrawData.xElementbuffer);

	// Draw the triangles !
	glDrawElements(GL_TRIANGLES, m_xDrawData.aIndices.size(), GL_UNSIGNED_SHORT, (void*)0);
		
	////// End of rendering of the second object //////

	glDisableVertexAttribArray(m_xDrawData.xVertexPosition_modelspaceID);
	glDisableVertexAttribArray(m_xDrawData.xVertexUVID);
	glDisableVertexAttribArray(m_xDrawData.xVertexNormal_modelspaceID);

	// Swap buffers
	Update(pWindow);
}

void CWindow::Update(GLFWwindow* pWindow)
{
	glfwSwapBuffers(pWindow);
	glfwPollEvents();
}

/*
void CWindow::DrawNew(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip)
{
    //Convert pivot pos from relative to object's center to screen space
    xPivot += dstRect.w / 2;
    yPivot += dstRect.h / 2;
    //SDL expects an SDL_Point as the pivot location
    SDL_Point pivot = { xPivot, yPivot };
    //Draw the texture
    SDL_RenderCopyEx(m_pRenderer, tex, clip, &dstRect, angle, &pivot, flip);
}

void CWindow::Draw(SDL_Texture* pTexure, int iX, int iY, SDL_Rect* pClip)
{
    SDL_Rect xPos;
    xPos.x = iX;
    xPos.y = iY;
	
    if (pClip != NULL)
	{
        xPos.w = pClip->w;
		xPos.h = pClip->h;
    }
    else 
		SDL_QueryTexture(pTexure, NULL, NULL, &xPos.w, &xPos.h);

	SDL_RenderCopy(m_pRenderer, pTexure, pClip, &xPos); // Draw the texture
}

SDL_Texture* CWindow::LoadImage(string sPath)
{
	SDL_Texture* pTex = NULL;
	pTex = IMG_LoadTexture(m_pRenderer, sPath.c_str());
	
	if (pTex == NULL)
		throw runtime_error("Failed to load image: " + sPath + "->" + IMG_GetError());

	return pTex;
}

SDL_Texture* CWindow::LoadText(string sMessage, string sFontFilePath, SDL_Color xColor, int iFontSize)
{
	TTF_Font* pFont = nullptr;
    pFont = TTF_OpenFont(sFontFilePath.c_str(), iFontSize);
    if (pFont == nullptr)
        throw runtime_error("Failed to load font: " + sFontFilePath + TTF_GetError());
	
    SDL_Surface* pSurf = TTF_RenderText_Blended(pFont, sMessage.c_str(), xColor);
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pSurf);

    SDL_FreeSurface(pSurf);
    TTF_CloseFont(pFont);
 
    return pTexture;
}

void CWindow::Clear()
{
    SDL_RenderClear(m_pRenderer);
}

void CWindow::Present()
{
    SDL_RenderPresent(m_pRenderer);
}

SDL_Rect CWindow::Box()
{
    SDL_GetWindowSize(m_pWindow, &m_WindowBox.w, &m_WindowBox.h);
    return m_WindowBox;
}

vector<SDL_Rect> CWindow::GatherClips(int iCutWidth, int iCutHeight)
{
    vector<SDL_Rect> aClips;
	SDL_Rect xClip;

    int iColumn = 0;
 
    for (int i = 0; i < 4; ++i)
	{
		if (i != 0 && i % 2 == 0)
            ++iColumn;

        xClip.x = iColumn * iCutWidth;
        xClip.y = i % 2 * iCutHeight;
        xClip.w = iCutWidth;
        xClip.h = iCutHeight;

		aClips.push_back(xClip);
    }

	return aClips;
}

void CWindow::AutoLoad()
{
	SDL_Texture* pImage = NULL;
	SDL_Texture* pBackground = NULL;
	SDL_Texture* pText = NULL;
	SDL_Texture* pClip = NULL;
	SDL_Texture* pClock = NULL;

	pImage = LoadImage(m_sExecPath + string(PATH_IMAGE_1));
	pBackground = LoadImage(m_sExecPath + string(PATH_IMAGE_2));
	pClip = LoadImage(m_sExecPath + string(PATH_IMAGE_3));

	m_aClipGrid = GatherClips(CLIP_WIDTH, CLIP_HEIGHT);

	m_aAutoObjects.push_back(pImage);
	m_aAutoObjects.push_back(pBackground);
	m_aAutoObjects.push_back(pClip);
}
*/
	
unsigned int iText2DTextureID;              // Texture containing the font
unsigned int iText2DVertexBufferID;         // Buffer containing the vertices
unsigned int iText2DUVBufferID;             // UVs
unsigned int iText2DShaderID;               // Program used to disaply the text
unsigned int iVertexPosition_screenspaceID; // Location of the program's "vertexPosition_screenspace" attribute
unsigned int iVertexUVID;                   // Location of the program's "vertexUV" attribute
unsigned int iText2DUniformID;              // Location of the program's texture attribute

/*
void CWindow::InitText2D(const char * cTexturePath)
{
	// Initialize texture
	iText2DTextureID = m_pTexture->LoadDDS(cTexturePath);

	// Initialize VBO
	glGenBuffers(1, &iText2DVertexBufferID);
	glGenBuffers(1, &iText2DUVBufferID);

	// Initialize Shader
	iText2DShaderID = m_pTexture->LoadShaders( "TextVertexShader.vertexshader", "TextVertexShader.fragmentshader" );

	// Get a handle for our buffers
	iVertexPosition_screenspaceID = glGetAttribLocation(iText2DShaderID, "vertexPosition_screenspace");
	iVertexUVID = glGetAttribLocation(iText2DShaderID, "vertexUV");

	// Initialize uniforms' IDs
	iText2DUniformID = glGetUniformLocation(iText2DShaderID, "myTextureSampler");
}
*/

void CWindow::PrintText2D(const char* pText, int x, int y, int size)
{
	unsigned int iLength = strlen(pText);

	// Fill buffers
	vector<vec2> aVertices;
	vector<vec2> aUVs;

	for (unsigned int i=0 ; i<iLength ; i++)
	{
		vec2 xVertex_up_left    = vec2( x+i*size     , y+size );
		vec2 xVertex_up_right   = vec2( x+i*size+size, y+size );
		vec2 xVertex_down_right = vec2( x+i*size+size, y      );
		vec2 xVertex_down_left  = vec2( x+i*size     , y      );

		aVertices.push_back(xVertex_up_left);
		aVertices.push_back(xVertex_down_left);
		aVertices.push_back(xVertex_up_right);

		aVertices.push_back(xVertex_down_right);
		aVertices.push_back(xVertex_up_right);
		aVertices.push_back(xVertex_down_left);

		char character = pText[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		vec2 uv_up_left    = vec2( uv_x           , uv_y );
		vec2 uv_up_right   = vec2( uv_x+1.0f/16.0f, uv_y );
		vec2 uv_down_right = vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		vec2 uv_down_left  = vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		aUVs.push_back(uv_up_left   );
		aUVs.push_back(uv_down_left );
		aUVs.push_back(uv_up_right  );

		aUVs.push_back(uv_down_right);
		aUVs.push_back(uv_up_right);
		aUVs.push_back(uv_down_left);
	}

	glBindBuffer(GL_ARRAY_BUFFER, iText2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, aVertices.size() * sizeof(vec2), &aVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, iText2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, aUVs.size() * sizeof(vec2), &aUVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(iText2DShaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iText2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(iText2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(iVertexPosition_screenspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, iText2DVertexBufferID);
	glVertexAttribPointer(iVertexPosition_screenspaceID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(iVertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, iText2DUVBufferID);
	glVertexAttribPointer(iVertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, aVertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(iVertexPosition_screenspaceID);
	glDisableVertexAttribArray(iVertexUVID);

}

void CWindow::CleanupText2D(){

	// Delete buffers
	glDeleteBuffers(1, &iText2DVertexBufferID);
	glDeleteBuffers(1, &iText2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &iText2DTextureID);

	// Delete shader
	glDeleteProgram(iText2DShaderID);
}






