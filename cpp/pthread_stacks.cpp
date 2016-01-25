/**
 * Access stack of a thread from another thread
 *
 * Note that the stack of every pthread is protected
 * with a guard page by default.
 */
#include <iostream>


struct ThreadInfo
{
	// Caller Data
	pthread_t handle;
	// Callee Data
	bool writer;

	unsigned long rsp;
	unsigned short cs, ds, ss, fs, gs, es;
	unsigned long rflags;
};

// ====================================
static volatile unsigned* g_ptr;
static bool g_ready_to_write;
static bool g_ready_to_read;

#include <pthread.h>
pthread_mutex_t g_mutex_log = PTHREAD_MUTEX_INITIALIZER;
#define THREAD_LOG(msg) \
	pthread_mutex_lock(&g_mutex_log); \
	std::cout << msg << std::endl; \
	pthread_mutex_unlock(&g_mutex_log)
static std::ostream& operator<<(std::ostream& os, volatile void* ptr)
{
	os << const_cast<const void*>(ptr);
	return os;
}


static void* thread(void* arg)
{
	ThreadInfo& info = *(ThreadInfo*)arg;

	asm volatile
	(
		//"int $3\n"
		"mov %%rsp, %[rsp]\n"
		"mov %%cs, %[cs]\n"
		"mov %%ds, %[ds]\n"
		"mov %%ss, %[ss]\n"
		"mov %%fs, %[fs]\n"
		"mov %%gs, %[gs]\n"
		"mov %%es, %[es]\n"
		"pushf\n"
		"pop %[rflags]\n"
		: [rsp]"=m"(info.rsp),
		  [cs]"=m"(info.cs), [ds]"=m"(info.ds), [ss]"=m"(info.ss),
		  [fs]"=m"(info.fs), [gs]"=m"(info.gs), [es]"=m"(info.es),
		  [rflags]"=m"(info.rflags)
		:: "memory"
	);

	if(info.writer)
	{
		// Writer
		THREAD_LOG("Writer: waiting for reader...");
		while(!g_ready_to_write)
			usleep(1);

		static const unsigned ValDirty = 0x0BADF00D;
		*g_ptr = ValDirty;
		THREAD_LOG("Writer: modified with 0x" << std::hex << ValDirty << " @ " << g_ptr);
		//*(long*)((char*)g_ptr + 0x8C) = 1;
		g_ready_to_read = true;
	}
	else
	{
		// Reader
		static const unsigned ValInit = 0xDEADBEEF;
		volatile unsigned local = ValInit;
		g_ptr = &local;
		//asm volatile("int $3\n");
		THREAD_LOG("Reader: initialized with 0x" << std::hex << ValInit << " @ " << g_ptr);
		g_ready_to_write = true;

		while(!g_ready_to_read)
			usleep(1);
		THREAD_LOG("Reader: " << ((local == ValInit) ? "NOT " : "") << "modified (0x" << std::hex << local << ')');
	}


	return NULL;
}

// ====================================
int main(int, char**)
{
	static const unsigned N = 2;

	if(sizeof(void*) != 8)
	{
		std::cout << "The binary must be compiled as a " << (sizeof(void*) * 8) << "-bit application" << std::endl;
		return 1;
	}

	ThreadInfo threads[N];
	memset(threads, 0, sizeof(threads));

	// Create threads
	bool bOK = true;
	unsigned i;
	for(i = 0; i < N; ++i)
	{
		threads[i].writer = i;
		if( pthread_create(&threads[i].handle, NULL, thread, &threads[i]) )
		{
			std::cout << "Failed to create a thread #" << i << std::endl;
			bOK = false;
			break;
		}
	}

	// Wait for completion
	for(--i; i < N; --i)
		pthread_join(threads[i].handle, NULL);
	if(!bOK)
		return 1;

	// Print
	for(i = 0; i < N; ++i)
	{
		const ThreadInfo& info = threads[i];
		std::cout << "Thread #" << i << std::endl
				  << "\tRSP = 0x" << std::hex << info.rsp << std::endl
				  << "\tCS = 0x" << std::hex << info.cs << std::endl
				  << "\tDS = 0x" << std::hex << info.ds << std::endl
				  << "\tSS = 0x" << std::hex << info.ss << std::endl
				  << "\tFS = 0x" << std::hex << info.fs << std::endl
				  << "\tGS = 0x" << std::hex << info.gs << std::endl
				  << "\tES = 0x" << std::hex << info.es << std::endl
				  << "\tRFLAGS = 0x" << std::hex << info.rflags << std::endl;
	}

	return 0;
}

