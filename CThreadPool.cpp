#include "stdafx.h"
#include "CThreadPool.h"

bool CThreadPool::queueFull()                         //Проверка, достигнут ли максимальный размер очереди
{
	return m_TaskQueue.size()>=m_MaxQueueSize;
}

void CThreadPool::setMaxQueueSize(int size)
{
	m_MaxQueueSize = size;
}

void CThreadPool::addThreads(int number)               //Добавляем потоки
{
	m_WorkStopFlag = false;
	for(int i = 0; i <number; ++i)
	{
		boost::shared_ptr<boost::thread> new_thread (new boost::thread(&CThreadPool::runTask, this));
		m_Threads.push_back(new_thread);
		++m_threadsNumber;
	}
}

void CThreadPool::removeThreads(int number)                //Удаляем потоки из пула
{
	for(int i = 0; i <number; ++i)
	{
		if(m_threadsNumber>0)
		{
			m_TaskQueue.stopOne();
			--m_threadsNumber;
		}
	}
};

bool CThreadPool::enqueueTask(int priority, const boost::function0<void>& new_task)            //Постановка задачи в очередь
{
	if(!m_QueueStopFlag)							 //Проверяем не остановлена ли работа очереди
	{
		if(m_TaskQueue.size() < m_MaxQueueSize)
		{
			m_TaskQueue.push(priority, new_task);
			return true;
		}
		else 
		{
			m_QueueIsFullFlag = true;         //Останавливаем очередь, ограничивая возможность добавления новых заданий
			return false;
		}
	}
	else return false;   
}

void CThreadPool::runTask()                              //Запуск задачи в работу если таковые есть в очереди
{
	while(!m_WorkStopFlag)                  //Прповеряем не остановлена ли работа пула
	{
		boost::function0<void> CurrentTask;
		if(m_TaskQueue.pop(CurrentTask))
		{
			m_QueueIsFullFlag = false;
			CurrentTask();
		}
		else
		{
			break;
		}
	}

}

void CThreadPool::stopQueue()         //Запрещает добавление новых задач в очередь. Пул продолжает получать задачи из очереди.
{
	m_QueueStopFlag = true;
}

void CThreadPool::stopWork()         //Остановка работы независимо от наличия ожидающих в очереди задач. Удаление объектов потоков
{
	m_TaskQueue.stopAll();
	for(auto thread : m_Threads)
	{
		thread->join();
	}
	m_Threads.clear();
}

void CThreadPool::abortWork()        //Полная остановка пула, прерываение всех выполяемых задач
{
	m_QueueStopFlag = true;
	for(auto thread : m_Threads)
	{
		thread->interrupt();
	}
	m_Threads.clear();
}

CThreadPool::~CThreadPool()
{
	stopWork();

}