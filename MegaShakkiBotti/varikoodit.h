#pragma once
#include <string>
#include <cstdint>

enum Vari
{
    musta = 16,
    punainen = 88,
    vihrea = 28,
    keltainen = 100,
    liila = 176,
    kulta = 178,
    luumu = 96,
    harmaa = 188,
    tumman_harmaa = 244,
    kirkkaan_punainen = 196,
    kirkkaan_vihrea = 46,
    kirkkaan_keltainen = 226,
    kirkkaan_liila = 183,
    kirkkaan_kulta = 220,
    kirkkaan_turkoosi = 51,
    valkoinen = 231,
};

std::string tekstivari(Vari vari);
std::string taustavari(Vari vari);

std::string resetoiVarit();
