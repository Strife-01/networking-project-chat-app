#ifndef Included_BlockingQueue
#define Included_BlockingQueue

#include <queue>
#include <thread>
#include <string>
#include <list>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T> class BlockingQueue
{

private:
	queue<T> theQueue;
	mutex mutex_queue;
	condition_variable cond;

public:
	BlockingQueue(){
		}

	T pop() {
		std::unique_lock<std::mutex> lk(mutex_queue);
		if (theQueue.empty()) { //we need to wait if there is nothing in the queue!
			cond.wait(lk);
		}
		T ret = theQueue.front();
		theQueue.pop();
		lk.unlock();
		return ret;
	}

	void push(T object) {
		mutex_queue.lock();
		theQueue.push(object);
		mutex_queue.unlock();
		cond.notify_one();
	}

};

#endif