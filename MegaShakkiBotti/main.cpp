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

void tulostaSiirrot(list<Siirto>& lista) {
    bool ruudut[8][8] = { false };
    
    for (auto& siirto : lista)
    {
        int x = siirto.getLoppuruutu().getSarake();
        int y = siirto.getLoppuruutu().getRivi();
        ruudut[y][x] = true;
    }
    
    for (int y = 7; y >= 0; y--)
    {
        cout << to_string(y + 1) << " ";
        for (int x = 0; x < 8; x++)
        {
            if (ruudut[y][x]) {
                cout << "x";
            }
            else {
                cout << "-";
            }
        }
        cout << "\n";
    }
    
    cout << "  abcdefgh\n";
}

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
    cout.imbue(std::locale());
	Asema asema; 
	Kayttoliittyma::getInstance().aseta_asema(&asema);
	Kayttoliittyma::getInstance().piirraLauta();

	list<Siirto> siirrot;
	
	while (true) {
		Siirto testiSiirto = Kayttoliittyma::getInstance().annaVastustajanSiirto();
		asema.paivitaAsema(&testiSiirto);
		asema.annaLaillisetSiirrot(siirrot);
		tulostaSiirrot(siirrot);
        Kayttoliittyma::getInstance().piirraLauta();
		siirrot.clear();
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
