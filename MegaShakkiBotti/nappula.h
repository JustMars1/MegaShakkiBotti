#pragma once

#include <list>
#include <vector>
#include <string>
#include <array>
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
    Nappula(std::string merkki, int vari, NappulaKoodi koodi, float arvo, int maxSiirrot, char kirjainSuomi, char kirjainEnglanti);
    
    virtual void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) = 0;
    
    NappulaKoodi getKoodi() const;
    int getVari() const;
    const std::string& getMerkki() const;
    float getArvo() const;
    
    char getKirjainSuomi() const;
    char getKirjainEnglanti() const;
    
    int getMaxSiirrot() const;
protected:
    const std::string        _merkki; // nappulaa vastaava unicode-merkki
    const int                _vari;   // valkea = 0, musta = 1
    const NappulaKoodi       _koodi;  // VT, VR, MT tms.
    const float              _arvo;
    const int                _maxSiirrot;
    const char               _kirjainSuomi;
    const char               _kirjainEnglanti;
};

// Torni-aliluokka. Virtuaalinen perint� tarkoittaa, ett� kantaluokka perit��n moniperinn�ss� vain kerran
// (koska daami perii sek� tornin ett� l�hetin).
class Torni : public virtual Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// L�hetti-aliluokka. Virtuaalinen perint� tarkoittaa, ett� kantaluokka perit��n moniperinn�ss� vain kerran
// (koska daami perii sek� tornin ett� l�hetin).
class Lahetti : public virtual Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Daami-aliluokka. Perii sek� l�hetin ett� tornin.
class Daami : public Lahetti, public Torni
{
public:
    Daami(std::string merkki, int vari, NappulaKoodi koodi, float arvo, int maxSiirrot, char kirjainSuomi, char kirjainEnglanti);
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Kuningas-aliluokka.
class Kuningas : public Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Ratsu-aliluokka.
class Ratsu : public Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Sotilas-aliluokka.
class Sotilas : public Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
private:
    void lisaaSotilaanKorotukset(const Siirto& siirto, std::vector<Siirto>& lista, const Asema& asema);
};
