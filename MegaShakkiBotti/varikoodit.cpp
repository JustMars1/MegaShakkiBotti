#include "varikoodit.h"
#include "kayttoliittyma.h"

std::string tekstivari(Vari vari)
{
    if (Kayttoliittyma::getInstance().getVaritaRuudut())
    {
        return std::string("\033[38;5;") + std::to_string(vari) + "m";
    }
    else
    {
        return "";
    }
}
std::string taustavari(Vari vari)
{
    if (Kayttoliittyma::getInstance().getVaritaRuudut())
    {
        return std::string("\033[48;5;") + std::to_string(vari) + "m";
    }
    else
    {
        return "";
    }
}

std::string resetoiVarit()
{
    return Kayttoliittyma::getInstance().getVaritaRuudut() ? "\033[0m" : "";
}
