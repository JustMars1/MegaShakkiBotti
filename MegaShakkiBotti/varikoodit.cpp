#include "varikoodit.h"
#include "kayttoliittyma.h"

std::string tekstivari(Vari vari)
{
    if (Kayttoliittyma::getInstance().varitaRuudut)
    {
        return std::string("\033[") + std::to_string(vari) + "m";
    }
    else
    {
        return "";
    }
}
std::string taustavari(Vari vari)
{
    if (Kayttoliittyma::getInstance().varitaRuudut)
    {
        return std::string("\033[") + std::to_string(vari + 10) + "m";
    }
    else
    {
        return "";
    }
}

std::string resetoiVarit()
{
    return Kayttoliittyma::getInstance().varitaRuudut ? "\033[0m" : "";
}
