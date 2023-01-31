#include <list>
#include <string>
#include "asema.h"
#include "nappula.h"

#include <iostream>

using namespace std;

namespace {
	void tulostaSiirrot(std::list<Siirto>& lista) {

		bool ruudut[8][8] = { false };

		for (auto& siirto : lista)
		{
			int x = siirto.getLoppuruutu().getSarake();
			int y = siirto.getLoppuruutu().getRivi();
			ruudut[y][x] = true;
		}

		for (int y = 7; y >= 0; y--)
		{
			for (int x = 0; x < 8; x++)
			{
				if (ruudut[y][x]) {
					std::cout << "x";
				}
				else {
					std::cout << "-";
				}
			}
			std::cout << "\n";
		}

		std::cout << "\n";
	}
}

Nappula::Nappula(string unicode, int vari, int koodi)
{
	_unicode = unicode;
	_vari = vari;
	_koodi = koodi;
}

std::string Nappula::getNimi()
{
	return _unicode;
}

void Torni::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int x = ruutu->getSarake();
	int y = ruutu->getRivi();

	// Oikea
	for (int i = x + 1; i < 8; i++)
	{
		if (asema->lauta[y][i] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(i, y)));
		}
		else {

			if (asema->lauta[y][i]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(i, y)));
			}

			break;
		}
	}

	// Vasen
	for (int i = x - 1; i >= 0; i--)
	{
		if (asema->lauta[y][i] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(i, y)));
		}
		else {

			if (asema->lauta[y][i]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(i, y)));
			}

			break;
		}
	}

	// Ylös
	for (int i = y + 1; i < 8; i++)
	{
		if (asema->lauta[i][x] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(x, i)));
		}
		else {

			if (asema->lauta[i][x]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(x, i)));
			}

			break;
		}
	}

	// Alas
	for (int i = y - 1; i >= 0; i--)
	{
		if (asema->lauta[i][x] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(x, i)));
		}
		else {

			if (asema->lauta[i][x]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(x, i)));
			}

			break;
		}
	}

	if (lista.size() > 0) {
		tulostaSiirrot(lista);
	}
}

void Ratsu::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	//int x = ruutu->getSarake();
	//int y = ruutu->getRivi();

	////suhteellinen sijainti mista ruudusta tahansa
	//int x_sarake[] = { -1, 1, 1, -1, 2, -2, 2, -2 };
	//int y_rivi[] = { 2, 2, -2, -2, 1, 1, -1, -1 };

	//for (int i = 0; i < 8; i++)
	//{
	//	//valkea ratsu
	//	int xv = x + x_sarake[i];
	//	int yv = y + y_rivi[i];

	//	//musta ratsu
	//	int xm = x - x_sarake[i];
	//	int ym = y - y_rivi[i];

	//	//onko valkean ratsun siirto laudan ulkopuolella
	//	if ((xv >= 0 && xv < 8) && (yv >= 0 && yv < 8))
	//	{
	//		//tarkistetaan, voiko ruutuun liikkua
	//		if (asema->lauta[yv][xv] == NULL)
	//		{
	//			lista.push_back(Siirto(*ruutu, Ruutu(xv, yv)));
	//		}

	//		else if (asema->lauta[yv][xv] != NULL && asema->lauta[yv][xv]->getVari() != vari)
	//		{
	//			lista.push_back(Siirto(*ruutu, Ruutu(xv, yv)));
	//		}
	//	}

	//	//onko mustan ratsun siirto laudan ulkopuolella
	//	if ((xm >= 0 && xm < 8) && (ym >= 0 && ym < 8))
	//	{
	//		//tarkistetaan, voiko ruutuun liikkua
	//		if (asema->lauta[ym][xm] == NULL)
	//		{
	//			lista.push_back(Siirto(*ruutu, Ruutu(xm, ym)));
	//		}

	//		else if (asema->lauta[ym][xm] != NULL && asema->lauta[ym][xm]->getVari() != vari)
	//		{
	//			lista.push_back(Siirto(*ruutu, Ruutu(xm, ym)));
	//		}
	//	}

	//}

}

