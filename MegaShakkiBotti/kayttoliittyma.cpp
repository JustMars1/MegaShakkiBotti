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

void Kayttoliittyma::piirraLauta() {
    for (int y = 7; y >= 0; y--) {
        cout << resetoiVarit() << to_string(y + 1) << " ";
        
        for (int x = 0; x < 8; x++) {
            // Joka toinen rivi valkoinen
            if ((x + y) % 2 == 0) {
                cout << taustavari(musta) << tekstivari(valkoinen);
            }
            else {
                cout << taustavari(valkoinen) << tekstivari(musta);
            }
            
            string merkki = " ";
            
            // Jos NULL niin printataan tyhjä paikka.
            if (_asema->lauta[y][x] != NULL) {
                merkki = _asema->lauta[y][x]->getNimi();
            }
            
            cout << merkki;
        }
        
        cout << resetoiVarit() << " |\n";
    }
    
    cout << resetoiVarit() << "  abcdefgh\n";
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
    
    auto tarkistaNappula = [this](char nappulaChar, int sarake, int rivi) -> bool {
        if (Asema::charToMustaNappula.find(nappulaChar) != Asema::charToMustaNappula.end()) {
            return true;
        }
        else if (Asema::charToValkoinenNappula.find(nappulaChar) != Asema::charToValkoinenNappula.end()) {
            return true;
        }
        else {
            return false;
        }
    };
    
    char nappulaChar = 'i';
    
    int alkuSarake = -1;
    int alkuRivi = -1;
    int loppuSarake = -1;
    int loppuRivi = -1;
    bool pitkaLinna = false, lyhytLinna = false;
    
    do {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cout << "Sy\xc3\xb6t\xc3\xa4 siirto muodossa: Nappula, alkuruutu ja loppuruutu.\n";
            cout << "Esim. Rg1-f3. Nappulan kirjain isolla, loput pienell\xc3\xa4.\n";
        }
        
        string syote;
        cin >> syote;
        
        pitkaLinna = syote == "O-O-O";
        lyhytLinna = syote == "O-O";
        
        if (pitkaLinna || lyhytLinna) {
            break;
        }
        
        if (syote.length() == 6) {
            nappulaChar = syote[0];
            
            alkuSarake = syote[1] - 'a';
            alkuRivi = syote[2] - '0' - 1;
            loppuSarake = syote[4] - 'a';
            loppuRivi = syote[5] - '0' - 1;
        }
    }
    while (!tarkistaRuutu(alkuSarake, alkuRivi)
           || !tarkistaRuutu(loppuSarake, loppuRivi)
           || !tarkistaNappula(nappulaChar, alkuSarake, alkuRivi));
    
    cout << "outstanding move\n";
    
    if (pitkaLinna || lyhytLinna) {
        return Siirto(lyhytLinna, pitkaLinna);
    }
    else {
        Ruutu alku(alkuSarake, alkuRivi);
        Ruutu loppu(loppuSarake, loppuRivi);
        
        return Siirto(alku, loppu);
    }
}


int Kayttoliittyma::kysyVastustajanVari() {
    return 0;
}
