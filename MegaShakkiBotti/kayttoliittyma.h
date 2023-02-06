#pragma once
#include "asema.h"
#include "nappula.h"
#include "peli.h"
#include "siirto.h"

// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.

class Kayttoliittyma
{
public:
    static Kayttoliittyma& getInstance();
    
    void piirraLauta(const std::list<Siirto>& siirrot = {});
    Siirto annaVastustajanSiirto();
    int kysyVastustajanVari();
    
    const Asema& getAsema() const;
    Asema& getAsema();
private:
	Asema _asema;
};
