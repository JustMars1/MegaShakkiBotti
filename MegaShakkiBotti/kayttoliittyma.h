#pragma once
#include <optional>
#include "peli.h"

// Shakkiohjelman k�ytt�liittym�, joka osaa visualisoida nykyisen aseman
// ja lukea k�ytt�j�n sy�tt�m�t siirrot. Singleton.

class Kayttoliittyma
{
public:
    static Kayttoliittyma& getInstance();
    
    Peli kysyPeli() const;
    void kysyPelimuoto(Peli& peli) const;
    std::optional<Peli> kysyFEN() const;
    
    Siirto kysyVastustajanSiirto(const Peli& peli);
    
    void piirra(const Peli& peli) const;
    bool tarkistaKomento(std::string komento, const Peli& peli) const;
    
    void tulostaVirhe(std::string virhe) const;
    
    bool getVaritaRuudut() const;
    void setVaritaRuudut(bool varita);
    
    bool getOhjelmaKaynnissa() const;
    void suljeOhjelma();
private:
    bool _varitaRuudut;
    bool _ohjelmaKaynnissa;
    
    Kayttoliittyma();
};
