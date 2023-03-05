#pragma once
#include <deque>
#include "pelaaja.h"
#include "asema.h"
#include "siirto.h"

class Peli {
public:
    Pelaaja valkoinen;
    Pelaaja musta;
    Asema asema;
    
    std::deque<Siirto> siirtoHistoria;
    
    Peli(Pelaaja valkoinen, Pelaaja musta, Asema aloitusAsema);
    Peli();
    
    const Asema& getAloitusAsema();
private:
    Asema _aloitusAsema;
};
