#include "varikoodit.h"

std::string tekstivari(Vari vari)
{
    return std::string("\033[") + std::to_string(vari) + "m";
}
std::string taustavari(Vari vari)
{
    return std::string("\033[") + std::to_string(vari + 10) + "m";
}

