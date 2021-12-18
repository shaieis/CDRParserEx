#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/core/noncopyable.hpp>
#include <mutex>
#include "CDRRecord.h"

/*
	Uses boost's lockfree queue
*/
template <class Elem>
class Queue : private boost::noncopyable
{
public:
	Queue(size_t lockSize, size_t unlockSize) : 
		lfqueue(lockSize), m_lockSize(lockSize), m_unlockSize(unlockSize), m_currSize(0), m_closed(false)
	{};
	
	// pushes element into queue
	void push(const Elem& elem);

	// pops element from queue into ret. if empty - returns false
	bool pop(Elem& ret);
	void close();

private:
	std::atomic<size_t> m_currSize;
	boost::lockfree::queue<Elem> lfqueue;

	size_t m_lockSize;
	size_t m_unlockSize;

	std::mutex m_pushMutex;
	std::condition_variable m_pushCond;

	std::mutex m_popMutex;
	std::condition_variable m_popCond;

	std::atomic<bool> m_closed;
};

template<class Elem>
inline void Queue<Elem>::push(const Elem& elem)
{	
	if (m_currSize >= m_lockSize)
	{
		std::unique_lock<std::mutex> lock(m_pushMutex);
		m_pushCond.wait(lock, [&]() { return m_currSize <= m_unlockSize; });
	}

	++m_currSize;
	lfqueue.push(elem);
	m_popCond.notify_one();
}

template<class Elem>
inline bool Queue<Elem>::pop(Elem& ret)
{
	bool success = false;
	while (!(success = lfqueue.pop(ret)) && !m_closed)
	{
		std::unique_lock<std::mutex> lock(m_popMutex);
		m_popCond.wait(lock, [&]() { return !lfqueue.empty() || m_closed; });
	}

	// Either success == true and we were able to pop, or success == false which means the queue was closed and is empty
	if (!success) return false;

	--m_currSize;
	if (m_currSize <= m_unlockSize)
	{
		m_pushCond.notify_one();
	}

	return true;
}

template<class Elem>
inline void Queue<Elem>::close()
{
	m_closed = true;
	m_popCond.notify_all();
}
