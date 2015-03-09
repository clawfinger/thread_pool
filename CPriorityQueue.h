
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <queue>
#include <boost/function.hpp>
typedef std::pair<unsigned int, boost::function0<void> > priorityPair;


class CPriorityPredicate          //�������� ��� ���������� ����� � ������������ � �����������
{
public:
	bool operator()(priorityPair& left_operand, priorityPair& right_operand) const;
};


class CPriorityQueue              //������� ����� � ������������ � ������������
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
	void stopAll();         //��������� ������ �������. ����������� � ���������� ������ ������.
	void stopOne();			//�������� ������ ������� �����				
	void push(int priority, const boost::function0<void>& new_task);  //���������� ������ � �����������
	bool notEmptyOrStopped();           //�������� ��� m_ConditionVariable.wait. ����� �� wait ���� ��������� ����� ������ ��� ��������� ���� ��������
	bool pop(boost::function0<void>& poppedTask);
	int size();

};