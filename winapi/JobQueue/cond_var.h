#pragma once

class Mutex
{
public:
	Mutex() : m_mutex(CreateMutex(nullptr, FALSE, nullptr)) {}
	~Mutex() { CloseHandle(m_mutex); }

	void lock() { WaitForSingleObject(m_mutex, INFINITE); }
	void unlock() { ReleaseMutex(m_mutex); }

private:
	HANDLE m_mutex;
};

class WaitCondition
{
	enum { EventOne, EventAll, EventsCount };
public:
	WaitCondition() :
		m_waiters(0)
	{
		m_hEvents[EventOne] = CreateEvent(nullptr, FALSE, TRUE, nullptr);
		m_hEvents[EventAll] = CreateEvent(nullptr, TRUE, TRUE, nullptr);
		//InitializeCriticalSection(&m_wakeLock);
	}
	~WaitCondition()
	{
		for(auto h : m_hEvents)
			CloseHandle(h);
	}

	bool wait(Mutex& mutex, DWORD ms = INFINITE)
	{
		++m_waiters;
		mutex.unlock();

		bool timeout = (WaitForMultipleObjects(ARRAYSIZE(m_hEvents), m_hEvents, FALSE, ms) == WAIT_TIMEOUT);

		mutex.lock();
		if(!--m_waiters)
		{
			//EnterCriticalSection(&m_wakeLock);
			for(auto h : m_hEvents)
				ResetEvent(h);
			//LeaveCriticalSection(&m_wakeLock);
		}

		return !timeout;
	}
	void wakeOne()
	{
		//EnterCriticalSection(&m_wakeLock);
		SetEvent(m_hEvents[EventOne]);
		//LeaveCriticalSection(&m_wakeLock);
	}
	void wakeAll()
	{
		//EnterCriticalSection(&m_wakeLock);
		SetEvent(m_hEvents[EventAll]);
		//LeaveCriticalSection(&m_wakeLock);
	}

private:
	HANDLE m_hEvents[EventsCount];

	//CRITICAL_SECTION m_wakeLock;
	unsigned m_waiters;
};