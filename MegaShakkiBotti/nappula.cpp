#include <list>
#include <string>
#include "asema.h"
#include "nappula.h"

#include <iostream>

using namespace std;

Nappula::Nappula(string unicode, int vari, int koodi)
{
	_unicode = unicode;
	_vari = vari;
	_koodi = koodi;
}

string Nappula::getNimi()
{
	return _unicode;
}

void Torni::annaSiirrot(list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
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
}

void Ratsu::annaSiirrot(list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari) {
	int x = ruutu->getSarake();
	int y = ruutu->getRivi();

	//suhteellinen sijainti mista ruudusta tahansa
	int x_sarake[] = { -1, 1, 1, -1, 2, -2, 2, -2 };
	int y_rivi[] = { 2, 2, -2, -2, 1, 1, -1, -1 };

	for (int i = 0; i < 8; i++)
	{
		//valkea ratsu
		int xv = x + x_sarake[i];
		int yv = y + y_rivi[i];

		//musta ratsu
		int xm = x - x_sarake[i];
		int ym = y - y_rivi[i];

		//onko valkean ratsun siirto laudan ulkopuolella
		if ((xv >= 0 && xv < 8) && (yv >= 0 && yv < 8))
		{
			//tarkistetaan, voiko ruutuun liikkua
			if (asema->lauta[yv][xv] == NULL)
			{
				lista.push_back(Siirto(*ruutu, Ruutu(xv, yv)));
			}

			else if (asema->lauta[yv][xv] != NULL && asema->lauta[yv][xv]->getVari() != vari)
			{
				lista.push_back(Siirto(*ruutu, Ruutu(xv, yv)));
			}
		}

		//onko mustan ratsun siirto laudan ulkopuolella
		if ((xm >= 0 && xm < 8) && (ym >= 0 && ym < 8))
		{
			//tarkistetaan, voiko ruutuun liikkua
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
}

void Lahetti::annaSiirrot(list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari) {
	int x0 = ruutu->getSarake();
	int y0 = ruutu->getRivi();

	// Oikea-Ylä: 
	for (int i = 1; i < 8; i++) {
        int y = y0 + i;
        int x = x0 + i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7) {
            break;
        }
        
        if (asema->lauta[y][x] == NULL) {
            lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
        }
        else {
            if (asema->lauta[y][x]->getVari() != vari ) {
                lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
            }
            
            break;
        }
	}

	// Vasen- Ylä: 
	for (int i = 1; i < 8; i++) {
        int y = y0 + i;
        int x = x0 - i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7) {
            break;
        }
        
        if (asema->lauta[y][x] == NULL) {
            lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
        }
        else {
            if (asema->lauta[y][x]->getVari() != vari ) {
                lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
            }
            
            break;
        }
	}

	// Oikea -Alas: 
    for (int i = 1; i < 8; i++) {
        int y = y0 - i;
        int x = x0 + i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7) {
            break;
        }
        
        if (asema->lauta[y][x] == NULL) {
            lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
        }
        else {
            if (asema->lauta[y][x]->getVari() != vari ) {
                lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
            }
            
            break;
        }
    }

	// Vasen -Alas: 
    for (int i = 1; i < 8; i++) {
        int y = y0 - i;
        int x = x0 - i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7) {
            break;
        }
        
        if (asema->lauta[y][x] == NULL) {
            lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
        }
        else {
            if (asema->lauta[y][x]->getVari() != vari ) {
                lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
            }
            
            break;
        }
    }
}

void Daami::annaSiirrot(list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari) {
    Torni::annaSiirrot(lista, ruutu, asema, vari);
    Lahetti::annaSiirrot(lista, ruutu, asema, vari);
}

void Kuningas::annaSiirrot(list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
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
}

void Sotilas::annaSiirrot(list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari) {
    int x0 = ruutu->getSarake();
    int y0 = ruutu->getRivi();
    
    int dir = vari == 0 ? 1 : -1;
    
    int maxLiike = 1;
    if (vari == 0 && y0 == 1) {
        maxLiike++;
    }
    else if (y0 == 6){
        maxLiike++;
    }
    
    for (int i = 1; i <= maxLiike; i++) {
        int y = y0 + dir * i;
        
        if (y < 0 || y > 7) {
            break;
        }
        
        if (asema->lauta[y][x0] != NULL) {
            break;
        }
        
        lista.push_back(Siirto(*ruutu, Ruutu(x0, y)));
    }
    
    {
        int y = y0 + dir;
        int x = x0 + 1;
        
        if (y >= 0 && y < 8 && x >= 0 && x < 8) {
            if (asema->lauta[y][x] != NULL && asema->lauta[y][x]->getVari() != vari) {
                lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
            }
        }
    }
    
    {
        int y = y0 + dir;
        int x = x0 - 1;
        
        if (y >= 0 && y < 8 && x >= 0 && x < 8) {
            if (asema->lauta[y][x] != NULL && asema->lauta[y][x]->getVari() != vari) {
                lista.push_back(Siirto(*ruutu, Ruutu(x, y)));
            }
        }
    }
}

void Sotilas::lisaaSotilaanKorotukset(Siirto* siirto, list<Siirto>& lista, Asema* asema) {

}
