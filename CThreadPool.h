
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "CPriorityQueue.h"

class CThreadPool
{
private:

	bool m_QueueStopFlag;
	bool m_WorkStopFlag;
	bool m_QueueIsFullFlag;
	int m_threadsNumber;
	int m_MaxQueueSize;
	std::vector<boost::shared_ptr<boost::thread> > m_Threads;
	CPriorityQueue m_TaskQueue;

public:

	bool queueFull() ;                           //��������, ��������� �� ������������ ������ �������
	void setMaxQueueSize(int size);				 //��������� ������������� ������� �������
	void addThreads(int number);				 //��������� ������
	void removeThreads(int number);				  //������� ������ �� ����
	bool enqueueTask(int priority, const boost::function0<void>& new_task);           //���������� ������ � �������
	void runTask();                            //������ ������ � ������ ���� ������� ���� � �������
	void stopQueue();        //��������� ���������� ����� ����� � �������. ��� ���������� �������� ������ �� �������.
	void stopWork();       //��������� ������ ���������� �� ������� ��������� � ������� �����. �������� �������� �������
	void abortWork();       //������ ��������� ����, ����������� ���� ���������� �����
	~CThreadPool();
	CThreadPool(int maxQueueSize = 10):
		m_QueueStopFlag(false), m_threadsNumber(0),
		m_MaxQueueSize(maxQueueSize), m_QueueIsFullFlag(false), m_WorkStopFlag(false)

	{}
};