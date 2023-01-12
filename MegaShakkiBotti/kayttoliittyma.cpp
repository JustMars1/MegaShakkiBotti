#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "kayttoliittyma.h"

using namespace std;


Kayttoliittyma* Kayttoliittyma::instance = 0;

Kayttoliittyma::Kayttoliittyma()
{

}


Kayttoliittyma* Kayttoliittyma::getInstance()
{
	if (instance == 0)
		instance = new Kayttoliittyma();
	return instance;
}


void Kayttoliittyma::piirraLauta()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | 
			//	BACKGROUND_GREEN | BACKGROUND_BLUE);

			// Joka toinen rivi valkoinen
			if ((x + y) % 2 == 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | 0 |
					BACKGROUND_GREEN | BACKGROUND_BLUE);
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | 
					BACKGROUND_GREEN | BACKGROUND_BLUE);
			}

			std::wstring testi = L" ";

			// Jos NULL niin printataan tyhjä paikka.
			if (_asema->lauta[y][x] != NULL)
			{
				testi = _asema->lauta[y][x]->getNimi();
			}
			wcout << testi;
		}
		wcout << "\n";
	}
}


/*
	Aliohjelma tarkistaa että käyttäjän antama syöte siirroksi on 
	muodollisesti korrekti (ei tarkista aseman laillisuutta)
	Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
*/
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
	Siirto siirto;
	return siirto;
	
}


int Kayttoliittyma::kysyVastustajanVari()
{
	return 0;
}
