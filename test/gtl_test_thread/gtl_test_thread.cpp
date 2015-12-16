// gtl_test_thread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <future>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "../../gtl/timer.h"

#ifndef THREAD_LOCAL_CPP11
#define	thread_local __declspec(thread)
#endif

using namespace std;
thread_local unsigned sum_total = 0;
static const unsigned THRESHOLD = 100000;
static vector<unsigned> v;
atomic_uint total_iterations;
mutex mx;

mutex m, mq;
condition_variable cv;

queue<int> q;

class ConcurrencyTester{
public:
	typedef int (*calculate_fun_ptr)(void);

	static int a ; 
	static int b ;
	
	

	static int calculateA(){
		cout << "Calculating A..." << endl;
		this_thread::sleep_for(chrono::seconds(5));
		return b*(b+1)-b;
	}
	static int calculateB(){
		cout << "Calculating B..." << endl;
		this_thread::sleep_for(chrono::seconds(5));
		return  a*(a+a*(a+1));
	}
	static int calculateC(){
		cout << "Calculating C..." << endl;
		this_thread::sleep_for(chrono::seconds(5));
		return  a+a*b;
	}

	static void findMultiples(int n) {

	  for (int i = 0; i < 100000; i++)

		if (i%n==0)

		  cout << i << " is a multiple of " << n << endl;

	}


	static void testAsychronousTask(){
		cout.sync_with_stdio(true);
		gtl::Timer timer;
		timer.start(" sequential running");
		long long int s = ConcurrencyTester::calculateA()+ConcurrencyTester::calculateB()+ConcurrencyTester::calculateC();
		timer.end();

		timer.start(" asynchronous running");
		future<int> f1 = async(ConcurrencyTester::calculateB);
		future<int> f2 = async(ConcurrencyTester::calculateC);
		s = (ConcurrencyTester::calculateA() + f1.get()) * f2.get();
		timer.end();
			

		timer.write(cout);
	}
	static void testThread(){
		cout.sync_with_stdio(true);

		thread th(&ConcurrencyTester::findMultiples, 1234);
		ConcurrencyTester::findMultiples(1234);
		th.join();
		cout<<" press any key...\n";
		_getch();
	}

	static void funcPromise(calculate_fun_ptr f, promise<int> &p ){
		p.set_value(f());
	}

	static void testPromises(){
		

		cout.sync_with_stdio(true);
		gtl::Timer timer;
		timer.start(" sequential running");
		long long int s = ConcurrencyTester::calculateA()+ConcurrencyTester::calculateB()+ConcurrencyTester::calculateC();
		timer.end();

		timer.start(" asynchronous running");
		promise<int> p1, p2;
		future<int> f1 = p1.get_future();
		future<int> f2 = p2.get_future();

		thread t1(&ConcurrencyTester::funcPromise, ConcurrencyTester::calculateB, std::ref(p1));
		thread t2(&ConcurrencyTester::funcPromise, ConcurrencyTester::calculateC, std::ref(p2));
	
		s = (calculateA() + f1.get()) * f2.get();
		t1.join(); 
		t2.join();
		timer.end();
			

		timer.write(cout);
	}

	static void sum_until_element_with_threshold(unsigned element, unsigned threshold, exception_ptr& pExc)
	{
		try{
			find_if_not(begin(v), end(v), [=](const unsigned i) -> bool {
				bool ret = (i!=element);
				sum_total+= i;
				if (sum_total>threshold)
					throw runtime_error("Sum exceeded threshold.");
				return ret;
			});
			stringstream ss;
			ss << "(Thread #" << this_thread::get_id() << ") " <<
				"Sum of elements until " << element << " is found: " << sum_total << endl;
			cout << ss.str();
		} catch (...) {
			pExc = current_exception();
		}
	}

	static void dealWithExceptionIfAny(exception_ptr pExc)
	{
		try
		{
			if (!(pExc==exception_ptr()))
				rethrow_exception(pExc);
		} catch (const exception& exc) {
			cout << "(Main thread) Exception received from thread: " << exc.what() << endl;
		}
	}

	static void testThreadLocal(){
		

		cout.sync_with_stdio(true);
		exception_ptr pExc1, pExc2;

		v.clear();
		for (size_t i = 0;i<1000;++i)
		v.push_back(i);

		for_each(begin(v), end(v), [](unsigned i) {
			size_t rnd = rand()%v.size();
			swap(v[i], v[rnd]);
		});

		thread th(&ConcurrencyTester::sum_until_element_with_threshold, 0, THRESHOLD, ref(pExc1));
		ConcurrencyTester::sum_until_element_with_threshold(100, THRESHOLD, ref(pExc2));
		th.join();

		ConcurrencyTester::dealWithExceptionIfAny(pExc1);
		ConcurrencyTester::dealWithExceptionIfAny(pExc2);

		cout << "Press any key...";
		_getch();
	}
	static void find_element(unsigned element)
	{
		unsigned iterations = 0;
		stringstream ss;

		find_if(begin(v), end(v), [=, &iterations](const unsigned i) -> bool {
			++iterations;
			return (i==element);
		});
		total_iterations+= iterations;

		ss << "Thread #" << this_thread::get_id() << ": found after " << iterations << " iterations." << endl;
		cout << ss.str();
	}

