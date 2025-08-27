#ifndef __3D_APP_HPP
#define __3D_APP_HPP

#include "Texture.hpp"
#include "EventHandler.hpp"
#include "WindowHandler.hpp"

class CApp
{

public:

    CApp(string sPath)
	{
		m_sExecPath = sPath;
		
		m_pTexture = new CTexture();

		m_pEventHandler = new CEventHandler();
		m_pWindowHandler = new CWindowHandler(m_pTexture, m_pEventHandler);
	
		MainLoop();
	}

	~CApp()
	{
		delete m_pEventHandler;
		delete m_pTexture;
		delete m_pWindowHandler;
	}

	void MainLoop();

	CTexture* GetTextures() { return m_pTexture; }

	CWindowHandler* GetWindowHandler() { return m_pWindowHandler; }
	CEventHandler* GetEventHandler() { return m_pEventHandler; }

	string GetExecPath() { return m_sExecPath; }

private:

	string m_sExecPath;

	CTexture* m_pTexture;

	CEventHandler* m_pEventHandler;
	CWindowHandler* m_pWindowHandler;

};

#endif __3D_APP_HPP