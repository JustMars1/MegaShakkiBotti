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
: _ohjelmaKaynnissa{true}
, _varitaRuudut{true} {}

Kayttoliittyma& Kayttoliittyma::getInstance()
{
    static Kayttoliittyma instance;
    return instance;
}

bool Kayttoliittyma::getVaritaRuudut() const { return _varitaRuudut; }
void Kayttoliittyma::setVaritaRuudut(bool varita)
{
    _varitaRuudut = varita;
}

bool Kayttoliittyma::getOhjelmaKaynnissa() const
{
    return _ohjelmaKaynnissa;
}

void Kayttoliittyma::suljeOhjelma()
{
    _ohjelmaKaynnissa = false;
}

Peli Kayttoliittyma::kysyPeli() const
{
    optional<Peli> peli = nullopt;
    while(!peli.has_value())
    {
        cout << "0 = uusi peli\n1 = lataa peli\nValitse ladataanko peli: ";
        
        bool lataaPeli = false;
        while(true)
        {
            int syote;
            cin >> syote;
            
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if (syote == 0 || syote == 1)
            {
                lataaPeli = syote;
                break;
            }
        }
        
        if (lataaPeli)
        {
            peli = kysyFEN();
        }
        else
        {
            peli = Peli();
        }
    }
    
    return peli.value();
}

void Kayttoliittyma::kysyPelimuoto(Peli& peli) const
{
    cout << "0 = pelaaja vs kone\n1 = kone vs kone\n2 = pelaaja vs pelaaja\nValitse pelimuoto: ";
    
    enum Pelimuoto
    {
        PELAAJA_VS_KONE = 0,
        KONE_VS_KONE = 1,
        PELAAJA_VS_PELAAJA = 2
    };
    
    int pelimuoto;
    while(true)
    {
        cin >> pelimuoto;
        
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (pelimuoto == 0 || pelimuoto == 1 || pelimuoto == 2)
        {
            break;
        }
    }
    
    switch (pelimuoto)
    {
        case PELAAJA_VS_KONE:
        {
            cout << "\n0 = valkoinen\n1 = musta\nValitse kummalla v\xc3\xa4rill\xc3\xa4 pelaat: ";
            
            int pelaajanVari = -1;
            while(true)
            {
                cin >> pelaajanVari;
                
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (pelaajanVari == 0 || pelaajanVari == 1)
                {
                    break;
                }
            }
            
            cout << "\n[1, " << numeric_limits<int>::max() << "] = hakusyvyys\nValitse koneen hakusyvyys: ";
            int syvyys = -1;
            while(true)
            {
                cin >> syvyys;
                
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (syvyys > 0)
                {
                    break;
                }
            }
            
            
            if (pelaajanVari == 0)
            {
                peli.valkoinen = Pelaaja(false, 0);
                peli.musta = Pelaaja(true, syvyys);
            }
            else
            {
                peli.valkoinen = Pelaaja(true, syvyys);
                peli.musta = Pelaaja(false, 0);
            }
            
            break;
        }
        case KONE_VS_KONE:
        {
            cout << "\n[1, " << numeric_limits<int>::max() << "] = valkoisen hakusyvyys\nValitse valkoisen hakusyvyys: ";
            int valkoisenSyvyys = -1;
            while(true)
            {
                cin >> valkoisenSyvyys;
                
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (valkoisenSyvyys > 0)
                {
                    break;
                }
            }
            
            cout << "\n[1, " << numeric_limits<int>::max() << "] = mustan hakusyvyys\nValitse mustan hakusyvyys: ";
            int mustanSyvyys = -1;
            while(true)
            {
                cin >> mustanSyvyys;
                
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (mustanSyvyys > 0)
                {
                    break;
                }
            }
            
            peli.valkoinen = Pelaaja(true, valkoisenSyvyys);
            peli.musta = Pelaaja(true, mustanSyvyys);
            break;
        }
        case PELAAJA_VS_PELAAJA:
        {
            peli.valkoinen = Pelaaja(false, 0);
            peli.musta = Pelaaja(false, 0);
            break;
        }
    }
}

