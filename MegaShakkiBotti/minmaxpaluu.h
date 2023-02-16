#pragma once
#include <iostream>
#include "siirto.h"

// luokka, jonka avulla saadaan palautettua minmax:ssa sek� siirto-olio ett� evaluointifunktion arvo
// Struct ajaisi saman asian. Kun ei rakenneta gettereit� ja settereit�, niin ei tarvita toteutus .cpp tiedostoa
class MinMaxPaluu
{
public:
	float evaluointiArvo;
	Siirto _parasSiirto;
    
    MinMaxPaluu(float arvo, Siirto siirto)
    : evaluointiArvo{arvo}
    , _parasSiirto(siirto) {}

    MinMaxPaluu() : evaluointiArvo{0} , _parasSiirto(Siirto()) {}
};

std::ostream& operator<<(std::ostream& os, const MinMaxPaluu& minmaxpaluu) 
{
    os << minmaxpaluu.evaluointiArvo << minmaxpaluu._parasSiirto;
    return os;
}
