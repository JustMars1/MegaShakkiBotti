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

Kayttoliittyma::Kayttoliittyma()
{
    int saieMaara = std::thread::hardware_concurrency() - 1;
    saikeet.reserve(saieMaara);

    for (int i = 0; i < saieMaara; i++)
    {
        saikeet.push_back(std::thread());
    }
};

void Kayttoliittyma::saieLuuppi()
{
    while (true)
    {
        while (tehtavat.empty())
        {
            std::this_thread::sleep_for(1ms);
        }

        auto tehtava = tehtavat.front();
        tehtavat.pop();
        tehtava();

    }
}

void Kayttoliittyma::lisaaTehtava(std::function<void()> tehtava)
{
    tehtavat.push(tehtava);
}

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
            
            // Jos NULL niin printataan tyhj� paikka.
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
 Aliohjelma tarkistaa ett� k�ytt�j�n antama sy�te siirroksi on
 muodollisesti korrekti (ei tarkista aseman laillisuutta)
 Ottaa irti my�s nappulan kirjaimen (K/D/L/R/T), tarkistaa ett� kirjain korrekti
 */
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
    auto tarkistaNappula = [this](char kirjain) -> Nappula*
    {
        kirjain = std::tolower(kirjain);
        
        if (_asema.getSiirtovuoro() == 0) {
            if (Asema::valkoinenNappulaMap.find(kirjain) != Asema::valkoinenNappulaMap.end())
            {
                return Asema::valkoinenNappulaMap.at(kirjain);
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
