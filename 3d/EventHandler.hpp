#ifndef __3D_EVENTHANDLER_HPP
#define __3D_EVENTHANDLER_HPP

#include "UserInput.hpp"

class CEventHandler
{

public:

	CEventHandler()
	{
		m_pUserInput = new CUserInput(this);
	}
	
	~CEventHandler()
	{
		delete m_pUserInput;
	}

	CUserInput* GetUserInput() { return m_pUserInput; }
	 
private:

	CUserInput* m_pUserInput;

};

#endif __3D_EVENTHANDLER_HPP