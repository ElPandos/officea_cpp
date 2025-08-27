/*
#include "Thread.hpp"

int TestThread(void *ptr)
{
	int cnt;

	for(cnt=0;cnt<10;cnt++)
	{
		printf("\nThread counter: %d", cnt);
		SDL_Delay(500);
	}

	return cnt;
}

SDL_Thread* CThread::CreateThread(void* pThreadedFunction, string sThreadName)
{
	//SDL_Thread* pThread = SDL_CreateThread(pThreadedFunction,sThreadName, (void*)NULL);
	
	//return pThread;
	return NULL;
}

void CThread::WaitThread(SDL_Thread* pThread)
{
	int threadReturnValue;
	SDL_WaitThread(pThread, &threadReturnValue);

	printf("\nThread () returned value: %d", threadReturnValue,pThread);
}

void CThread::LockThread(void* pThreadedFunction)
{
	SDL_LockMutex(m_pMut);
	cout << "Thread: " << (int)pThreadedFunction << endl; 
	SDL_UnlockMutex(m_pMut);
}

*/