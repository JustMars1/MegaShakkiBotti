#ifdef _WIN32

#define NOMINMAX
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include <iostream>
#include <string>
#include <string_view>
#include <iostream>
#include <limits>
#include "kayttoliittyma.h"

#include "varikoodit.h"

using namespace std;

Kayttoliittyma& Kayttoliittyma::getInstance()
{
    static Kayttoliittyma instance;
    return instance;
}

const Asema& Kayttoliittyma::getAsema() const { return _asema; }
Asema& Kayttoliittyma::getAsema() { return _asema; }

void Kayttoliittyma::piirraLauta(const list<Siirto>& siirrot)
{
    bool siirtoRuudut[8][8] = { false };
    
    for (auto& siirto : siirrot)
    {
        int x = siirto.getLoppuruutu().getSarake();
        int y = siirto.getLoppuruutu().getRivi();
        siirtoRuudut[y][x] = true;
    }
    
    for (int y = 7; y >= 0; y--)
    {
        cout << resetoiVarit() << to_string(y + 1) << " ";
        
        for (int x = 0; x < 8; x++)
        {
            // Joka toinen rivi valkoinen
            if ((x + y) % 2 == 0)
            {
                cout << tekstivari(musta);
                if (siirtoRuudut[y][x])
                {
                    cout << taustavari(punainen);
                }
                else
                {
                    cout << taustavari(turkoosi);
                }
            }
            else
            {
                cout << tekstivari(musta);
                if (siirtoRuudut[y][x])
                {
                    cout << taustavari(kirkkaan_punainen);
                }
                else
                {
                    cout << taustavari(valkoinen);
                }
            }
            
            string merkki = " ";
            
            // Jos NULL niin printataan tyhjä paikka.
            if (_asema.lauta[y][x] != NULL)
            {
                merkki = _asema.lauta[y][x]->getMerkki();
            }
            
            cout << " " << merkki << " ";
        }
        
        cout << resetoiVarit() << " |\n";
    }
    
    cout << resetoiVarit() << "   a  b  c  d  e  f  g  h  \n";
}

/*
 Aliohjelma tarkistaa että käyttäjän antama syšte siirroksi on
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
    
    auto tarkistaNappula = [this](char nappulaChar) -> Nappula*
    {
        if (_asema.getSiirtovuoro() == 0) {
            if (Asema::valkoinenNappulaMap.find(nappulaChar) != Asema::valkoinenNappulaMap.end())
            {
                return Asema::valkoinenNappulaMap.at(nappulaChar);
            }
        }
        else if (Asema::mustaNappulaMap.find(nappulaChar) != Asema::mustaNappulaMap.end())
        {
            return Asema::mustaNappulaMap.at(nappulaChar);
        }
        
        return nullptr;
    };
    
    while(true)
    {
        if (cin.fail())
        {
            cin.clear();
        }
        else
        {
            cout << "Sy\xc3\xb6t\xc3\xa4 siirto muodossa: Nappula, alkuruutu ja loppuruutu.\n";
            cout << "Esim. Rg1-f3. Nappulan kirjain isolla, loput pienell\xc3\xa4.\n";
        }
        
        string syote;
        cin >> syote;
        
        if (cin.fail())
        {
            continue;
        }
        
        bool pitkaLinna = syote == "O-O-O";
        bool lyhytLinna = syote == "O-O";
        if (pitkaLinna || lyhytLinna)
        {
            return Siirto(lyhytLinna, pitkaLinna);
        }
        
        if (syote.length() == 6)
        {
            char nappulaChar = 'i';
            int alkuSarake = -1;
            int alkuRivi = -1;
            int loppuSarake = -1;
            int loppuRivi = -1;
            
            nappulaChar = syote[0];
            
            alkuSarake = syote[1] - 'a';
            alkuRivi = syote[2] - '0' - 1;
            loppuSarake = syote[4] - 'a';
            loppuRivi = syote[5] - '0' - 1;
            
            Nappula* nappula = tarkistaNappula(nappulaChar);
            if (tarkistaRuutu(alkuSarake, alkuRivi) && tarkistaRuutu(loppuSarake, loppuRivi) && nappula != nullptr)
            {
                Siirto siirto(Ruutu(alkuSarake, alkuRivi), Ruutu(loppuSarake, loppuRivi));
                if ((nappula == &Asema::ms && loppuRivi == 0) || (nappula == &Asema::vs && loppuRivi == 7))
                {
                    while (true)
                    {
                        Nappula* korotus = nullptr;
                        if (cin.fail())
                        {
                            cin.clear();
                        }
                        else
                        {
                            cout << "Sy\xc3\xb6t\xc3\xa4 miksi korotetaan:\nEsim. D, R, T tai L.\n";
                        }
                        
                        char korotusChar;
                        cin >> korotusChar;
                        
                        if (cin.fail())
                        {
                            continue;
                        }
                        
                        korotus = tarkistaNappula(korotusChar);
                        if (korotus != nullptr && korotus != &Asema::ms && korotus != &Asema::vs) {
                            siirto.miksiKorotetaan = korotus;
                            break;
                        }
                    }
                }
                
                return siirto;
            }
        }
    }
}

void Kayttoliittyma::tulostaVirhe(string virhe)
{
    cout << tekstivari(punainen) << "! " << virhe <<  " !" << resetoiVarit() << std::endl;
}

int Kayttoliittyma::kysyVastustajanVari()
{
    return 0;
}
