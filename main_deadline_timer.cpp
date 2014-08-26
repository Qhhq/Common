

#define BOOST_ALL_DYN_LINK

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <Windows.h>

bool isRun = false;

boost::asio::io_service io;
boost::asio::deadline_timer timer(io);


void StopRun(const boost::system::error_code & /*e*/)
{
	isRun = false;
}


void Run(int id, int count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (isRun)
		{
			std::cout << "id = " << id << "\n";
			Sleep(1000);
		}
		else
		{
			break;
		}
	}
	timer.cancel();
	io.stop();
	io.reset();
}

void Timing()
{
	timer.async_wait(StopRun);
	io.run();
}

int main()
{
	for (size_t i = 0; i < 5; ++i)
	{
		std::cout << "Iteraton = " << i << "\n";

		timer.cancel();
		io.stop();
		io.reset();
		timer.expires_from_now(boost::posix_time::seconds(200));

		isRun = true;
		boost::thread th(Timing);

		Run(i, 5);

		th.join();
	}



	return 0;
}

