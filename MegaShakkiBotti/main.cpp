#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <string>
#include <clocale>
#include <limits>

#include "kayttoliittyma.h"
#include "siirto.h"
#include "asema.h"
#include "ajastin.hpp"

#include <chrono>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
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
    
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "nocolor") == 0)
        {
            Kayttoliittyma::getInstance().varitaRuudut = false;
        }
    }
    
    int koneenVari;
    cout << "Kummalla v\xc3\xa4rill\xc3\xa4 pelaat? (0 = valkoinen, 1 = musta)\n";
    
    while(true)
    {
        int pelaajanVari;
        cin >> pelaajanVari;
        
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        }
        else if (pelaajanVari == 0 || pelaajanVari == 1)
        {
            koneenVari = 1 - pelaajanVari;
            break;
        }
    }
    
    auto& kayttoliittyma = Kayttoliittyma::getInstance();
    auto& asema = kayttoliittyma.getAsema();
    list<Siirto> siirrot;
    
    while (true)
    {
        cout << "Siirtovuoro: ";
        if (asema.getSiirtovuoro() == 0)
        {
            cout << "Valkoinen\n";
        }
        else
        {
            cout << "Musta\n";
        }
        
        cout << "Laudan arvo: " << asema.evaluoi() << endl;
        asema.annaLaillisetSiirrot(siirrot);
        kayttoliittyma.piirraLauta(koneenVari == 0, siirrot);
        siirrot.clear();
        
        Siirto siirto;
        bool ok = false;
        
        while(!ok)
        {
            if (asema.getSiirtovuoro() == koneenVari)
            {
                Ajastin ajastin("MiniMax");
                MinMaxPaluu minimax = asema.minimax(3);
                
                siirto = minimax._parasSiirto;
                cout << "Koneen siirto: " << siirto << endl;
            }
            else
            {
                siirto = kayttoliittyma.annaVastustajanSiirto();
            }
            
            ok = asema.tarkistaSiirto(siirto);
        }
        
        asema.paivitaAsema(siirto);
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
