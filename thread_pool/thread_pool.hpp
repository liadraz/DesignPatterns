//----------------------------------------------------------------------------//
//							- THREAD_POOL -
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		API
//	AUTHOR 			Liad Raz
//	REVIEWER		
//	DATE			09 Jun 2022
//	FILES			thread_pool.cpp thread_pool_test.cpp thread_pool.hpp
// 
//----------------------------------------------------------------------------//

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <mutex>				// std::mutex
#include <condition_variable>	// std::condition_variable
#include <vector>				// std::vector
#include <queue>				// std::queue
#include <functional>			// std::function

#include <thread>				// std::thread


class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();
	ThreadPool(const ThreadPool& other_) = delete;
	ThreadPool& operator =(const ThreadPool& other_) = delete;

    void Start();
    void QueueJob(const std::function<void()>& job_);
    bool Busy();
    void Stop();

private:
	//
	// Functions

	// Each thread executes this function which keeps it alive
    void ThreadLoop();

	//
	// Members

	// Tells m_threads to stop looking for jobs
    bool m_shouldTerminate = false;

	// Prevents data races to the job queue
    std::mutex m_queueMutex;

	// Allows m_threads to wait on new jobs or termination
    std::condition_variable m_mutexCondition; 

    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_jobs;
};



void ThreadPool::Start() 
{
	// Max # of m_threads the system supports
    const uint32_t NUM_THREADS = std::thread::hardware_concurrency();

	// Resize the vector container to the size of #
    m_threads.resize(NUM_THREADS);

    for (uint32_t i = 0; i < NUM_THREADS; i++) 
	{
        m_threads[i] (ThreadLoop);
    }
}


// An infinite loop function. This is a while (true) loop waiting for the task queue to open up.
void ThreadPool::ThreadLoop() 
{
    while (true) 
	{
		// signature goes inside the angle brackets : takes zero arguments and returns nothing
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
			m_mutexCondition.wait(lock, [this] 
			{
                return !m_jobs.empty() || m_shouldTerminate;
            });

            if (m_shouldTerminate) {
                return;
            }

            job = m_jobs.front();
            m_jobs.pop();
        }

        job();
    }
}


// Add a new job to the pool; use a lock so that there isn't a data race.
void ThreadPool::QueueJob(const std::function<void()>& job) 
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_jobs.push(job);
    }

	// If any threads are waiting on *this, 
	//notify_one unblocks one of the waiting threads
    m_mutexCondition.notify_one();
}

// use thread_pool->QueueJob([] { /* ... */ });


// Function can be used in a while loop, such that the main thread 
// can wait the threadPool to complete all the tasks before calling 
// the threadPool destructor.
bool ThreadPool::Busy() 
{
    bool poolBusy = false;

    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        poolBusy = m_jobs.empty();
    }

    return poolBusy;
}



void ThreadPool::Stop() 
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_shouldTerminate = true;
    }

	// notifies all waiting threads
    m_mutexCondition.notify_all();

	// SYNTAX	for ( range_declaration : range_expression )
    for (std::thread& activeThread : m_threads) 
	{
        activeThread.join();
    }

    m_threads.clear();
}



#endif // THREAD_POOL_HPP

