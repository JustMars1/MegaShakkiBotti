#pragma once
#include <optional>
#include "peli.h"
#include "kieli.h"

// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.

class Kayttoliittyma
{
public:
    static Kayttoliittyma& getInstance();
    
    void kysyKieli();
    
    Peli kysyPeli() const;
    void kysyPelimuoto(Peli& peli) const;
    std::optional<Peli> kysyFEN() const;
    
    Siirto kysyVastustajanSiirto(const Peli& peli);
    
    void tulostaSiirtoOhje() const;
    
    void piirra(const Peli& peli) const;
    bool tarkistaKomento(std::string komento, const Peli& peli) const;
    
    void tulostaVirhe(std::string virhe) const;
    
    bool getVaritaRuudut() const;
    void setVaritaRuudut(bool varita);
    
    bool getOhjelmaKaynnissa() const;
    void suljeOhjelma();
    
    const Kieli& getKieli() const;
    
    const std::string& teksti();
private:
    bool _varitaRuudut;
    bool _ohjelmaKaynnissa;
    int _kieli;
    
    std::vector<Kieli> _kielet;
    
    static std::vector<Kieli> lataaKielet();
    
    Kayttoliittyma();
};

const std::string& operator""_k(const char* avain, size_t);
