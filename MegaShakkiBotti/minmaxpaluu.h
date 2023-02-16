#pragma once
#include <iostream>
#include "siirto.h"

// luokka, jonka avulla saadaan palautettua minmax:ssa sekä siirto-olio että evaluointifunktion arvo
// Struct ajaisi saman asian. Kun ei rakenneta gettereitä ja settereitä, niin ei tarvita toteutus .cpp tiedostoa
class MinMaxPaluu
{
public:
	float evaluointiArvo;
	Siirto _parasSiirto;
    
    MinMaxPaluu(float arvo, Siirto siirto);

    MinMaxPaluu();
};

std::ostream& operator<<(std::ostream& os, const MinMaxPaluu& minmaxpaluu);
