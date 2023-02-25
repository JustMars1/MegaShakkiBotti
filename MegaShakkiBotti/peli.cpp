#include "peli.h"

Peli::Peli(Pelaaja valkoinen, Pelaaja musta)
: valkoinen(valkoinen)
, musta(musta)
, viimeisinSiirto(std::nullopt) {}
