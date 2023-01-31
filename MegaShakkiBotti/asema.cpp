#include <iostream>
#include "asema.h"
#include "minmaxpaluu.h"
#include "nappula.h"
#include "ruutu.h"

Nappula* Asema::vk = new Kuningas("\xe2\x99\x94", 0, VK);
Nappula* Asema::vd = new Daami("\xe2\x99\x95", 0, VD);
Nappula* Asema::vt = new Torni("\xe2\x99\x96", 0, VT);
Nappula* Asema::vl = new Lahetti("\xe2\x99\x97", 0, VL);
Nappula* Asema::vr = new Ratsu("\xe2\x99\x98", 0, VR);
Nappula* Asema::vs = new Sotilas("\xe2\x99\x99", 0, VS);

Nappula* Asema::mk = new Kuningas("\xe2\x99\x9a", 1, MK);
Nappula* Asema::md = new Daami("\xe2\x99\x9b", 1, MD);
Nappula* Asema::mt = new Torni("\xe2\x99\x9c", 1, MT);
Nappula* Asema::ml = new Lahetti("\xe2\x99\x9d", 1, ML);
Nappula* Asema::mr = new Ratsu("\xe2\x99\x9e", 1, MR);
Nappula* Asema::ms = new Sotilas("\xe2\x99\x9f", 1, MS);

std::map<char, Nappula*> Asema::charToValkoinenNappula = {
	{'K', Asema::vk},
	{'D', Asema::vd},
	{'T', Asema::vt},
	{'L', Asema::vl},
	{'R', Asema::vr},
	{'S', Asema::vs}
};

std::map<char, Nappula*> Asema::charToMustaNappula = {
	{'K', Asema::mk},
	{'D', Asema::md},
	{'T', Asema::mt},
	{'L', Asema::ml},
	{'R', Asema::mr},
	{'S', Asema::ms}
};

Asema::Asema() : lauta
{
	// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
	// Asetetaan alkuaseman mukaisesti nappulat ruuduille
		vt, vr, vl, vd, vk, vl, vr, vt,
		vs, vs, vs, vs, vs, vs, vs, vs,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		ms, ms, ms, ms, ms, ms, ms, ms,
		mt, mr, ml, md, mk, ml, mr, mt
}
{
	_siirtovuoro = 0;
	_onkoValkeaKuningasLiikkunut = false;
	_onkoMustaKuningasLiikkunut = false;	
	_onkoValkeaDTliikkunut = false;		
	_onkoValkeaKTliikkunut = false;		
	_onkoMustaDTliikkunut = false;			
	_onkoMustaKTliikkunut = false;
}

void Asema::paivitaAsema(Siirto* siirto)
{
	// Kaksoisaskel-lippu on oletusarvoisesti pois päältä.
	// Asetetaan myöhemmin, jos tarvii.
	//Tarkastetaan on siirto lyhyt linna tai pitkä linna

	if (siirto->onkoLyhytLinna() || siirto->onkoPitkalinna() ) {
		if (_siirtovuoro == 0)
		{
			// vuoro on valkoisen 
			if (siirto->onkoLyhytLinna())
			{
				_onkoValkeaKTliikkunut = true;
				_onkoValkeaKuningasLiikkunut = true;

				lauta[7][5] = Asema::vt;
				lauta[7][6] = Asema::vk;
				lauta[7][4] = NULL;
				lauta[7][7] = NULL;
				

			}
			else if (siirto->onkoPitkalinna())
			{
				_onkoValkeaDTliikkunut = true;
				_onkoValkeaKuningasLiikkunut = true;

				lauta[7][3] = Asema::vt;
				lauta[7][2] = Asema::vk;
				lauta[7][0] = NULL;
				lauta[7][4] = NULL;
			}
		}
		else {

			// on mustan vuoro
			if (siirto->onkoLyhytLinna())
			{
				_onkoMustaKTliikkunut = true;
				_onkoMustaKuningasLiikkunut = true;

				lauta[0][5] = Asema::mt;
				lauta[0][6] = Asema::mk;
				lauta[0][7] = NULL;
				lauta[0][4] = NULL;
			}
			else if (siirto->onkoPitkalinna())
			{
				_onkoMustaDTliikkunut = true;
				_onkoMustaKuningasLiikkunut = true;

				lauta[0][3] = Asema::mt;
				lauta[0][2] = Asema::mk;
				lauta[0][0] = NULL;
				lauta[0][4] = NULL;
			}
		}
	}
	else 
	{
		int alkuX = siirto->getAlkuruutu().getSarake();
		int alkuY = siirto->getAlkuruutu().getRivi();

		int loppuX = siirto->getLoppuruutu().getSarake();
		int loppuY = siirto->getLoppuruutu().getRivi();

		Nappula* nappulaPtr = lauta[alkuY][alkuX];

		if (nappulaPtr == Asema::vk)
		{
			_onkoValkeaKuningasLiikkunut = true;
		}
		else if (nappulaPtr == Asema::mk)
		{
			_onkoMustaKuningasLiikkunut = true;
		}
		else if (nappulaPtr == Asema::vt)
		{
			if (alkuX == 0 && !_onkoValkeaDTliikkunut)
			{
				_onkoValkeaDTliikkunut = true;
			}
			else if (alkuX == 7 && !_onkoValkeaKTliikkunut)
			{
				_onkoValkeaKTliikkunut = true;
			}
		}
		else if (nappulaPtr == Asema::mt)
		{
			if (alkuX == 0 && !_onkoMustaKTliikkunut)
			{
				_onkoMustaKTliikkunut = true;
			}
			else if (alkuX == 7 && !_onkoMustaDTliikkunut)
			{
				_onkoMustaDTliikkunut = true;
			}
		}
		

		lauta[loppuY][loppuX] = nappulaPtr;
		lauta[alkuY][alkuX] = NULL;
	}
	
	// Kaikki muut siirrot


		//Ottaa siirron alkuruudussa olleen nappulan talteen 


		//Laittaa talteen otetun nappulan uuteen ruutuun


		// Tarkistetaan oliko sotilaan kaksoisaskel
		// (asetetaan kaksoisaskel-lippu)

		// Ohestalyönti on tyhjään ruutuun. Vieressä oleva (sotilas) poistetaan.

		//// Katsotaan jos nappula on sotilas ja rivi on päätyrivi niin ei vaihdeta nappulaa 
		////eli alkuruutuun laitetaan null ja loppuruudussa on jo kliittymän laittama nappula MIIKKA, ei taida minmaxin kanssa hehkua?

		//
		////muissa tapauksissa alkuruutuun null ja loppuruutuun sama alkuruudusta lähtenyt nappula

		// katsotaan jos liikkunut nappula on kuningas niin muutetaan onkoKuningasLiikkunut arvo (molemmille väreille)

		// katsotaan jos liikkunut nappula on torni niin muutetaan onkoTorniLiikkunut arvo (molemmille väreille ja molemmille torneille)

	//päivitetään _siirtovuoro
	
	_siirtovuoro = _siirtovuoro == 1 ? 0 : 1;

}

