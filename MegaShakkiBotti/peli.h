#pragma once

#include <optional>
#include "pelaaja.h"
#include "asema.h"
#include "siirto.h"

class Peli {
public:
    Pelaaja valkoinen;
    Pelaaja musta;
    Asema asema;
    
    int siirtoparilaskuri;
    
    std::optional<Siirto> viimeisinSiirto;
    
    Peli(Pelaaja valkoinen = Pelaaja(false, 0), Pelaaja musta = Pelaaja(true, 4));
};
