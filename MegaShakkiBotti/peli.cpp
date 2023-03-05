#include "peli.h"

Peli::Peli(Pelaaja valkoinen, Pelaaja musta, Asema aloitusAsema)
: valkoinen(valkoinen)
, musta(musta)
, _aloitusAsema(aloitusAsema)
, asema(aloitusAsema) {}

Peli::Peli() : Peli(Pelaaja(false, 0), Pelaaja(true, 4), Asema()) {}

const Asema& Peli::getAloitusAsema()
{
    return _aloitusAsema;
}
