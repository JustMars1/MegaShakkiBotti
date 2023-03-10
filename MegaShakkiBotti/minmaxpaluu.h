#pragma once
#include <iostream>
#include "siirto.h"

// luokka, jonka avulla saadaan palautettua minmax:ssa sek? siirto-olio ett? evaluointifunktion arvo
// Struct ajaisi saman asian. Kun ei rakenneta gettereit? ja settereit?, niin ei tarvita toteutus .cpp tiedostoa
class MinMaxPaluu
{
public:
    float evaluointiArvo;
    Siirto parasSiirto;
    
    MinMaxPaluu(float arvo, Siirto siirto);
    MinMaxPaluu();
};

std::ostream& operator<<(std::ostream& os, const MinMaxPaluu& minmaxpaluu);
