#ifndef __3D_GRAPHICS_HPP
#define __3D_GRAPHICS_HPP

#include "Window.hpp"

#define SCREEN_MAIN_TITLE "3D Window"

#define SCREEN_MAIN_WIDTH 1024
#define SCREEN_MAIN_HEIGHT 768

#define SCREEN_MAIN_POSITION_X 100
#define SCREEN_MAIN_POSITION_Y 40

class CGraphics: public CWindow
{

public:

	CGraphics(CTexture* pTexture, CEventHandler* pEventHandler)
	{
		m_pEventHandler = pEventHandler;
		m_pTexture = pTexture;

		m_iFrame = 0;

		InitGlfw();
		m_pWindow = InitWindow(SCREEN_MAIN_TITLE, SCREEN_MAIN_WIDTH, SCREEN_MAIN_HEIGHT, SCREEN_MAIN_POSITION_X, SCREEN_MAIN_POSITION_Y);
		InitGlew(m_pWindow, SCREEN_MAIN_WIDTH, SCREEN_MAIN_HEIGHT);
		InitGl();

		Load(m_pTexture);

	}

	~CGraphics()
	{

	}

	void IncreaseFrame() { m_iFrame++; }
	
	GLFWwindow* GetWindow() { return m_pWindow; }

private:

	CEventHandler* m_pEventHandler;
	CTexture* m_pTexture;

	GLFWwindow* m_pWindow;

	int m_iFrame;

};


#endif __3D_GRAPHICS_HPP