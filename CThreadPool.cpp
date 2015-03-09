#include "stdafx.h"
#include "CThreadPool.h"

bool CThreadPool::queueFull()                         //��������, ��������� �� ������������ ������ �������
{
	return m_TaskQueue.size()>=m_MaxQueueSize;
}

void CThreadPool::setMaxQueueSize(int size)
{
	m_MaxQueueSize = size;
}

void CThreadPool::addThreads(int number)               //��������� ������
{
	m_WorkStopFlag = false;
	for(int i = 0; i <number; ++i)
	{
		boost::shared_ptr<boost::thread> new_thread (new boost::thread(&CThreadPool::runTask, this));
		m_Threads.push_back(new_thread);
		++m_threadsNumber;
	}
}

void CThreadPool::removeThreads(int number)                //������� ������ �� ����
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

bool CThreadPool::enqueueTask(int priority, const boost::function0<void>& new_task)            //���������� ������ � �������
{
	if(!m_QueueStopFlag)							 //��������� �� ����������� �� ������ �������
	{
		if(m_TaskQueue.size() < m_MaxQueueSize)
		{
			m_TaskQueue.push(priority, new_task);
			return true;
		}
		else 
		{
			m_QueueIsFullFlag = true;         //������������� �������, ����������� ����������� ���������� ����� �������
			return false;
		}
	}
	else return false;   
}

void CThreadPool::runTask()                              //������ ������ � ������ ���� ������� ���� � �������
{
	while(!m_WorkStopFlag)                  //���������� �� ����������� �� ������ ����
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

void CThreadPool::stopQueue()         //��������� ���������� ����� ����� � �������. ��� ���������� �������� ������ �� �������.
{
	m_QueueStopFlag = true;
}

void CThreadPool::stopWork()         //��������� ������ ���������� �� ������� ��������� � ������� �����. �������� �������� �������
{
	m_TaskQueue.stopAll();
	for(auto thread : m_Threads)
	{
		thread->join();
	}
	m_Threads.clear();
}

void CThreadPool::abortWork()        //������ ��������� ����, ����������� ���� ���������� �����
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