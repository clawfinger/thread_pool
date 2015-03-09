
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <boost/function.hpp>
typedef std::pair<unsigned int, boost::function0<void> > priorityPair;


class CPriorityPredicate          //Предикат для сортировки задач в соответствии с приоритетом
{
public:
	bool operator()(priorityPair& left_operand, priorityPair& right_operand) const;
};


class CPriorityQueue              //Очередь задач с приоритетами и блокировками
{
private:

	boost::condition_variable m_ConditionVariable;
	boost::mutex m_QueueMutex;	
	std::priority_queue<priorityPair, std::vector<priorityPair>, CPriorityPredicate> m_Queue;	
	bool m_stopFlag;

public:
	CPriorityQueue()
		: m_stopFlag(false)
	{};
	void stopAll();         //Остановка работы очереди. Пробуждение и завершение ждущих тредов.
	void stopOne();			//Удаление одного ждущего треда				
	void push(int priority, const boost::function0<void>& new_task);  //Добавление задачи с приоритетом
	bool notEmptyOrStopped();           //Предикат для m_ConditionVariable.wait. Выход из wait если добавлена новая задача или устанолен флаг останоки
	bool pop(boost::function0<void>& poppedTask);
	int size();

};