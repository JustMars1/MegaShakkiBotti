#pragma once
#include <optional>
#include "peli.h"
#include "kieli.h"
#include "ajastin.h"

// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.

class Kayttoliittyma
{
public:
    static Kayttoliittyma& getInstance();
    
    void kysyAsetukset();
    
    void kysyKieli();
    void kysySiirronMerkintatapa();
    
    Peli kysyPeli() const;
    void kysyPelimuoto(Peli& peli) const;
    std::optional<Peli> kysyFEN() const;
    
    Siirto kysySiirto(Peli& peli);
    
    void tulostaSiirtoOhje(bool uci, int sisennys = 0) const;
    
    void piirra(const Peli& peli) const;
    bool tarkistaKomento(const std::string& syote, Peli& peli);
    
    void tulostaVirhe(std::string virhe) const;
    
    bool getVaritaRuudut() const;
    void setVaritaRuudut(bool varita);
    
    const Kieli& getKieli() const;
    
    bool getOnkoUCI() const;
    void setOnkoUCI(bool uci);

    bool getAloitaUCI() const;
    void uciProtokolla(Kayttoliittyma kayttoliittyma) const;
    
    const std::string& teksti();
private:
    bool _varitaRuudut;
    bool _ohjelmaKaynnissa;
    int _kieli;
    
    bool _onkoUCI;
    
    std::vector<Kieli> _kielet;
    
    static std::vector<Kieli> lataaKielet();
    
    Kayttoliittyma();
};

const std::string& operator""_k(const char* avain, size_t);
