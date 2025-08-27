#include "EventHandler.hpp"
#include "UserInput.hpp"

/*
void CEventHandler::Loop()
{
	
	SDL_Event xEvent; 

	bool bQuit = false;

	while (!bQuit)
	{
		while (SDL_PollEvent(&xEvent))
		{
			int iVal = 0;

			if (xEvent.type == SDL_QUIT) // Close program window
				bQuit = true;

			if (xEvent.type == SDL_KEYDOWN)
			{
				m_pUserInput->GetKeyboard()->Key(xEvent, &m_CurrentPosition);
			}
			
			if (xEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				m_pUserInput->GetMouse()->Button(xEvent, &m_CurrentPosition);
			}
		}
	}
}

	*/