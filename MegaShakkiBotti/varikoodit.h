#pragma once
#include <string>
#include <string_view>

enum Vari : uint8_t
{
    musta = 30,
    punainen = 31,
    vihrea = 32,
    keltainen = 33,
    sininen = 34,
    purppura = 35,
    turkoosi = 36,
    harmaa = 37,
    tumman_harmaa = 90,
    kirkkaan_punainen = 91,
    kirkkaan_vihrea = 92,
    kirkkaan_keltainen = 93,
    kirkkaan_sininen = 94,
    kirkkaan_purppura = 95,
    kirkkaan_turkoosi = 96,
    valkoinen = 97
};

std::string tekstivari(Vari vari);
std::string taustavari(Vari vari);

constexpr std::string_view resetoiVarit() { return "\033[0m"; }
