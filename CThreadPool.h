
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

	bool queueFull() ;                           //Проверка, достигнут ли максимальный размер очереди
	void setMaxQueueSize(int size);				 //Установка максимального размера очереди
	void addThreads(int number);				 //Добавляем потоки
	void removeThreads(int number);				  //Удаляем потоки из пула
	bool enqueueTask(int priority, const boost::function0<void>& new_task);           //Постановка задачи в очередь
	void runTask();                            //Запуск задачи в работу если таковые есть в очереди
	void stopQueue();        //Запрещает добавление новых задач в очередь. Пул продолжает забирать задачи из очереди.
	void stopWork();       //Остановка работы независимо от наличия ожидающих в очереди задач. Удаление объектов потоков
	void abortWork();       //Полная остановка пула, прерываение всех выполяемых задач
	~CThreadPool();
	CThreadPool(int maxQueueSize = 10):
		m_QueueStopFlag(false), m_threadsNumber(0),
		m_MaxQueueSize(maxQueueSize), m_QueueIsFullFlag(false), m_WorkStopFlag(false)

	{}
};