#ifndef __3D_TIMER_H
#define __3D_TIMER_H

class CTimer
{

public:

    CTimer()
	{
		m_dStartTicks = 0;
		m_dPausedTicks = 0;
		m_bStarted = false;
		m_bPaused = false;
	}

	~CTimer() {}

    void Start();
    void Stop();

    void Pause();
    void Unpause();

    double Restart();
    double Ticks();

	bool Started() { return m_bStarted; }
    bool Paused() { return m_bPaused; }

	void CalcFps(int& iCurrentFrame);
	double GetFps() { return m_dFps; }

private:

    double m_dStartTicks;
	double m_dPausedTicks;

	double m_dLastTicks;
	double m_dFps;

    bool m_bStarted;
	bool m_bPaused;

};

#endif __3D_TIMER_H