	static void testAtomic(){
		cout.sync_with_stdio(true);

		total_iterations = 0;

		for (size_t i = 0;i<1000;++i)
			v.push_back(i);

		for_each(begin(v), end(v), [](unsigned i) {
			size_t rnd = rand()%v.size();
			unsigned swapper = v[i];
			v[i] = v[rnd];
			v[rnd] = swapper;
		});

		thread th(find_element, 0);
		find_element(100);
		th.join();

		cout << total_iterations << " total iterations." << endl;

		cout << "Press any key...";
		_getch();
	}

	static void funcA()
	{
		stringstream ss;
		for (int i = 0; i<3; ++i)
		{
			this_thread::sleep_for(chrono::seconds(1));

			ss.str("");
			ss << "funcA: locking with wait... " << endl;
			cout << ss.str();
			lock_guard<mutex> lg(mx);
			ss.str("");
			ss << "funcA: lock secured... " << endl;
			cout << ss.str();

			this_thread::sleep_for(chrono::seconds(rand()%2));

			ss.str("");
			ss << "funcA: releasing lock." << endl;
			cout << ss.str();
		}
	}

	static void funcB()
	{
		int successful_attempts = 0;
		stringstream ss;
		for (int i = 0; i<5; ++i)
		{
			unique_lock<mutex> ul(mx, try_to_lock_t());
			if (ul)
			{
				++successful_attempts;

				ss.str("");
				ss << "funcB: lock attempt successful." << endl;
				cout << ss.str();

				this_thread::sleep_for(chrono::seconds(rand()%3));

				ss.str("");
				ss << "funcB: releasing lock." << endl;
				cout << ss.str();
			} else {
				ss.str("");
				ss << "funcB: lock attempt unsuccessful. Hibernating..." << endl;
				cout << ss.str();
				this_thread::sleep_for(chrono::seconds(1));
			}
		}
		ss.str("");
		ss << "funcB: " << successful_attempts << " successful attempts." << endl;
		cout << ss.str();
	}

	static void testMutex(){
		cout.sync_with_stdio(true);

		thread th(&ConcurrencyTester::funcA);
		ConcurrencyTester::funcB();
		th.join();

		cout << "Press any key...";
		_getch();
	}

	static void producer()
	{
		stringstream ss;
		for (int i = 0;i<5;++i)
		{
			this_thread::sleep_for(chrono::seconds(rand()%1));
			mq.lock();	q.push(i);	mq.unlock();
			ss.str("");
			ss << "Producer: element " << i << " queued." << endl;
			cout << ss.str();
			cv.notify_all();
		}
	}

	static void consumer()
	{
		unique_lock<mutex> l(m);
		int failed_attempts = 0;
		stringstream ss;
		while (true)
		{
			mq.lock();
			if (q.size())
			{
				int elem = q.front();
				q.pop();
				mq.unlock();
				failed_attempts = 0;
				ss.str("");
				ss << "Consumer: fetching " << elem << " from queue." << endl;
				cout << ss.str();
				this_thread::sleep_for(chrono::seconds(rand()%1));
			} else {
				mq.unlock();
				if (++failed_attempts>1)
				{
					ss.str("");
					ss << "Consumer: two consecutive failed attempts -> Exiting." << endl;
					cout << ss.str();
					break;
				} else {
					ss.str("");
					ss << "Consumer: queue not ready -> going to sleep." << endl;
					cout << ss.str();
					cv.wait_for(l, chrono::seconds(2));
				}
			}
		}
	}
	static void testConditionVariable(){
		cout.sync_with_stdio(true);

		thread th(consumer);
		producer();
		th.join();

		cout << "Press any key...";
		_getch();
	}
};

int ConcurrencyTester::a=10;
int ConcurrencyTester::b=10;

int _tmain(int argc, _TCHAR* argv[])
{
	//ConcurrencyTester::testAsychronousTask();
	//ConcurrencyTester::testThread();
	//ConcurrencyTester::testPromises();
	//ConcurrencyTester::testThreadLocal();
	//ConcurrencyTester::testAtomic();
	//ConcurrencyTester::testMutex();
	ConcurrencyTester::testConditionVariable();
	return 0;
}

