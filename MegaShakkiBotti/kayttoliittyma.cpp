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
#include <cctype>
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

void Kayttoliittyma::piirraLauta(bool mustaAlhaalla, const list<Siirto>& siirrot)
{
    bool siirtoRuudut[8][8] = { false };
    
    for (auto& siirto : siirrot)
    {
        int x = siirto.getLoppuruutu().getSarake();
        int y = siirto.getLoppuruutu().getRivi();
        siirtoRuudut[y][x] = true;
    }
    
    for (int rivi = 0; rivi < 8; rivi++)
    {
        int y = mustaAlhaalla ? rivi : 8 - rivi - 1;
        cout << resetoiVarit() << to_string(y + 1) << " ";
        for (int sarake = 0; sarake < 8; sarake++)
        {
            int x = mustaAlhaalla ? 8 - sarake - 1 : sarake;
            
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
            if (_asema.lauta[y][x] != nullptr)
            {
                merkki = _asema.lauta[y][x]->getMerkki();
            }
            
            cout << " " << merkki << " ";
        }
        
        cout << resetoiVarit() << " |\n";
    }
    
    cout << resetoiVarit();
    
    if (mustaAlhaalla)
    {
        cout << "   h  g  f  e  d  c  b  a  \n";
    }
    else
    {
        cout << "   a  b  c  d  e  f  g  h  \n";
    }
}

/*
 Aliohjelma tarkistaa että käyttäjän antama syšte siirroksi on
 muodollisesti korrekti (ei tarkista aseman laillisuutta)
 Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
 */
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
    auto tarkistaNappula = [this](char kirjain) -> Nappula*
    {
        kirjain = tolower(kirjain);
        
        if (_asema.getSiirtovuoro() == 0) {
            if (Asema::valkeaNappulaMap.find(kirjain) != Asema::valkeaNappulaMap.end())
            {
                return Asema::valkeaNappulaMap.at(kirjain);
            }
        }
        
        if (Asema::mustaNappulaMap.find(kirjain) != Asema::mustaNappulaMap.end())
        {
            return Asema::mustaNappulaMap.at(kirjain);
        }
        
        return nullptr;
    };
    
    while(true)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            cout << "Sy\xc3\xb6t\xc3\xa4 siirto muodossa: Nappula, alkuruutu ja loppuruutu.\n";
            cout << "Esim. Rg1-f3.\n";
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
            char nappulaKirjain = 'i';
            
            Ruutu alku(-1, -1);
            Ruutu loppu(-1, -1);
            
            nappulaKirjain = syote[0];
            
            alku.setSarake(syote[1] - 'a');
            alku.setRivi(syote[2] - '0' - 1);
            loppu.setSarake(syote[4] - 'a');
            loppu.setRivi(syote[5] - '0' - 1);
            
            Nappula* nappula = tarkistaNappula(nappulaKirjain);
            if (alku.ok() && loppu.ok() && nappula != nullptr && _asema.lauta[alku.getRivi()][alku.getSarake()] == nappula)
            {
                Siirto siirto(alku, loppu);
                if ((nappula == &Asema::ms && loppu.getRivi() == 0) || (nappula == &Asema::vs && loppu.getRivi() == 7))
                {
                    while (true)
                    {
                        Nappula* korotus = nullptr;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
                        }
                        else
                        {
                            cout << "Sy\xc3\xb6t\xc3\xa4 miksi korotetaan:\nEsim. D, R, T tai L.\n";
                        }
                        
                        char korotusKirjain;
                        cin >> korotusKirjain;
                        
                        if (cin.fail())
                        {
                            continue;
                        }
                        
                        korotus = tarkistaNappula(korotusKirjain);
                        if (korotus != nullptr && korotus != &Asema::ms && korotus != &Asema::vs)
                        {
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
    cout << tekstivari(punainen) << "! " << virhe <<  " !" << resetoiVarit() << endl;
}

int Kayttoliittyma::kysyVastustajanVari()
{
    return 0;
}
