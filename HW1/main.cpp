#include <cstdint>
#include <iostream>
#include <vector>


#define PASTE10(num, stride, dest) {\
    uint8_t sTmp = static_cast<uint8_t>(num >> stride & (1 << 6) - 1);\
    sTmp += 128;\
    dest.push_back(sTmp);\
}

std::vector<uint8_t> to_utf8(const std::vector<uint32_t>& aInput)
{
    std::vector<uint8_t> sRes;
    for(const auto& sElem: aInput)
    {
        if (sElem < 0x80)
            sRes.push_back(static_cast<uint8_t>(sElem));
        else if (sElem < 0x800)
        {
            uint8_t sTmp = static_cast<uint8_t>(sElem >> 6 & (1 << 5) - 1);
            sTmp += (128 + 64);
            sRes.push_back(sTmp);
            PASTE10(sElem, 0, sRes)
        }
        else if (sElem < 0x10000)
        {
            uint8_t sTmp = static_cast<uint8_t>(sElem >> 12 & (1 << 4) - 1);
            sTmp += (128 + 64 + 32);
            sRes.push_back(sTmp);
            PASTE10(sElem, 6, sRes)
            PASTE10(sElem, 0, sRes)
        }
        else
        {
            uint8_t sTmp = static_cast<uint8_t>(sElem >> 18 & (1 << 3) - 1);
            sTmp += (128 + 64 + 32 + 16);
            sRes.push_back(sTmp);
            PASTE10(sElem, 12, sRes)
            PASTE10(sElem, 6, sRes)
            PASTE10(sElem, 0, sRes)
        }
    }
    return sRes;
}

std::vector<uint32_t> from_utf8(const std::vector<uint8_t>& aInput)
{
    std::vector<uint32_t> sRes;
    for(uint32_t i = 0; i< aInput.size(); ++i)
    {
        auto sElem = aInput[i];
        if (sElem < 128)
            sRes.push_back(sElem);
        else
        {
            uint32_t sBytes = 1;
            uint32_t k;
            for (k = 1 << 5; (sElem & k) != 0; k >>= 1)
                sBytes++;
            uint32_t sTmp = k - 1 & sElem;
            for (uint32_t step = 0; step < sBytes; ++step)
            {
                sTmp <<= 6;
                sTmp += aInput[i + 1 + step] & 63;
            }
            i += sBytes;
            sRes.push_back(sTmp);
        }
    }
    return sRes;
}

int main()
{
    std::vector<uint32_t> sTest = {500001, 14880, 120, 1200, 500};
    auto sRes = to_utf8(sTest);
    //for (const auto& sElem: sRes)
    //    std::cout << static_cast<uint32_t>(sElem) << std::endl;
    auto sTestTest = from_utf8(to_utf8(sTest));
//    for (const auto& sElem: sTest)
//        std::cout << sElem << std::endl;
    for (const auto& sElem: sTestTest)
        std::cout << sElem << std::endl;
    return 0;
}