void Kayttoliittyma::piirra(const Peli& peli) const
{
    enum RuutuTyyppi
    {
        TAVALLINEN_RUUTU,
        SIIRTO_RUUTU,
        VIIMEISIN_SIIRTO_RUUTU
    };
    
    vector<Siirto> siirrot = peli.asema.annaLaillisetSiirrot();
    RuutuTyyppi ruutujenTyypit[8][8] = { TAVALLINEN_RUUTU };
    
    for (auto& siirto : siirrot)
    {
        int x = siirto.getLoppuruutu().getSarake();
        int y = siirto.getLoppuruutu().getRivi();
        ruutujenTyypit[y][x] = SIIRTO_RUUTU;
    }
    
    if (peli.viimeisinSiirto.has_value())
    {
        int x = peli.viimeisinSiirto.value().getAlkuruutu().getSarake();
        int y = peli.viimeisinSiirto.value().getAlkuruutu().getRivi();
        ruutujenTyypit[y][x] = VIIMEISIN_SIIRTO_RUUTU;
        
        x = peli.viimeisinSiirto.value().getLoppuruutu().getSarake();
        y = peli.viimeisinSiirto.value().getLoppuruutu().getRivi();
        ruutujenTyypit[y][x] = VIIMEISIN_SIIRTO_RUUTU;
    }
    
    bool mustaAlhaalla = false;
    
    if (!peli.musta.onkoKone && peli.valkoinen.onkoKone)
    {
        mustaAlhaalla = true;
    }
    else if (!peli.musta.onkoKone && !peli.valkoinen.onkoKone)
    {
        mustaAlhaalla = peli.asema.getSiirtovuoro() == 1;
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
                switch (ruutujenTyypit[y][x]) {
                    case TAVALLINEN_RUUTU:
                    {
                        cout << taustavari(turkoosi);
                        break;
                    }
                    case SIIRTO_RUUTU:
                    {
                        cout << taustavari(sininen);
                        break;
                    }
                    case VIIMEISIN_SIIRTO_RUUTU:
                    {
                        cout << taustavari(purppura);
                        break;
                    }
                }
            }
            else
            {
                cout << tekstivari(musta);
                switch (ruutujenTyypit[y][x]) {
                    case TAVALLINEN_RUUTU:
                    {
                        cout << taustavari(valkoinen);
                        break;
                    }
                    case SIIRTO_RUUTU:
                    {
                        cout << taustavari(tumman_harmaa);
                        break;
                    }
                    case VIIMEISIN_SIIRTO_RUUTU:
                    {
                        cout << taustavari(kirkkaan_purppura);
                        break;
                    }
                }
            }
            
            string merkki = " ";
            
            if (peli.asema.lauta[y][x] != nullptr)
            {
                merkki = peli.asema.lauta[y][x]->getMerkki();
            }
            
            cout << " " << merkki << " ";
        }
        
        cout << resetoiVarit();
#ifdef _WIN32
        cout << " |";
