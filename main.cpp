#include <fstream>
#include <codecvt>

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

int main(int argc, char *argv[])
{
    std::ofstream f("test.csv", std::ios::out | std::ios::binary);
    unsigned char marker[] = { 0xEF, 0xBB, 0xBF, 0 };
    f << marker;
    f << wstring_to_utf8(L"Тест");

    return 0;
}