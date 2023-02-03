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
	Siirto(Ruutu, Ruutu);
	Siirto(bool, bool); // Linnoitus lyhesti (K-siipi) tai pitkästi (D-siipi)?
	const Ruutu& getAlkuruutu() const;
	const Ruutu& getLoppuruutu() const;
	bool onkoLyhytLinna() const;
	bool onkoPitkalinna() const;
	Nappula* miksikorotetaan;
private:
	Ruutu _alkuRuutu;
	Ruutu _loppuRuutu;
	bool _lyhytLinna;
	bool _pitkaLinna;
};
