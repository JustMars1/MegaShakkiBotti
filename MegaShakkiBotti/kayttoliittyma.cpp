#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <limits>
#include "kayttoliittyma.h"

using namespace std;

Kayttoliittyma* Kayttoliittyma::instance = 0;

Kayttoliittyma* Kayttoliittyma::getInstance()
{
	if (instance == 0)
		instance = new Kayttoliittyma();
	return instance;
}


void Kayttoliittyma::piirraLauta()
{
	for (int y = 7; y >= 0; y--)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED |
			BACKGROUND_GREEN | BACKGROUND_BLUE);
		std::wcout << to_wstring(y + 1);

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
			std::wcout << testi;
		}
		std::wcout << "\n";
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED |
		BACKGROUND_GREEN | BACKGROUND_BLUE);
	std::wcout << " abcdefgh\n";

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
} 


/*
	Aliohjelma tarkistaa että käyttäjän antama syöte siirroksi on 
	muodollisesti korrekti (ei tarkista aseman laillisuutta)
	Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
*/
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
	auto tarkistaRuutu = [](int sarake, int rivi) -> bool
	{
		if (sarake > 7 || sarake < 0)
		{
			return false;
		}

		if (rivi > 7 || rivi < 0)
		{
			return false;
		}

		return true;
	};

	auto tarkistaNappula = [this](char nappulaChar, int sarake, int rivi) -> bool
	{
		Nappula* nappula = _asema->lauta[rivi][sarake];

		if (Asema::charToMustaNappula.find(nappulaChar) != Asema::charToMustaNappula.end())
		{
			return true;
		}
		else if (Asema::charToValkoinenNappula.find(nappulaChar) != Asema::charToValkoinenNappula.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	char nappulaChar = 'i';

	int alkuSarake = -1;
	int alkuRivi = -1;
	int loppuSarake = -1;
	int loppuRivi = -1;

	string syote;
	bool pitkaLinna = false, lyhytLinna = false;

	do
	{
		std::wcout << "Syötä siirto muodossa: Nappula, alkuruutu ja loppuruutu.\n";
		std::wcout << "Esim. Rg1-f3. Nappulan kirjain isolla, loput pienellä.\n";

		std::cin >> syote;

		pitkaLinna = syote == "O-O-O";
		lyhytLinna = syote == "O-O";
		
		if (pitkaLinna || lyhytLinna) 
		{
			break;
		}

		if (syote.length() == 6)
		{
			nappulaChar = syote[0];

			alkuSarake = syote[1] - 'a';
			alkuRivi = syote[2] - '0' - 1;
			loppuSarake = syote[4] - 'a';
			loppuRivi = syote[5] - '0' - 1;
		}
		

	} while (!tarkistaRuutu(alkuSarake, alkuRivi) 
		|| !tarkistaRuutu(loppuSarake, loppuRivi) 
		|| !tarkistaNappula(nappulaChar, alkuSarake, alkuRivi) );

	std::wcout << "outstanding move\n";

	if (pitkaLinna || lyhytLinna) 
	{
		return Siirto(lyhytLinna, pitkaLinna);
	}
	else 
	{
		Ruutu alku(alkuSarake, alkuRivi);
		Ruutu loppu(loppuSarake, loppuRivi);

		return Siirto(alku, loppu);
	} 
}


int Kayttoliittyma::kysyVastustajanVari()
{
	return 0;
}
