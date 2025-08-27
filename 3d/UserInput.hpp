#ifndef __3D_USERINPUT_H
#define __3D_USERINPUT_H

#include "UserControls.hpp"
#include "UserState.hpp"

class CEventHandler;

class CUserInput
{

public:

	CUserInput(CEventHandler* pHandler)
	{
		m_pHandler = pHandler;

		m_pUserControls	= new CUserControls();
		m_pUserState = new CUserState();

		m_dMouseWheel = 0;

		m_xMouseCurrentPosition.X = 0;
		m_xMouseCurrentPosition.Y = 0;

		m_xMouseMovePosition.X = 0;
		m_xMouseMovePosition.Y = 0;
	}
	
	~CUserInput() 
	{
		delete m_pUserControls;
		delete m_pUserState;
	}

	void KeyPressed();
	void KeyReleased();

	void MouseButtonPressed();
	void MouseButtonReleased();

	void MouseMove(dXY xPos);

	void MouseScrollUp() { m_dMouseWheel++; }
	void MouseScrollDown() { m_dMouseWheel++; }

	CUserControls* GetUserControls() { return m_pUserControls; }
	CUserState* GetUserState() { return m_pUserState; }

private:

	CEventHandler* m_pHandler;

	CUserControls* m_pUserControls;
	CUserState* m_pUserState;

	double m_dMouseWheel;

	dXY m_xMouseCurrentPosition;
	dXY m_xMouseMovePosition;

};

#endif __3D_USERINPUT_H