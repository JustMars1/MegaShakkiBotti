#pragma once

#include <list>
#include <string>
#include "asema.h"
#include "siirto.h"


// Vakioarvot nappulatyypeille.
enum
{
	VT,VR,VL,VD,VK,VS,
	MT,MR,ML,MD,MK,MS
};


// Yliluokka shakkinappuloille.
class Nappula
{

private:
	std::string	    _unicode;	// nappulaa vastaava unicode-merkki
	int				_vari;		// valkea = 0, musta = 1
	int				_koodi;		// VT, VR, MT tms.

public:
	Nappula(std::string, int, int);

	// Siirtojen generointi. Puhdas virtuaalifunktio, eli aliluokat toteuttavat t‰m‰n
	// omalla tavallaan.
	virtual void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) = 0;

	void setKoodi(int koodi)				{ _koodi = koodi; }
	int getKoodi()							{ return _koodi; }
	void setUnicode(std::string unicode)	{ _unicode = unicode;			}
	std::string getNimi();
	void setVari(int vari)					{ _vari = vari;					}
	int getVari()							{ return _vari;					}
};

// Torni-aliluokka. Virtuaalinen perint‰ tarkoittaa, ett‰ kantaluokka perit‰‰n moniperinn‰ss‰ vain kerran
// (koska daami perii sek‰ tornin ett‰ l‰hetin).
class Torni : public virtual Nappula {
public:
	Torni(std::string unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
	void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

// Ratsu-aliluokka.
class Ratsu : public Nappula {
public:
	Ratsu(std::string unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
	void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

// L‰hetti-aliluokka. Virtuaalinen perint‰ tarkoittaa, ett‰ kantaluokka perit‰‰n moniperinn‰ss‰ vain kerran
// (koska daami perii sek‰ tornin ett‰ l‰hetin).
class Lahetti : public virtual Nappula {
public:
	Lahetti(std::string unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
	void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

// Daami-aliluokka. Perii sek‰ l‰hetin ett‰ tornin.
class Daami : public Lahetti, public Torni {
public:
	Daami(std::string unicode, int vari, int koodi) :
		Nappula(unicode, vari, koodi), Lahetti(unicode, vari, koodi), Torni(unicode, vari, koodi) {}
	void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

// Kuningas-aliluokka.
class Kuningas : public Nappula {
public:
	Kuningas(std::string unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
	void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

// Sotilas-aliluokka.
class Sotilas : public Nappula {
public:
	Sotilas(std::string unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
	void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
private:
	void lisaaSotilaanKorotukset(Siirto*, std::list<Siirto>& lista, Asema*);
};


