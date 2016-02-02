#define NOMINMAX
#include <Windows.h>

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>


std::mutex g_log_mutex;
#define LOG(m) \
do { \
	std::lock_guard<std::mutex> guard(g_log_mutex); \
	std::cout << m << std::endl; \
} while(0)
#define LOG_FN(m) LOG(__FUNCTION__ << ": " << m)

//#define CALL_ONCE(expr) do { static volatile int dummy = (expr, 0); } while(0)


class exc_thread_create : public std::exception
{
public:
	exc_thread_create() throw() : std::exception("failed to create a thread") {}
};

// ====================================
template<typename T>
class RingBuffer
{
public:
	RingBuffer(size_t f_size = 0):
		m_buffer(f_size),
		m_index(0), m_size(0)
	{}

	size_t size() const { return m_size; }
	bool empty() const { return !m_size; }
	bool full() const { return (m_size == m_buffer.size()); }

	void push(const T& x)
	{
		m_buffer[(m_index + m_size) % m_buffer.size()] = x;
		++m_size;
	}
	T pop()
	{
		T obj = m_buffer[m_index];
		m_index = (m_index + 1) % m_buffer.size();
		--m_size;
		return obj;
	}

	void reserve(size_t f_size)
	{
		if(f_size > m_size)
			resize(f_size);
	}
	void resize(size_t f_size)
	{
		if(!m_size)
		{
			m_buffer.resize(f_size);
			return;
		}

		std::vector<T> temp(m_buffer);
		size_t new_size = std::min(m_size, f_size);
		m_buffer.resize(f_size);

		size_t j = 0, n = new_size;
		for(size_t i = m_index; n && i < temp.size(); ++i, ++j, --n)
			m_buffer[j] = temp[i];
		for(size_t i = 0; n; ++i, ++j, --n)
			m_buffer[j] = temp[i];

		m_index = 0;
		m_size = new_size;
	}

	void sort()
	{
		size_t n = size();
		if(!n)
			return;
		if(m_index != 0)
			resize(n);
		std::sort(m_buffer.begin(), m_buffer.begin() + n, std::greater<unsigned>());
	}

private:
	std::vector<T> m_buffer;
	size_t m_index;
	size_t m_size;
};


class Thread
{
public:
	Thread()
	{
		m_handle = CreateThread(NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, NULL);
		if(!m_handle)
			throw exc_thread_create();
	}
	virtual ~Thread()
	{
		if(m_handle)
			TerminateThread(m_handle, 1);
	}

	HANDLE handle() const { return m_handle; }

protected:
	void run() { ResumeThread(m_handle); }

private:
	static DWORD WINAPI ThreadFunc(VOID* pParam)
	{
		Thread* p = static_cast<Thread*>(pParam);
		p->ThreadFunc();
		p->m_handle = NULL;
		return 0;
	}

	virtual void ThreadFunc() = 0;

protected:
	HANDLE m_handle;
};

// ====================================
RingBuffer<unsigned> tasks;
static CRITICAL_SECTION g_tasks_lock;


class ThreadWriter : public Thread
{
public:
	ThreadWriter() { run(); }

private:
	void ThreadFunc() override
	{
		LOG_FN("started");
		srand( static_cast<unsigned>(time(NULL)) );

		LOG_FN("gen tasks");
		tasks.reserve(40);
		for(unsigned i = 0; i < 40; ++i)
			tasks.push( gen_task() );
		tasks.sort();

		LOG_FN("loop");
		while(1)
		{
			Sleep(100);
			EnterCriticalSection(&g_tasks_lock);
			if(tasks.size() < 8)
			{
				auto t = gen_task();
				LOG_FN(tasks.size() << " tasks,  new = " << t);
				tasks.push(t);
			}
			LeaveCriticalSection(&g_tasks_lock);
		}

		LOG_FN("done");
	}

	unsigned gen_task() const { return (rand() % 9 + 1); }
};


class ThreadReader : public Thread
{
public:
	ThreadReader() { run(); }

private:
	void ThreadFunc() override
	{
		LOG_FN("started");
		while(1)
		{
			unsigned delay = 100;
			EnterCriticalSection(&g_tasks_lock);
			if(size_t n = tasks.size())
			{
				auto t = tasks.pop();
				LOG_FN("process tasks " << t << " (" << n << " left)");
				delay = t * 1000;
			}
			else
				LOG_FN("no tasks, wait...");
			LeaveCriticalSection(&g_tasks_lock);
			Sleep(delay);
		}
		LOG_FN("done");
	}
};

// ====================================
template<typename T, typename... Args>
static std::unique_ptr<T> make_unique()
{
	return std::unique_ptr<T>( new T(std::forward<Args>(args)...) );
}


template<typename T>
static DWORD wait_all(const T& threads)
{
	unsigned n = threads.size();
	std::vector<HANDLE> hh(n);
	for(unsigned i = 0; i < n; ++i)
		hh[i] = threads[i]->handle();
	return WaitForMultipleObjects(hh.size(), &hh[0], TRUE/*wait all*/, INFINITE);
}


int main(int, const char**)
{
	LOG("Readers-Writer Sample");

	try
	{
		InitializeCriticalSection(&g_tasks_lock);

		// 1 writer & 8 readers
		std::vector<std::unique_ptr<Thread>> threads;

		// Create a writer
		threads.push_back(make_unique<ThreadWriter>());
		// Create readers
		for(unsigned i = 1; i < 8; ++i)
			threads.push_back(make_unique<ThreadReader>());

		switch(DWORD res = wait_all(threads))
		{
		case WAIT_OBJECT_0:
			LOG("All threads succesfully exited");
		}
	}
	catch(const exc_thread_create& e)
	{
		LOG("Error: " << e.what());
	}

	getchar();
	return 0;
}
