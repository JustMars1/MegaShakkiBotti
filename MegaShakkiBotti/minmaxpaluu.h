#pragma once
#include "siirto.h"

// luokka, jonka avulla saadaan palautettua minmax:ssa sekä siirto-olio että evaluointifunktion arvo
// Struct ajaisi saman asian. Kun ei rakenneta gettereitä ja settereitä, niin ei tarvita toteutus .cpp tiedostoa
class MinMaxPaluu {
public:
	double evaluointiArvo;
	Siirto _parasSiirto;
    
    MinMaxPaluu(double arvo, Siirto siirto)
    : evaluointiArvo{arvo}
    , _parasSiirto(siirto) {}
};
