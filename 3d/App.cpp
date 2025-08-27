#include "App.hpp"

void CApp::MainLoop()
{
	while (glfwGetKey(m_pWindowHandler->GetGraphics()->GetWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(m_pWindowHandler->GetGraphics()->GetWindow()) == 0) // Check if the ESC key was pressed or the window was closed
	{
		//for(map<int, CWindow*>::iterator itWindow = m_pWindowHandler->GetWindows().begin(); itWindow != m_pWindowHandler->GetWindows().end(); itWindow++)
		//{
		m_pWindowHandler->GetGraphics()->Draw(m_pWindowHandler->GetGraphics()->GetWindow(), m_pEventHandler);	
		//}
	}

	m_pWindowHandler->GetGraphics()->CleanUp();
}