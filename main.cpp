// ConsoleApplication18.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "CThreadPool.h"






void Task1()
{   
	int i = 0;
	while(true)
	{
		std::cout << i++ <<std::endl;
		boost::this_thread::interruption_point();
	}
	
}
void Task2()
{
	while(true)
		{
			std::cout << "Yet another useless work" << std::endl;
		}
}
void Task3()
{
	std::cout << "This thread will be deleted immediatly" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{

	CThreadPool pool;
	pool.addThreads(8);
	pool.enqueueTask(5, Task1);
	pool.enqueueTask(3, Task2);
	pool.enqueueTask(4, Task3);
	pool.enqueueTask(2, Task1);
	pool.removeThreads(1);
	pool.stopQueue();
	pool.enqueueTask(99, Task1);
	system("pause");
	pool.abortWork();
	system("pause");
	return 0;
}







