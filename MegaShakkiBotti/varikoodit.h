#pragma once
#include <string>
#include <cstdint>

enum Vari
{
    musta = 0,
    punainen = 1,
    vihrea = 2,
    keltainen = 3,
    sininen = 4,
    purppura = 5,
    turkoosi = 6,
    harmaa = 7,
    tumman_harmaa = 8,
    kirkkaan_punainen = 9,
    kirkkaan_vihrea = 10,
    kirkkaan_keltainen = 11,
    kirkkaan_sininen = 12,
    kirkkaan_purppura = 13,
    kirkkaan_turkoosi = 14,
    valkoinen = 15,
};

std::string tekstivari(Vari vari);
std::string taustavari(Vari vari);

std::string resetoiVarit();

