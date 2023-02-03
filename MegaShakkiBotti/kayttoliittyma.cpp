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

Kayttoliittyma& Kayttoliittyma::getInstance() {
    static Kayttoliittyma instance;
    return instance;
}

const Asema& Kayttoliittyma::getAsema() const { return _asema; }
Asema& Kayttoliittyma::getAsema() { return _asema; }

void Kayttoliittyma::piirraLauta(const list<Siirto>& siirrot) {
    bool siirtoRuudut[8][8] = { false };
    
    for (auto& siirto : siirrot)
    {
        int x = siirto.getLoppuruutu().getSarake();
        int y = siirto.getLoppuruutu().getRivi();
        siirtoRuudut[y][x] = true;
    }
    
    for (int y = 7; y >= 0; y--) {
        cout << resetoiVarit() << to_string(y + 1) << " ";
        
        for (int x = 0; x < 8; x++) {
            // Joka toinen rivi valkoinen
            if ((x + y) % 2 == 0) {
                cout << tekstivari(musta);
                if (siirtoRuudut[y][x]) {
                    cout << taustavari(punainen);
                }
                else {
                    cout << taustavari(turkoosi);
                }
            }
            else {
                cout << tekstivari(musta);
                if (siirtoRuudut[y][x]) {
                    cout << taustavari(kirkkaan_punainen);
                }
                else {
                    cout << taustavari(valkoinen);
                }
            }
            
            string merkki = " ";
            
            // Jos NULL niin printataan tyhjä paikka.
            if (_asema.lauta[y][x] != NULL) {
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
Siirto Kayttoliittyma::annaVastustajanSiirto() {
    auto tarkistaRuutu = [](int sarake, int rivi) -> bool {
        if (sarake > 7 || sarake < 0) {
            return false;
        }
        
        if (rivi > 7 || rivi < 0) {
            return false;
        }
        
        return true;
    };
    
    auto tarkistaNappula = [this](char nappulaChar) -> bool {
        if (Asema::charToMustaNappula.find(nappulaChar) != Asema::charToMustaNappula.end()) {
            return true;
        }
        else if (Asema::charToValkoinenNappula.find(nappulaChar) != Asema::charToValkoinenNappula.end()) {
            return true;
        }
        
        return false;
    };
    
    while(true) {
        if (cin.fail()) {
            cin.clear();
        }
        else {
             cout << "Sy\xc3\xb6t\xc3\xa4 siirto muodossa: Nappula, alkuruutu ja loppuruutu.\n";
            cout << "Esim. Rg1-f3. Nappulan kirjain isolla, loput pienell\xc3\xa4.\n";
        }
        
        string syote;
        cin >> syote;
        
        if (cin.fail()) {
            continue;
        }
        
        bool pitkaLinna = syote == "O-O-O";
        bool lyhytLinna = syote == "O-O";
        if (pitkaLinna || lyhytLinna) {
            return Siirto(lyhytLinna, pitkaLinna);
        }
        
        if (syote.length() == 6) {
            char nappulaChar = 'i';
            int sarake0 = -1;
            int rivi0 = -1;
            int sarake1 = -1;
            int rivi1 = -1;
            
            nappulaChar = syote[0];
            
            sarake0 = syote[1] - 'a';
            rivi0 = syote[2] - '0' - 1;
            sarake1 = syote[4] - 'a';
            rivi1 = syote[5] - '0' - 1;
            
            if (tarkistaRuutu(sarake0, rivi0) && tarkistaRuutu(sarake1, rivi1) && tarkistaNappula(nappulaChar)) {
                return Siirto(Ruutu(sarake0, rivi0), Ruutu(sarake1, rivi1));
            }
        }
    }
}


int Kayttoliittyma::kysyVastustajanVari() {
    return 0;
}
