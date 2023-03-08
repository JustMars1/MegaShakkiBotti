#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <cstring>

#include "kayttoliittyma.h"
#include "siirto.h"
#include "asema.h"
#include "ajastin.h"
#include "uci.h"

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
    
    UCI uci;
    uci.uciLoop();
    
    kayttoliittyma.kysyAsetukset();
    
    bool ohjelmaKaynnissa = true;
    while(ohjelmaKaynnissa)
    {
        Peli peli = kayttoliittyma.kysyPeli();
        cout << endl;
        kayttoliittyma.kysyPelimuoto(peli);
        cout << endl;
        kayttoliittyma.tulostaSiirtoOhje(kayttoliittyma.getOnkoUCI());
        cout << endl;
        
        bool peliKaynnissa = true;
        while (peliKaynnissa)
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
                    
                    cout << minimax << endl;
                    
                    {
                        Ajastin ajastin("alphabetaMinimax");
                        minimax = peli.asema.alphabetaMinimax(syvyys);
                    }
                    
                    cout << minimax << endl;
                    
                    {
                        Ajastin ajastin("minimaxAsync");
                        minimax = peli.asema.minimaxAsync(syvyys);
                    }
                    
                    cout << minimax << endl;
                    
                    {
                        Ajastin ajastin("minimax");
                        minimax = peli.asema.minimax(syvyys);
                    }
                    
                    cout << minimax << endl;
                    
                    siirto = minimax.parasSiirto;
                    cout << "koneenSiirto"_k << ": " << siirto.getMerkinta(peli.asema) << endl;
                }
                else
                {
                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else
                    {
                        cout << "syotaSiirto"_k << ": ";
                    }
                    
                    string syote;
                    cin >> syote;
                    
                    if (cin.fail())
                    {
                        continue;
                    }
                    
                    if (kayttoliittyma.tarkistaKomento(syote, peli))
                    {
                        continue;
                    }
                    
                    optional<Siirto> luettuSiirto = Siirto::lue(syote, peli.asema.getSiirtovuoro());
                    if (!luettuSiirto.has_value())
                    {
                        continue;
                    }
                    
                    siirto = luettuSiirto.value();
                }
                
                ok = peli.asema.tarkistaSiirto(siirto);
            }

            peli.asema.paivitaAsema(siirto);
            peli.siirtoHistoria.push_back(siirto);
            cout << endl;

            vector<Siirto> siirrot = peli.asema.annaLaillisetSiirrot();

            if (siirrot.empty())
            {
                kayttoliittyma.piirra(peli);
                
                if (peli.asema.onkoRuutuUhattu(peli.asema.getValkeanKuninkaanRuutu(), 1))
                {
                    // valkoinen voittaa
                    cout << "matti"_k << ". " << "valkoisenVoitto"_k;

                }
                else if (peli.asema.onkoRuutuUhattu(peli.asema.getMustanKuninkaanRuutu(), 0))
                {
                    // musta voittaa
                    cout << "matti"_k << ". " << "mustanVoitto"_k;
                }
                else
                {
                    // patti 
                    cout << "patti"_k;
                }
                peliKaynnissa = false;
                cout << endl << "FEN: " << peli << endl << endl;
            }
        }

    }
        
    return 0;
}