void Lahetti::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int x = ruutu->getSarake();
	int y = ruutu->getRivi();

	// "korkeus", eli lauta pituussuunnassa / y-akseli. 
	int korkeus = 1;
	

	// Oikea-Ylä: 
	for (int i = x + 1; i < 8; i++) 
	{
		
		if (asema->lauta[y + korkeus][i] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(i, y + korkeus)));
		}
		else {

			
			if ((i >= 0 && i < 8) && (y+korkeus >= 0 && y+korkeus < 8) && asema->lauta[y + korkeus][i]->getVari() != vari ) {
				lista.push_back(Siirto(*ruutu, Ruutu(i, y + korkeus)));
			}

			break;
		}
		korkeus++;
		
	}

	korkeus = 1;

	// Vasen- Ylä: 
	for (int i = x - 1; i >= 0; i--) {

		if (asema->lauta[y + korkeus][i] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(i, y + korkeus)));
		}
		else {

			if ((i >= 0 && i < 8) && (y + korkeus >= 0 && y + korkeus < 8) &&  asema->lauta[y + korkeus][i]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(i, y + korkeus)));
			}

			break;
		}
		korkeus++;
	} 
	
	korkeus = 1;

	// Oikea -Alas: 
	for (int i = x + 1; x < 8; i++) {

		if (asema->lauta[y - korkeus][i] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(i, y - korkeus)));
		}
		else {
			if ((i >= 0 && i < 8) && (y - korkeus >= 0 && y - korkeus < 8) &&  asema->lauta[y - korkeus][i]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(i, y - korkeus)));
			}

			break;
		}
		korkeus++;
	}

	korkeus = 1;

	// Vasen -Alas: 
	for (int i = x - 1; i >= 0; i--) {

		if (asema->lauta[y - korkeus][i] == NULL) {
			lista.push_back(Siirto(*ruutu, Ruutu(i, y - korkeus)));
		}
		else {
			if ((i >= 0 && i < 8) && (y - korkeus >= 0 && y - korkeus < 8) &&  asema->lauta[y - korkeus][i]->getVari() != vari) {
				lista.push_back(Siirto(*ruutu, Ruutu(i, y - korkeus)));
			}

			break;
		}
		korkeus++;
	}


}

void Daami::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{

}

void Kuningas::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	/*perusidea on että kaikki viereiset ruudut ovat sallittuja. kuten tornilla ja lähetillä,
	oman nappulan päälle ei voi mennä ja vastustajan nappulan voi syödä.

	Kaikki muu kuninkaaseen liittyvä tarkistus tehdään eri paikassa*/
	int x = ruutu->getSarake();
	int y = ruutu->getRivi();

	// Suhteellinen sijainti mista ruudusta tahansa.
	int x_sarake[] = { -1, 0, 1, 1, 1, 0, -1, -1};
	int y_rivi[] = { 1, 1, 1, 0, -1, -1, -1, 0};

	//// Valkea kunigas
	//int xv = 0;
	//int yv = 0;

	//// Musta kuningas
	//int xm = 0;
	//int ym = 0;

	for (int i = 0; i < 8; i++)
	{
		// Valkea kuningas
		int xv = x + x_sarake[i];
		int yv = y + y_rivi[i];

		// Musta kuningas		
		int xm = x - x_sarake[i];
		int ym = y - y_rivi[i];	

		// Onko valkean kuninkaan siirto laudan ulkopuolella
		if ((xv >= 0 && xv < 8) && (yv >= 0 && yv < 8))
		{
			// Tarkistetaan, voiko ruutuun liikkua
			if (asema->lauta[yv][xv] == NULL)
			{
				lista.push_back(Siirto(*ruutu, Ruutu(xv, yv)));
			}
			else if (asema->lauta[yv][xv] != NULL && asema->lauta[yv][xv]->getVari() != vari)
			{
				lista.push_back(Siirto(*ruutu, Ruutu(xv, yv)));
			}
		}

		// Onko mustan kuninkaan siirto laudan ulkopuolella
		if ((xm >= 0 && xm < 8) && (ym >= 0 && ym < 8))
		{
			// Tarkistetaan, voiko ruutuun liikkua
			if (asema->lauta[ym][xm] == NULL)
			{
				lista.push_back(Siirto(*ruutu, Ruutu(xm, ym)));
			}
			else if (asema->lauta[ym][xm] != NULL && asema->lauta[ym][xm]->getVari() != vari)
			{
				lista.push_back(Siirto(*ruutu, Ruutu(xm, ym)));
			}
		}
	}

	// Näytetään mahdolliset siirrot
	//if (lista.size() > 0)
	//{
	//	tulostaSiirrot(lista);
	//}
}

void Sotilas::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{

}

void Sotilas::lisaaSotilaanKorotukset(Siirto* siirto, std::list<Siirto>& lista, Asema* asema) {

}
