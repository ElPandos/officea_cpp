#ifndef __3D_WINDOW_H
#define __3D_WINDOW_H

#include "General.hpp"

class CTexture;
class CEventHandler;

struct DrawData
{
	GLuint xProgramID;

	GLuint xMatrixID;
	GLuint xViewMatrixID;
	GLuint xModelMatrixID;

	GLuint xVertexPosition_modelspaceID;
	GLuint xVertexUVID;
	GLuint xVertexNormal_modelspaceID;

	GLuint xTexture;
	GLuint xTextureID;

	vector<vec3> aVertices, aNormals;
	vector<vec2> aUvs;

	vector<unsigned short> aIndices;
	vector<vec3> aIndexed_vertices, aIndexed_normals;
	vector<vec2> aIndexed_uvs;

	GLuint xVertexbuffer;
	GLuint xUvbuffer;
	GLuint xNormalbuffer;
	
	GLuint xElementbuffer;

	GLuint xLightID;

};

class CWindow
{

public:

	bool InitGlfw();
	void InitGlew(GLFWwindow* pWindow, int iWidth, int iHeight);
	void InitGl();
	GLFWwindow* InitWindow(string sName, int iWidth, int iHeight, int iPosX, int iPosY);

	void InitText2D(const char* cTexturePath);

    //void OpenWindow(GLFWwindow* pWindow, string sName, int iWidth, int iHeight, int iPosX, int iPosY);

	void PrintText2D(const char * text, int x, int y, int size);
	void CleanupText2D();

	void Clear();
	void Update(GLFWwindow* pWindow);
	void Load(CTexture* pTexture);
	void Draw(GLFWwindow* pWindow, CEventHandler* pEventHandler);
	void CleanUp();

	virtual GLFWwindow* GetWindow()=0;
	

	//void DrawNew(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip);
    //void Draw(SDL_Texture* pTexure, int iX, int iY, SDL_Rect* pClip  = NULL);

    //SDL_Texture* LoadImage(string sPath);
    //SDL_Texture* LoadText(string sMessage, string sFontFilePath, SDL_Color xColor, int iFontSize);
   
    
    //SDL_Rect Box();
	
	//vector<SDL_Rect> GatherClips(int iCutWidth, int iCutHeight);

	//void AutoLoad();

	//vector<SDL_Texture*> GetTexture() { return m_aAutoObjects; }
	//vector<SDL_Rect> GetClips() { return m_aClipGrid; }

private:

	map<string, CWindow*> m_pDraw;

	DrawData m_xDrawData;

    //CEventHandler* m_pEventHandler;
	//CTexture* m_pTexture;	

};

#endif __3D_WINDOW_H