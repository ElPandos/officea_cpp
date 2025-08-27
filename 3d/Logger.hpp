#ifndef __3D_LOGGER_HPP
#define __3D_LOGGER_HPP

#include "Window.hpp"

#define SCREEN_LOG_TITLE "3D Logger"

#define SCREEN_LOG_WIDTH 512
#define SCREEN_LOG_HEIGHT 768

#define SCREEN_LOG_POSITION_X 1000
#define SCREEN_LOG_POSITION_Y 40

class CLogger: public CWindow
{

public:
	
	CLogger() 
	{
		InitGlfw();
		m_pWindow = InitWindow(SCREEN_LOG_TITLE, SCREEN_LOG_WIDTH, SCREEN_LOG_HEIGHT, SCREEN_LOG_POSITION_X, SCREEN_LOG_POSITION_Y);
		InitGlew(m_pWindow, SCREEN_LOG_WIDTH, SCREEN_LOG_HEIGHT);
	}

	~CLogger()
	{
	
	}

	GLFWwindow* GetWindow() { return m_pWindow; }

private:

	GLFWwindow* m_pWindow;

};


#endif __3D_LOGGER_HPP