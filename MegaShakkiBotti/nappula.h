#pragma once

#include <list>
#include <string>
#include "siirto.h"

class Asema;

// Vakioarvot nappulatyypeille.
enum NappulaKoodi
{
    VT,VR,VL,VD,VK,VS,
    MT,MR,ML,MD,MK,MS
};

// Yliluokka shakkinappuloille.
class Nappula
{
public:
    Nappula(std::string merkki, int vari, NappulaKoodi koodi);
    
    // Siirtojen generointi. Puhdas virtuaalifunktio, eli aliluokat toteuttavat t‰m‰n
    // omalla tavallaan.
    virtual void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) = 0;
    
    NappulaKoodi getKoodi() const;
    void setKoodi(NappulaKoodi NappulaKoodi);
    
    int getVari() const;
    void setVari(int vari);
    
    const std::string& getMerkki() const;
    void setMerkki(const std::string& nimi);
protected:
    std::string        _merkki; // nappulaa vastaava unicode-merkki
    int                _vari;   // valkea = 0, musta = 1
    NappulaKoodi       _koodi;  // VT, VR, MT tms.
};

// Torni-aliluokka. Virtuaalinen perint‰ tarkoittaa, ett‰ kantaluokka perit‰‰n moniperinn‰ss‰ vain kerran
// (koska daami perii sek‰ tornin ett‰ l‰hetin).
class Torni : public virtual Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// L‰hetti-aliluokka. Virtuaalinen perint‰ tarkoittaa, ett‰ kantaluokka perit‰‰n moniperinn‰ss‰ vain kerran
// (koska daami perii sek‰ tornin ett‰ l‰hetin).
class Lahetti : public virtual Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Daami-aliluokka. Perii sek‰ l‰hetin ett‰ tornin.
class Daami : public Lahetti, public Torni
{
public:
    Daami(std::string merkki, int vari, NappulaKoodi koodi);
    void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Kuningas-aliluokka.
class Kuningas : public Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Ratsu-aliluokka.
class Ratsu : public Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Sotilas-aliluokka.
class Sotilas : public Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
private:
    void lisaaSotilaanKorotukset(const Siirto& siirto, std::list<Siirto>& lista, const Asema& asema);
};
