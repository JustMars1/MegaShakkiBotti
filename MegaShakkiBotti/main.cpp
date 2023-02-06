#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <string>
#include <clocale>

#include "kayttoliittyma.h"
#include "siirto.h"
#include "asema.h"

using namespace std;

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif
    setlocale(LC_ALL, "fi_FI.UTF-8");
    cout.imbue(locale());
    
    auto& kayttoliittyma = Kayttoliittyma::getInstance();
    Asema& asema = kayttoliittyma.getAsema();
    
    list<Siirto> siirrot;
    asema.annaLaillisetSiirrot(siirrot);
    kayttoliittyma.piirraLauta(siirrot);
	std::cout << "Siirtovuoro: Valkoinen.\n";

	while (true)
    {
        siirrot.clear();
		Siirto testiSiirto = kayttoliittyma.annaVastustajanSiirto();
		asema.paivitaAsema(testiSiirto);
		asema.annaLaillisetSiirrot(siirrot);
        kayttoliittyma.piirraLauta(siirrot);

		// Näyttää kenen siirtovuoro.
		if (asema.getSiirtovuoro() == 1)
		{
			std::cout << "Siirtovuoro: Musta.\n";
		}
		else if (asema.getSiirtovuoro() == 0)
		{
			std::cout << "Siirtovuoro: Valkoinen. \n";
		}
	}
	
	//int lopetus = 100;
	// Open juttu alla
	//Peli peli(Kayttoliittyma::getInstance()->
	//	kysyVastustajanVari());
	//std::list<Siirto> lista;
	//system("cls");
	//int koneenVari = peli.getKoneenVari();

	//while (lopetus != 0) {
	//	lista.clear();
	//	Kayttoliittyma::getInstance()->piirraLauta();
	//	cout << "\n";
	//	// Tarkasta onko peli loppu?
	//	asema.annaLaillisetSiirrot(lista);
	//	if (lista.size() == 0) {
	//		lopetus = 0;
	//		std::cout << "Peli loppui";
	//		continue;
	//	}
	//	Siirto siirto;
	//	if (asema.getSiirtovuoro() == koneenVari) {
	//		MinMaxPaluu paluu;
	//		if (koneenVari == 0) {
	//			paluu = asema.maxi(3);
	//		}
	//		else {
	//			paluu = asema.mini(3);
	//		}
	//		siirto = paluu._parasSiirto;
	//	}
	//	else {
	//		siirto = Kayttoliittyma::getInstance()->
	//			annaVastustajanSiirto();
	//	}
	//	asema.paivitaAsema(&siirto);
	//}

	
	return 0;
}
