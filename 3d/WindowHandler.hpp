#ifndef __3D_WINDOWHANDLER_HPP
#define __3D_WINDOWHANDLER_HPP

#include "Graphics.hpp"
#include "Logger.hpp"

class CWindowHandler
{

public:

	CWindowHandler(CTexture* pTexture, CEventHandler* pEventHandler)
	{
		m_pTimer = new CTimer();

		m_pGraphics = new CGraphics(pTexture, pEventHandler);
		//m_pLogger = new CLogger();

		//m_aWindows[0] = m_pGraphics;
		//m_aWindows[1] = m_pLogger;
	}
	
	~CWindowHandler()
	{
		delete m_pGraphics;
		//delete m_pLogger;
	}

	CGraphics* GetGraphics() { return m_pGraphics; }
	CLogger* GetLogger() { return m_pLogger; }

	map<int, CWindow*> GetWindows() { return m_aWindows; }
	 
private:

	CTimer* m_pTimer;

	CGraphics* m_pGraphics;
	CLogger* m_pLogger;

	map<int, CWindow*> m_aWindows;

};

#endif __3D_WINDOWHANDLER_HPP