int Asema::getSiirtovuoro()
{
	return _siirtovuoro;
}

void Asema::setSiirtovuoro(int vuoro)
{
	_siirtovuoro = vuoro;
}

bool Asema::getOnkoValkeaKuningasLiikkunut()
{
	return _onkoValkeaKuningasLiikkunut;
}

bool Asema::getOnkoMustaKuningasLiikkunut()
{
	return _onkoMustaKuningasLiikkunut;
}

bool Asema::getOnkoValkeaDTliikkunut()
{
	return _onkoValkeaDTliikkunut;
}

bool Asema::getOnkoValkeaKTliikkunut()
{
	return _onkoValkeaKTliikkunut;
}

bool Asema::getOnkoMustaDTliikkunut()
{
	return _onkoMustaDTliikkunut;
}

bool Asema::getOnkoMustaKTliikkunut()
{
	return _onkoMustaKTliikkunut;
}

/* 1. Laske nappuloiden arvo
Daami = 9
Torni = 5
Lähetti = 3,25
Ratsu = 3
Sotilas = 1

2. Kuninkaan hyvyys
Jos avaus tai keskipeli, niin hyvä että kunigas g1 tai b1/c1
Loppupelissä vaikea sanoa halutaanko olla auttamassa omaa sotilasta korottumaan
vai olla estämässä vastustajan korotusta siksi ei oteta kantaa
3. Arvosta keskustaa sotilailla ja ratsuilla
4. Arvosta pitkiä linjoja daami, torni ja lähetti
*/
double Asema::evaluoi()
{
	return 0;

	//kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta

	//1. Nappuloiden arvo

	//2. Kuningas turvassa

	//3. Arvosta keskustaa

	// 4. Arvosta linjoja

}

double Asema::laskeNappuloidenArvo(int vari)
{
	return 0;

}

bool Asema::onkoAvausTaiKeskipeli(int vari)
{
	return 0;
	// Jos upseereita 3 tai vähemmän on loppupeli
	// mutta jos daami laudalla on loppueli vasta kun kuin vain daami jäljellä

	//Jos vari on 0 eli valkoiset
	//niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1


}

double Asema::nappuloitaKeskella(int vari)
{
	return 0;

	//sotilaat ydinkeskustassa + 0.25/napa
	//ratsut ydinkeskustassa + 0.25/napa
	//sotilaat laitakeskustassa + 0.11/napa
	//ratsut laitakeskustassa + 0.11/napa

	//valkeille ydinkeskusta



	//valkeille laitakeskusta



	//mustille ydinkeskusta

	//mustille laitakeskusta

}

double Asema::linjat(int vari)
{
	return 0;

	//valkoiset

	//mustat

}

// https://chessprogramming.wikispaces.com/Minimax MinMax-algoritmin pseudokoodi (lisäsin parametrina aseman)
//int maxi(int depth, asema a) 
//	if (depth == 0) return evaluate();
//	int max = -oo;
//	for (all moves ) {
//		score = mini(depth - 1, seuraaja);
//		if (score > max)
//			max = score;
//	}
//	return max;
//}

//int mini(int depth, asema a) {
//	if (depth == 0) return -evaluate();
//	int min = +oo;
//	for (all moves) {
//		score = maxi(depth - 1);
//		if (score < min)
//			min = score;
//	}
//	return min;
//}
MinMaxPaluu Asema::minimax(int syvyys)
{
	MinMaxPaluu paluuarvo;

	// Generoidaan aseman lailliset siirrot.

	// Rekursion kantatapaus 1: peli on loppu

	// Rekursion kantatapaus 2: katkaisusyvyydessä

	// Rekursioaskel: kokeillaan jokaista laillista siirtoa s
	// (alustetaan paluuarvo huonoimmaksi mahdolliseksi).

	return paluuarvo;
}

MinMaxPaluu Asema::maxi(int syvyys)
{
	MinMaxPaluu paluu;
	return paluu;
}

MinMaxPaluu Asema::mini(int syvyys)
{
	MinMaxPaluu paluu;
	return paluu;
}

bool Asema::onkoRuutuUhattu(Ruutu* ruutu, int vastustajanVari)
{

	return false;
}

void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>& lista, int vari)
{
    
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista) {
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (lauta[y][x] != NULL) {
				Ruutu ruutu(x, y);
				lauta[y][x]->annaSiirrot(lista, &ruutu, this, lauta[y][x]->getVari());
			}
		}
	}
}
