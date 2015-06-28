#include <iostream>
#include <cstring>
#include <iconv.h>
#include <bitset>

int main(int /*argc*/, char */*argv*/[])
{
    iconv_t conv = iconv_open("UCS-2BE", "WCHAR_T");
    if(reinterpret_cast<iconv_t>(-1) == conv)
    {
        throw std::runtime_error("Error opening iconv");
    }

    std::wstring str = L"Тест";
    std::vector<wchar_t> in(str.begin(), str.end());

    size_t inlSize = str.length() * sizeof(wchar_t);
    size_t outSize = str.length() * sizeof(uint16_t);
    std::vector<char> out(outSize);

    char *inbuf = reinterpret_cast<char *>(&in[0]);
    char *outbuf = &out[0];

    iconv(conv, &inbuf, &inlSize, &outbuf, &outSize);
    iconv_close(conv);

    int i = 0;

    for(const auto &item: out)
    {
        std::cout << std::bitset<8>(item);
        ++i;
        if (0 == i%2)
        {
           std::cout << std::endl;
        }
    }

    return 0;
}
