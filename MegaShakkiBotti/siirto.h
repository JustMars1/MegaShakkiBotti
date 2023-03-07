#pragma once
#include <iostream>
#include <optional>
#include "ruutu.h"

// Ns. "forward declaration". Nyt Asema-luokassa voidaa esitellä Nappula-osoittimia ilman,
// että nappula.h -tiedostoa täytyy includoida.
class Nappula;

// Siirto kuvaa nappulan siirtymisen ruudusta toiseen, mukaanlukien erikoissiirrot
// (linnoitus ja ohestalyönti).
class Siirto
{
public:
    static std::optional<Siirto> lue(std::string merkinta, int vari);
    
    Siirto(Ruutu alku, Ruutu loppu);
    Siirto(bool lyhytLinna, bool pitkaLinna); // Linnoitus lyhesti (K-siipi) tai pitkästi (D-siipi)?
    Siirto();
    
    bool operator==(const Siirto& rhs) const;
    
    const Ruutu& getAlkuruutu() const;
    const Ruutu& getLoppuruutu() const;
    bool onkoLyhytLinna() const;
    bool onkoPitkaLinna() const;
    
    Nappula* getNappula(const Asema& alkuAsema) const;
    
    std::string getMerkinta(const Asema& asema) const;
    
    Nappula* miksiKorotetaan;
private:
    Ruutu _alkuRuutu;
    Ruutu _loppuRuutu;
    bool _lyhytLinna;
    bool _pitkaLinna;
};

std::ostream& operator<<(std::ostream& os, const Siirto& siirto);
