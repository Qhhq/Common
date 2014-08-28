#include <fstream>

#define BOOST_ALL_DYN_LINK
#include <boost/locale.hpp>

#define NAME L"Тест"

int main(int argc, char *argv[])
{
	std::ofstream f("c:/123\\test.csv", std::ios::out | std::ios::binary);
	unsigned char marker[] = { 0xEF, 0xBB, 0xBF, 0 };
	f << marker;
	f << boost::locale::conv::utf_to_utf<char>(NAME);
	f << boost::locale::conv::utf_to_utf<char>("test");
	return 0;
}