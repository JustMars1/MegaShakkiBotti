#pragma once
#include "asema.h"
#include "nappula.h"
#include "siirto.h"

// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.

class Kayttoliittyma
{
public:
    static Kayttoliittyma& getInstance();
    
    void piirraLauta(bool mustaAlhaalla, const std::vector<Siirto>& siirrot = {}) const;
    Siirto annaVastustajanSiirto() const;
    int kysyVastustajanVari() const;
    
    void lataaAsema();
    
    static void tulostaVirhe(std::string virhe);
    
    const Asema& getAsema() const;
    Asema& getAsema();
    
    bool getVaritaRuudut() const;
    void setVaritaRuudut(bool varita);
    
private:
    bool _varitaRuudut;
    Asema _asema;
    
    Kayttoliittyma();
};