#endif
        cout << endl;
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
Siirto Kayttoliittyma::kysyVastustajanSiirto()
{
    auto tarkistaNappula = [this](char kirjain) -> Nappula*
    {
        kirjain = tolower(kirjain);
        
        for (auto nappula : Asema::nappulat)
        {
            char nappulaKirjain = nappula->getKirjainSuomi();
            if (tolower(nappulaKirjain) != kirjain)
            {
                return nappula;
            }
        }
        
        return nullptr;
    };
    
    
    while(true)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            if (alku.ok() && loppu.ok() && nappula != nullptr)
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
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

void Kayttoliittyma::tulostaVirhe(string virhe) const
{
    cout << tekstivari(punainen) << "! " << virhe <<  " !" << resetoiVarit() << endl;
}

bool Kayttoliittyma::tarkistaKomento(string komento, const Peli& peli) const
{
    for (char& kirjain : komento)
    {
        kirjain = tolower(kirjain);
    }
    
    if (komento == "siirrot")
    {
        vector<Siirto> siirrot = peli.asema.annaLaillisetSiirrot();
        
        for (Siirto& siirto : siirrot)
        {
            if (!siirto.onkoLyhytLinna() && !siirto.onkoPitkaLinna())
            {
                int y = siirto.getAlkuruutu().getRivi();
                int x = siirto.getAlkuruutu().getSarake();
                char kirjain = toupper(peli.asema.lauta[y][x]->getKirjainSuomi());
                cout << kirjain;
            }
            
            cout << siirto << endl;
        }
        
        return true;
    }
    
    return false;
}

optional<Peli> Kayttoliittyma::kysyFEN() const
{
    cout << "Anna FEN: ";
    
    Peli peli;
    peli.asema = Asema({nullptr});
    peli.asema.setMustaDTliikkunut(true);
    peli.asema.setMustaKTliikkunut(true);
    peli.asema.setMustaKuningasLiikkunut(true);
    peli.asema.setValkeaDTliikkunut(true);
    peli.asema.setValkeaKTliikkunut(true);
    peli.asema.setValkeaKuningasLiikkunut(true);
    
    // Lauta
    
    string lauta;
    cin >> lauta;
    
    int x = 0, y = 0;
    
    for (char kirjain : lauta)
    {
        if (isdigit(kirjain))
        {
            x += kirjain - '0';
        }
        else if (kirjain != '/')
        {
            auto nappulaItr = find_if(Asema::nappulat.begin(), Asema::nappulat.end(), [kirjain](auto nappula) {
                return nappula->getKirjainEnglanti() == kirjain;
            });
            
            if (nappulaItr == Asema::nappulat.end())
            {
                tulostaVirhe("Virheellinen FEN (lauta)");
                return nullopt;
            }
            
            int rivi = 8 - y - 1;
            
            if (*nappulaItr == &Asema::vk)
            {
                peli.asema.setValkeanKuninkaanRuutu(Ruutu(x, rivi));
            }
            else if (*nappulaItr == &Asema::mk)
            {
                peli.asema.setMustanKuninkaanRuutu(Ruutu(x, rivi));
            }
            
            peli.asema.lauta[rivi][x] = *nappulaItr;
            x++;
        }
        
        y += x / 8;
        x %= 8;
    }
    
    // Vuoro
    
    char vuoro = 'e';
    cin >> vuoro;
    
    if (vuoro == 'w')
    {
        peli.asema.setSiirtovuoro(0);
    }
    else if (vuoro == 'b')
    {
        peli.asema.setSiirtovuoro(1);
    }
    else
    {
        tulostaVirhe("Virheellinen FEN (vuoro)");
        return nullopt;
    }
    
    // Linnoitus
    
    string linnoitus;
    cin >> linnoitus;
    
    if (linnoitus != "-")
    {
        for (char kirjain : linnoitus)
        {
            switch (kirjain) {
                case 'K':
                {
                    peli.asema.setValkeaKTliikkunut(false);
                    peli.asema.setValkeaKuningasLiikkunut(false);
                    break;
                }
                case 'Q':
                {
                    peli.asema.setValkeaDTliikkunut(false);
                    peli.asema.setValkeaKuningasLiikkunut(false);
                    break;
                }
                case 'k':
                {
                    peli.asema.setMustaKTliikkunut(false);
                    peli.asema.setMustaKuningasLiikkunut(false);
                    break;
                }
                case 'q':
                {
                    peli.asema.setMustaDTliikkunut(false);
                    peli.asema.setMustaKuningasLiikkunut(false);
                    break;
                }
                default:
                {
                    tulostaVirhe("Virheellinen FEN (linnoitus)");
                    return nullopt;
                }
            }
        }
    }
    
    // Ohestalyonti
    
    string ohestalyonti;
    cin >> ohestalyonti;
    
    if (ohestalyonti != "-")
    {
        if (ohestalyonti.length() == 2)
        {
            int sarake = ohestalyonti[0] - 'a';
            if (sarake >= 0 && sarake < 7)
            {
                peli.asema.setKaksoisaskelSarake(sarake);
            }
            else
            {
                tulostaVirhe("Virheellinen FEN (ohestalyonti, sarake)");
                return nullopt;
            }
            
            int rivi = ohestalyonti[1] - '0';
            
            if (rivi < 0 || rivi > 7)
            {
                tulostaVirhe("Virheellinen FEN (ohestalyonti, rivi)");
                return nullopt;
            }
        }
    }
    
    // Siirtolaskuri
    
    int siirtolaskuri = -1;
    cin >> siirtolaskuri;
    
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (siirtolaskuri < 0)
    {
        tulostaVirhe("Virheellinen FEN (siirtolaskuri)");
        return nullopt;
    }
    
    int vuorolaskuri = -1;
    cin >> vuorolaskuri;
    
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (vuorolaskuri < 0)
    {
        tulostaVirhe("Virheellinen FEN (vuorolaskuri)");
        return nullopt;
    }
    
    return peli;
}
