#include <Windows.h>

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>


std::mutex g_log_mutex;
#define PRINT(m) \
	do { \
		std::lock_guard<std::mutex> locker(g_log_mutex); \
		std::cout << m << std::endl; \
	} while(0)
#define PRINT_THREAD(m) PRINT(GetCurrentThreadId() << ": " << m)

//#define DBG
#ifdef DBG
#define DEBUG_PRINT(m) PRINT(m)
#define DEBUG_PRINT_THREAD(m) PRINT_THREAD(m)
#else
#define DEBUG_PRINT(m) do {} while(0)
#define DEBUG_PRINT_THREAD(m) do {} while(0)
#endif

// ====================================
using task_gcd_t = std::pair<uint64_t, uint64_t>;

template<typename T>
class JobQueue
{
public:
	typedef T type;

private:
	using queue_t = std::queue<T>;

public:
	JobQueue() { InitializeCriticalSection(&m_cs); }

	void push(const T& job)
	{
		EnterCriticalSection(&m_cs);
		m_queue.push(job);
		LeaveCriticalSection(&m_cs);
	}

	bool pop(T& jobs)
	{
		bool res;
		EnterCriticalSection(&m_cs);
		if(m_queue.empty())
			res = false;
		else
		{
			jobs = m_queue.front();
			m_queue.pop();
			res = true;
		}
		LeaveCriticalSection(&m_cs);
		return res;
	}

	size_t size() const
	{
		EnterCriticalSection(&m_cs);
		auto n = m_queue.size();
		LeaveCriticalSection(&m_cs);
		return n;
	}

private:
	queue_t m_queue;
	mutable CRITICAL_SECTION m_cs;
};


template<typename T>
static T gcd(T x, T y)
{
	return y ? gcd(y, x % y) : x;
}


template<typename T = uint64_t>
static T urand()
{
	static auto dummy = (srand(static_cast<unsigned>(time(nullptr))), 0);
	T x = 0;

	constexpr auto BITS_BLOCK = 12;
	for(unsigned i = 0; i < (sizeof(T) * 8) / BITS_BLOCK; ++i)
	{
		x <<= BITS_BLOCK;
		x |= rand() % ((1 << BITS_BLOCK) - 1);
	}

	constexpr auto BITS_REM = sizeof(T) % BITS_BLOCK;
	x <<= BITS_REM;
	x |= rand() % ((1 << BITS_REM) - 1);
	return x;
}

// ====================================
template<typename T>
struct WorkerData
{
	JobQueue<T> jobs;
	volatile unsigned bActive;
	HANDLE eventWork;
	HANDLE eventIdle;
	CRITICAL_SECTION csStateChange;

	explicit WorkerData():
		bActive(1),
		eventWork(CreateEvent(nullptr, TRUE, FALSE, nullptr)),
		eventIdle(CreateEvent(nullptr, FALSE, TRUE, nullptr))
	{
		InitializeCriticalSection(&csStateChange);
	}
	~WorkerData()
	{
		CloseHandle(eventIdle);
		CloseHandle(eventWork);
	}
};

template<typename T>
static DWORD WINAPI Worker(VOID* pParam)
{
	PRINT_THREAD("start");

	WorkerData<T>& data = *static_cast<WorkerData<T>*>(pParam);

	for(decltype(data.jobs)::type job; /*data.bActive*/;)
	{
		// Sleep on empty queue
		if( !data.jobs.pop(job) )
		{
			EnterCriticalSection(&data.csStateChange);
			if(!data.bActive)
			{
				DEBUG_PRINT_THREAD("exit on idle");
				LeaveCriticalSection(&data.csStateChange);
				break;
			}
			DEBUG_PRINT_THREAD("prepare idle");
			SetEvent(data.eventIdle);
			DEBUG_PRINT_THREAD("idle...");
			LeaveCriticalSection(&data.csStateChange);
			WaitForSingleObject(data.eventWork, INFINITE);
			ResetEvent(data.eventWork);
			DEBUG_PRINT_THREAD("resume");
			continue;
		}

		auto x = job.first;
		auto y = job.second;
		auto z = gcd(x, y);
		if(z > 1)
			DEBUG_PRINT_THREAD("gcd(" << x << ", " << y << ") = " << z);
	}
	PRINT_THREAD("exit");

	return 0;
}


int main(int, const char**)
{
	constexpr size_t TaskCount = 0x10000;
	constexpr unsigned ThreadCount = 4;
	constexpr size_t MaxJobs = 4 * ThreadCount;

	// Init all tasks
	std::vector<task_gcd_t> tasks(TaskCount);

	PRINT("Generate " << TaskCount << " tasks");
	for(size_t i = 0, n = tasks.size(); i < n; ++i)
	{
		tasks[i].first  = urand<>();
		tasks[i].second = urand<>();
	}

	// Create threads
	std::vector<HANDLE> hThreads;
	WorkerData<task_gcd_t> workerData;

	PRINT("Create " << ThreadCount << " threads");
	for(auto i = 0; i < ThreadCount; ++i)
	{
		HANDLE h = CreateThread(NULL, 0, Worker<task_gcd_t>, &workerData, 0, NULL);
		if(!h)
			break;
		hThreads.push_back(h);
	}

	// Loop
	PRINT("Go");
	for(auto iTask = 0, percent = 0; hThreads.size() == ThreadCount;)
	{
		DEBUG_PRINT("Update queue");
		// Fill the job queue
		for(auto n = MaxJobs - workerData.jobs.size(); n && (iTask < TaskCount); --n, ++iTask)
		{
			auto pc = ((iTask + 1) * 10) / TaskCount;
			if(pc != percent)
			{
				percent = pc;
				PRINT(pc * 10 << '%');
			}
			workerData.jobs.push( tasks[iTask] );
		}
		if(iTask >= TaskCount)
		{
			EnterCriticalSection(&workerData.csStateChange);
			DEBUG_PRINT("Tasks processed");
			workerData.bActive = 0;
			LeaveCriticalSection(&workerData.csStateChange);
			SetEvent(workerData.eventWork);
			break;
		}

		DEBUG_PRINT("Release workers");
		SetEvent(workerData.eventWork);
		DEBUG_PRINT("Wait for idle...");
		WaitForSingleObject(workerData.eventIdle, INFINITE);
	}

	// Clean up
	if(!hThreads.empty())
	{
		PRINT("Wait for threads...");
		DWORD res = WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);
		DEBUG_PRINT("Threads exited (0x" << std::hex << res << ')');
	}

	PRINT("Done");
	getchar();

	return 0;
}
