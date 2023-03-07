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
    VT, VR, VL, VD, VK, VS,
    MT, MR, ML, MD, MK, MS,
    NappulaKoodiLkm
};

// Yliluokka shakkinappuloille.
class Nappula
{
public:
    Nappula(const std::string& merkki, int vari, NappulaKoodi koodi, float arvo, int maxSiirrot, const std::string& siirtoMerkkiAvain, const std::string& fenMerkki);
    
    virtual void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) = 0;
    
    NappulaKoodi getKoodi() const;
    int getVari() const;
    float getArvo() const;
    
    const std::string& getLautaMerkki() const;
    const std::string& getSiirtoMerkki() const;
    const std::string& getFENMerkki() const;
    
    int getMaxSiirrot() const;
protected:
    const std::string        _merkki;
    const int                _vari;   // valkea = 0, musta = 1
    const NappulaKoodi       _koodi;
    const float              _arvo;
    const int                _maxSiirrot;
    const std::string        _siirtoMerkkiAvain;
    const std::string        _fenMerkki;
};

// Torni-aliluokka. Virtuaalinen perint‰ tarkoittaa, ett‰ kantaluokka perit‰‰n moniperinn‰ss‰ vain kerran
// (koska daami perii sek‰ tornin ett‰ l‰hetin).
class Torni : public virtual Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// L‰hetti-aliluokka. Virtuaalinen perint‰ tarkoittaa, ett‰ kantaluokka perit‰‰n moniperinn‰ss‰ vain kerran
// (koska daami perii sek‰ tornin ett‰ l‰hetin).
class Lahetti : public virtual Nappula
{
public:
    using Nappula::Nappula;
    void annaSiirrot(std::vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari) override;
};

// Daami-aliluokka. Perii sek‰ l‰hetin ett‰ tornin.
class Daami : public Lahetti, public Torni
{
public:
    Daami(const std::string& merkki, int vari, NappulaKoodi koodi, float arvo, int maxSiirrot, const std::string& siirtoMerkkiAvain, const std::string&);
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
