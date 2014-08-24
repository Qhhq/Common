

#define BOOST_ALL_DYN_LINK

#include <iostream>
#include <fstream>
#include <ctime>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "windows.h" 

bool isDeadline = false;

struct Item
{
	bool IsSuccess;
	int id;
	Item() : id(0), IsSuccess(0) {};
	~Item(){ std::cout << "delete " << id << "\n"; };
};

typedef boost::shared_ptr<Item> ItemPtr;
typedef std::list<ItemPtr> ItemPtrlist;

void ThreadFunc(ItemPtrlist *mylist)
{
	int counter = 10;
	while (true)
	{
		if (isDeadline)
		{
			ItemPtrlist::iterator it = mylist->begin();
			while (it != mylist->end())
			{
				if (0 == (*it)->IsSuccess)
				{
					mylist->erase(it++);
				}
				else
				{
					++it;
				}
			}
			break;
		}
		else
		{
			Sleep(1000);
			std::cout << "Sleep(1000)" << std::endl;
			--counter;
			if (0 == counter)
			{
				break;
			}
			continue;
		}
	}


}

int main(int argc, char* argv[])
{
	ItemPtrlist mylist;

	for (int i = 0; i < 10; ++i)
	{
		ItemPtr itt(new Item);
		itt->id = i;
		mylist.push_back(itt);
	}

	boost::condition_variable isDeadlineCv;

	std::cout << isDeadline << std::endl;
	boost::thread th = boost::thread(ThreadFunc, &mylist);

	boost::chrono::steady_clock::time_point start = boost::chrono::steady_clock::now();
	bool r = th.timed_join(boost::posix_time::seconds(20));
	boost::chrono::steady_clock::time_point end = boost::chrono::steady_clock::now();

	std::cout << "= " << r << std::endl;
	std::cout << "= " << boost::chrono::duration_cast<boost::chrono::seconds>(end - start).count() << std::endl;
	isDeadline = true;
	th.join();

	std::cout << "end " << std::endl;

	return 0;
}

