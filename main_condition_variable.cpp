
#define BOOST_ALL_DYN_LINK

#include <iostream>
#include <fstream>
#include <ctime>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <queue>


std::queue<int> produced_nums;
boost::mutex m;
boost::condition_variable cond_var;
bool done = false;
bool notified = false;


void consumer()
{
	boost::unique_lock<boost::mutex> lock(m);
	while (!done) {
		while (!notified) {  // loop to avoid spurious wakeups
			cond_var.wait(lock);
		}
		while (!produced_nums.empty()) {
			std::cout << "consuming " << produced_nums.front() << '\n';
			produced_nums.pop();
		}
		notified = false;
	}
}

int main()
{
	boost::thread cons(consumer);

	for (int i = 0; i < 5; ++i)
	{
		boost::this_thread::sleep_for(boost::chrono::seconds(1));

		boost::unique_lock<boost::mutex> lock(m);
		std::cout << "it " << i << std::endl;
		produced_nums.push(i);
		notified = true;
		cond_var.notify_one();
	}

	done = true;
	cond_var.notify_one();

	cons.join();

	return 0;
}