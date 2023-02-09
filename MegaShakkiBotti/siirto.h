#pragma once
#include "ruutu.h"

// Ns. "forward declaration". Nyt Asema-luokassa voidaa esitellä Nappula-osoittimia ilman,
// että nappula.h -tiedostoa täytyy includoida.
class Nappula; 


// Siirto kuvaa nappulan siirtymisen ruudusta toiseen, mukaanlukien erikoissiirrot
// (linnoitus ja ohestalyönti).
class Siirto
{
public:
	Siirto(Ruutu alku, Ruutu loppu);
	Siirto(bool lyhytLinna, bool pitkaLinna); // Linnoitus lyhesti (K-siipi) tai pitkästi (D-siipi)?
	Siirto();
	const Ruutu& getAlkuruutu() const;
	const Ruutu& getLoppuruutu() const;
	bool onkoLyhytLinna() const;
	bool onkoPitkaLinna() const;
	Nappula* miksikorotetaan;
private:
	Ruutu _alkuRuutu;
	Ruutu _loppuRuutu;
	bool _lyhytLinna;
	bool _pitkaLinna;
};
