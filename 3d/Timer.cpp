#include "Timer.hpp"
#include "glfw3.h"
#include <stdio.h>

void CTimer::Start()
{
	m_bStarted = true;
    m_bPaused = false;
    m_dStartTicks = glfwGetTime();
	m_dLastTicks = m_dStartTicks;
	m_dFps = 0;
}

void CTimer::Stop()
{
    m_bStarted = false;
    m_bPaused = false;
}

void CTimer::Pause()
{
	if (m_bStarted && !m_bPaused)
	{
        m_bPaused = true;
        m_dPausedTicks = glfwGetTime() - m_dStartTicks;
    }
}

void CTimer::Unpause()
{
	if (m_bPaused)
	{
        m_bPaused = false;
        m_dStartTicks = glfwGetTime() - m_dPausedTicks;
        m_dPausedTicks = 0;
    }
}

double CTimer::Restart()
{
	double dElapsedTicks = Ticks();
    Start();

    return dElapsedTicks;
}

double CTimer::Ticks()
{
	if (m_bStarted)
	{
        if (m_bPaused)
            return m_dPausedTicks;
        else
            return glfwGetTime() - m_dStartTicks;
    }

    return 0;
}

void CTimer::CalcFps(int& iCurrentFrame)
{
	if (glfwGetTime() - m_dLastTicks >= 1)
	{
		m_dFps = (double)iCurrentFrame/(glfwGetTime() - m_dLastTicks);
		m_dLastTicks += 1.0;
		iCurrentFrame = 0;

		printf("%f Fps\n", m_dFps);
	}	
}