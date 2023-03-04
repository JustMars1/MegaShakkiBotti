#include "peli.h"

Peli::Peli(Pelaaja valkoinen, Pelaaja musta)
: valkoinen(valkoinen)
, musta(musta)
, viimeisinSiirto(std::nullopt)
, siirtoparilaskuri{1} {}

Peli::Peli() : Peli(Pelaaja(false, 0), Pelaaja(true, 4)) {}
