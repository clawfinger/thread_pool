#include "stdafx.h"
#include "CPriorityQueue.h"

bool CPriorityPredicate::operator()(priorityPair& left_operand, priorityPair& right_operand) const
{
	return left_operand.first < right_operand.first;
}

void CPriorityQueue::stopAll()                               //Остановка работы очереди. Пробуждение и завершение ждущих тредов.
{
	boost::unique_lock<boost::mutex> lock(m_QueueMutex);
	m_stopFlag = true;
	m_ConditionVariable.notify_all();
}

void CPriorityQueue::stopOne()
{
	boost::unique_lock<boost::mutex> lock(m_QueueMutex);
	m_stopFlag = true;
	m_ConditionVariable.notify_one();
}

void CPriorityQueue::push(int priority, const boost::function0<void>& new_task)
{
	boost::unique_lock<boost::mutex> lock(m_QueueMutex);
	m_Queue.push(std::move(std::make_pair(priority, new_task)));
	m_ConditionVariable.notify_one();                                   //Будим ждущий поток при поступлении новой задачи в очередь
}

bool CPriorityQueue::notEmptyOrStopped()           //Предикат для m_ConditionVariable.wait. Выход из wait если добавлена новая задача или устанолен флаг останоки
{
	return !m_Queue.empty() || m_stopFlag;
}

bool CPriorityQueue::pop(boost::function0<void>& poppedTask)
{
	boost::unique_lock<boost::mutex> lock(m_QueueMutex);
	m_ConditionVariable.wait(lock, boost::bind(&CPriorityQueue::notEmptyOrStopped, this)); //Ставим поток в ожидание, пока не появятся задачи в очереди
	if(!m_stopFlag)
	{
		poppedTask=std::move(m_Queue.top().second);
		m_Queue.pop();
		return true;
	}
	else
	{
		return false;
	}
}

int CPriorityQueue::size()
{
	boost::lock_guard<boost::mutex> lock (m_QueueMutex);
	return m_Queue.size();
}