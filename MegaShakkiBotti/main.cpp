#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <string>
#include <clocale>
#include <limits>
#include <vector>

#include "kayttoliittyma.h"
#include "siirto.h"
#include "asema.h"
#include "ajastin.h"

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
    auto& kayttoliittyma = Kayttoliittyma::getInstance();
    
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "nocolor") == 0)
        {
            Kayttoliittyma::getInstance().setVaritaRuudut(false);
        }
    }
    
    while(kayttoliittyma.getOhjelmaKaynnissa())
    {
        Peli peli = kayttoliittyma.kysyPeli();
        cout << endl;
        kayttoliittyma.kysyPelimuoto(peli);
        cout << endl;
        cout << "[S|R|L|T|D|K]            = nappula\n";
        cout << "[a, h][1, 8]             = ruutu (sarake, rivi)\n";
        cout << "[ruutu]-[ruutu]          = siirto\n";
        cout << "[ruutu]-[ruutu][nappula] = korotus\n";
        cout << "O-O                      = lyhyt linnoitus siirto\n";
        cout << "O-O-O                    = pitk\xc3\xa4 linnoitus siirto\n";
        cout << endl;
        
        while (kayttoliittyma.getOhjelmaKaynnissa())
        {
            kayttoliittyma.piirra(peli);
            cout << endl;
            
            Siirto siirto;
            bool ok = false;
            
            while(!ok)
            {
                Pelaaja& pelaaja = peli.asema.getSiirtovuoro() == 0 ? peli.valkoinen : peli.musta;
                if (pelaaja.onkoKone)
                {
                    MinMaxPaluu minimax;
                    int syvyys = pelaaja.syvyys;
                    
                    {
                        Ajastin ajastin("alphabetaMinimaxAsync");
                        minimax = peli.asema.alphabetaMinimaxAsync(syvyys);
                    }
                    
//                    cout << minimax << endl;
//                    
//                    {
//                        Ajastin ajastin("alphabetaMinimax");
//                        minimax = peli.asema.alphabetaMinimax(syvyys);
//                    }
//                    
//                    cout << minimax << endl;
//                    
//                    {
//                        Ajastin ajastin("minimaxAsync");
//                        minimax = peli.asema.minimaxAsync(syvyys);
//                    }
//                    
//                    cout << minimax << endl;
//                    
//                    {
//                        Ajastin ajastin("minimax");
//                        minimax = peli.asema.minimax(syvyys);
//                    }
//                    
//                    cout << minimax << endl;
                    
                    siirto = minimax.parasSiirto;
                    
                    cout << "Koneen siirto: ";
                    
                    if (!siirto.onkoLyhytLinna() && !siirto.onkoPitkaLinna())
                    {
                        int y = siirto.getAlkuruutu().getRivi();
                        int x = siirto.getAlkuruutu().getSarake();
                        char kirjain = toupper(peli.asema.lauta[y][x]->getKirjainSuomi());
                        cout << kirjain;
                    }
                    
                    cout << siirto << endl;
                }
                else
                {
                    siirto = kayttoliittyma.kysyVastustajanSiirto(peli);
                }
                
                ok = peli.asema.tarkistaSiirto(siirto);
            }
            
            if (peli.asema.getSiirtovuoro() == 1)
            {
                peli.siirtoparilaskuri++;
            }
            
            peli.asema.paivitaAsema(siirto);
            peli.viimeisinSiirto = siirto;
            cout << endl;
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
