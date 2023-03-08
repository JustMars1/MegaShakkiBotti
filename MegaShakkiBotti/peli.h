#pragma once
#include <deque>
#include "pelaaja.h"
#include "asema.h"
#include "siirto.h"

class Peli {
public:
    static std::optional<Peli> lue(std::string fen);
    
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

std::ostream& operator<<(std::ostream& os, const Peli& peli);
