#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
	ThreadPool(size_t threadCount);
	~ThreadPool();
	void enqueueTask(std::function<void()> task);
    void waitForAll();
private:
    // Worker threads that process tasks from the queue
    std::vector<std::thread> workers;
    // Task queue to hold the tasks
    std::queue<std::function<void()>> taskQueue;

    // Synchronization mechanisms
    std::mutex queueMutex; // Protects access to the queue
    std::condition_variable condition; // Used for thread synchronization
    std::condition_variable doneCondition; // Used by waitForAll() to wait for completion
    std::atomic<bool> stopFlag; // Signals threads to stop execution
    std::atomic<size_t> active_threads;    //Number of active tasks (enqueued + running)

    void workerThread();
};