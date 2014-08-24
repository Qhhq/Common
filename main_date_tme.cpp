
#define BOOST_ALL_DYN_LINK

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/io/time_point_io.hpp>

#include <sstream>
#include <fstream>
#include <ctime>

typedef boost::chrono::system_clock::time_point SysTime;

std::string GetStrDateTime(long long milliseconds)
{
	boost::chrono::milliseconds ms(milliseconds);
	boost::chrono::seconds sec = boost::chrono::duration_cast<boost::chrono::seconds> (ms);

	time_t t = sec.count();
	tm * ptm = std::localtime(&t);

	size_t fractionalSeconds = ms.count() % 1000;

	char *format = "%d/%m/%Y %H:%M:%S,";
	char buffer[100] = { 0 };
	std::strftime(buffer, 100, format, ptm);

	std::stringstream formatted;
	formatted << buffer << fractionalSeconds;

	return formatted.str();
}


long long timestamp()
{
	const SysTime tm = boost::chrono::system_clock::now();
	return boost::chrono::duration_cast<boost::chrono::milliseconds>(tm.time_since_epoch()).count();
}


int main(int argc, char* argv[])
{
	long long nm = timestamp();
	std::cout << GetStrDateTime(nm);

	return 0;
}

