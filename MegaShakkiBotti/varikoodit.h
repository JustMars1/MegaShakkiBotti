#pragma once
#include <string>
#include <cstdint>

enum Vari
{
    musta = 16,
    punainen = 88,
    vihrea = 28,
    keltainen = 100,
    sininen = 18,
    purppura = 93,
    turkoosi = 27,
    harmaa = 250,
    tumman_harmaa = 244,
    kirkkaan_punainen = 196,
    kirkkaan_vihrea = 46,
    kirkkaan_keltainen = 226,
    kirkkaan_sininen = 21,
    kirkkaan_purppura = 201,
    kirkkaan_turkoosi = 51,
    valkoinen = 231
};

std::string tekstivari(Vari vari);
std::string taustavari(Vari vari);

std::string resetoiVarit();
