#ifdef _WIN32

#define NOMINMAX
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <cctype>
#include <array>
#include <exception>
#include <algorithm>
#include "kayttoliittyma.h"

#include "varikoodit.h"

using namespace std;

const std::string& operator""_k(const char* avain, size_t)
{
    return Kayttoliittyma::getInstance().getKieli().get(avain);
}

Kayttoliittyma::Kayttoliittyma()
: _ohjelmaKaynnissa{true}
, _varitaRuudut{true}
, _kielet(lataaKielet())
, _kieli(0)
, _onkoUCI{false} {}

vector<Kieli> Kayttoliittyma::lataaKielet()
{
    ifstream file("kielet.csv");
    
    if (file)
    {
        std::vector<Kieli> kielet;
        string tmp;
        
        {
            getline(file, tmp);
            stringstream nimiRivi(tmp);
            string nimiAvain, nimiArvo;
            
            getline(nimiRivi, nimiAvain, ',');
            while(getline(nimiRivi, nimiArvo, ','))
            {
                Kieli kieli;
                kieli.kaannokset[nimiAvain] = nimiArvo;
                kielet.push_back(kieli);
            }
        }
        
        while(getline(file, tmp))
        {
            stringstream rivi(tmp);
            
            string avain;
            getline(rivi, avain, ',');
            
            int i = 0;
            string arvo;
            while(getline(rivi, arvo, ','))
            {
                kielet[i].kaannokset[avain] = arvo;
                i++;
            }
        }
        
        if (kielet.empty())
        {
            cerr << "! virhe kielten lataamisessa !" << endl;
            terminate();
        }
        
        return kielet;
    }
    else
    {
        cerr << "! kielet.csv tiedosto puuttuu !" << endl;
        terminate();
    }
}

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

const Kieli& Kayttoliittyma::getKieli() const { return _kielet[_kieli]; }

bool Kayttoliittyma::getOnkoUCI() const { return _onkoUCI; }

void Kayttoliittyma::kysyKieli()
{
    for (size_t i = 0; i < _kielet.size(); i++)
    {
        cout << i << " = " << _kielet[i].get("kielenNimi") << endl;
    }
    
    for (size_t i = 0; i < _kielet.size() - 1; i++)
    {
        cout << _kielet[i].get("valitseKieli") << "/";
    }
    
    cout << _kielet.back().get("valitseKieli")  << ": ";
    
    int kieli = -1;
    while(true)
    {
        cin >> kieli;
        
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (kieli > -1 && kieli < _kielet.size())
        {
            break;
        }
    }
    
    _kieli = kieli;
}

void Kayttoliittyma::tulostaSiirtoOhje(bool uci, size_t sisennys) const
{
    string nappulaOhje = "[";
    int nappulaLkm = Asema::nappulat.size() / 2;
    
    for (int i = 0; i < nappulaLkm - 1; i++)
    {
        Nappula* nappula = Asema::nappulat[i];
        string merkki = nappula->getSiirtoMerkki(uci);
        if (uci)
        {
            transform(merkki.begin(), merkki.end(), merkki.begin(), ::tolower);
        }
        
        nappulaOhje += merkki + "|";
    }
    
    {
        Nappula* nappula = Asema::nappulat[nappulaLkm - 1];
        string merkki = nappula->getSiirtoMerkki(uci);
        if (uci)
        {
            transform(merkki.begin(), merkki.end(), merkki.begin(), ::tolower);
        }
        
        nappulaOhje += merkki + "]";
    }
    
    string siirtoOhje;
    if (uci)
    {
        siirtoOhje = "[" + "ruutu"_k + "][" + "ruutu"_k + "]";
    }
    else
    {
        siirtoOhje = "[" + "nappula"_k + "][" + "ruutu"_k + "]-[" + "ruutu"_k + "]";
    }
    
    string korotusOhje;
    if (uci)
    {
        korotusOhje = "[" + "ruutu"_k + "][" + "ruutu"_k + "]" + "[" + "nappula"_k + "]";
    }
    else
    {
        korotusOhje = "[" + "nappula"_k + "]" + "[" + "ruutu"_k + "]-[" + "ruutu"_k + "]" + "[" + "nappula"_k + "]";
    }
    
    string ruutuOhje = "[a, h][1, 8]";
    string lyhytLinnoitusOhje = uci ? "e1g1/e8g8" : "O-O";
    string pitkaLinnoitusOhje = uci ? "e1c1/e8c8" : "O-O-O";
    
    size_t leveys = max(siirtoOhje.length(), nappulaOhje.length());
    leveys = max(leveys, korotusOhje.length());
    
    cout << string(sisennys, ' ');
    cout << nappulaOhje << string(leveys - nappulaOhje.length(), ' ') << " = " << "nappula"_k << endl;
    
    cout << string(sisennys, ' ');
    cout << ruutuOhje << string(leveys - ruutuOhje.length(), ' ') << " = " << "ruutu"_k << " (" << "sarake"_k << ", " << "rivi"_k << ")" << endl;
    
    cout << string(sisennys, ' ');
    cout << siirtoOhje << string(leveys - siirtoOhje.length(), ' ') << " = " << "siirto"_k << endl;
    
    cout << string(sisennys, ' ');
    cout << korotusOhje << string(leveys - korotusOhje.length(), ' ') << " = " << "korotus"_k << endl;
    
    cout << string(sisennys, ' ');
    cout << lyhytLinnoitusOhje << string(leveys - lyhytLinnoitusOhje.length(), ' ') << " = " << "lyhytLinnoitus"_k << endl;
    
    cout << string(sisennys, ' ');
    cout << pitkaLinnoitusOhje << string(leveys - pitkaLinnoitusOhje.length(), ' ') << " = " << "pitkaLinnoitus"_k << endl;
}

Peli Kayttoliittyma::kysyPeli() const
{
    optional<Peli> peli = nullopt;
    while(!peli.has_value())
    {
        cout
        << "0 = " << "uusiPeli"_k << endl
        << "1 = " << "lataaPeli"_k << endl
        << "valitseLadataankoPeli"_k << ": ";
        
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
    cout
    << "0 = " << "pelaajaVsKone"_k << endl
    << "1 = " << "koneVsKone"_k << endl
    << "2 = " << "pelaajaVsPelaaja"_k << endl
    << "valitsePelimuoto"_k << ": ";
    
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
        else if (pelimuoto == PELAAJA_VS_KONE || pelimuoto == KONE_VS_KONE || pelimuoto == PELAAJA_VS_PELAAJA)
        {
            break;
        }
    }
    
    switch (pelimuoto)
    {
        case PELAAJA_VS_KONE:
        {
            cout << endl
            << "0 = " << "valkoinen"_k << endl
            << "1 = " << "musta"_k << endl
            << "valitseKummallaVarillaPelaat"_k << ": ";
            
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
            
            cout << endl
            << "[1, " << numeric_limits<int>::max() << "] = " << "hakusyvyys"_k << endl
            << "valitseKoneenHakusyvyys"_k << ": ";
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
            cout << endl
            << "[1, " << numeric_limits<int>::max() << "] = " << "hakusyvyys"_k << endl
            << "valitseValkoisenHakusyvyys"_k << ": ";
            
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
            
            cout << endl
            << "[1, " << numeric_limits<int>::max() << "] = " << "hakusyvyys"_k << endl
            << "valitseMustanHakusyvyys"_k << ": ";
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
    
    float laudanArvo = peli.asema.evaluoi();
    
    string siirrotKomento = "/siirrot"_k;
    string fenKomento = "/fen";
    size_t leveys = std::max(siirrotKomento.length(), fenKomento.length());
    
    array<string, 8> sivupalkki =
    {
        to_string(peli.siirtoparilaskuri) + ". " + "siirtopari"_k,
        (peli.asema.getSiirtovuoro() == 0 ? "valkoisenSiirtovuoro"_k : "mustanSiirtovuoro"_k),
        (laudanArvo > 0 ? "+" : "") + to_string(laudanArvo),
        "komennot"_k + ": ",
        siirrotKomento + string(leveys - siirrotKomento.length(), ' ') + " = " + ("listaaLaillisetSiirrot"_k),
        fenKomento + string(leveys - fenKomento.length(), ' ') + " = " + ("tulostaAsemaFEN-merkintana"_k),
        "/",
        "/"
    };
    
    for (int y = 0; y < 8; y++)
    {
        int rivi = mustaAlhaalla ? y : 7 - y;
        cout << resetoiVarit() << to_string(rivi + 1) << " ";
        for (int x = 0; x < 8; x++)
        {
            int sarake = mustaAlhaalla ? 7 - x : x;
            
            // Joka toinen rivi valkoinen
            if ((sarake + rivi) % 2 == 0)
            {
                cout << tekstivari(musta);
                switch (ruutujenTyypit[rivi][sarake])
                {
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
                switch (ruutujenTyypit[rivi][sarake])
                {
                    case TAVALLINEN_RUUTU:
                    {
                        cout << taustavari(valkoinen);
                        break;
                    }
                    case SIIRTO_RUUTU:
                    {
                        cout << taustavari(kirkkaan_sininen);
                        break;
                    }
                    case VIIMEISIN_SIIRTO_RUUTU:
                    {
                        cout << taustavari(kirkkaan_purppura);
                        break;
                    }
                }
            }
            
            Nappula* nappula = peli.asema.lauta[rivi][sarake];
            cout << " " << (nappula == nullptr ? " " : nappula->getLautaMerkki()) << " ";
        }
        
        cout << resetoiVarit() << " " << sivupalkki[y] << endl;
    }
    
    cout << resetoiVarit();
    
    if (mustaAlhaalla)
    {
        cout << "   h  g  f  e  d  c  b  a  ";
    }
    else
    {
        cout << "   a  b  c  d  e  f  g  h  ";
    }
    
    cout << endl;
}

/*
 Aliohjelma tarkistaa ett� k�ytt�j�n antama sy�te siirroksi on
 muodollisesti korrekti (ei tarkista aseman laillisuutta)
 
 UCI:
 Ottaa irti myös korotettaessa nappulan kirjaimen (D/L/R/T), tarkistaa että kirjain korrekti
 
 Ei UCI:
 Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
 */
Siirto Kayttoliittyma::kysyVastustajanSiirto(const Peli& peli)
{
    auto tarkistaNappula = [this, &peli](string merkki) -> Nappula*
    {
        transform(merkki.begin(), merkki.end(), merkki.begin(), ::tolower);
        
        for (auto nappula : Asema::nappulat)
        {
            if (nappula->getVari() == peli.asema.getSiirtovuoro())
            {
                string siirtoMerkki = nappula->getSiirtoMerkki();
                transform(siirtoMerkki.begin(), siirtoMerkki.end(), siirtoMerkki.begin(), ::tolower);
                
                if (siirtoMerkki == merkki)
                {
                    return nappula;
                }
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
            cout << "syotaSiirto"_k << ": ";
        }
        
        string syote;
        cin >> syote;
        
        if (cin.fail())
        {
            continue;
        }
        else if (tarkistaKomento(syote, peli))
        {
            continue;
        }
        
        if (_onkoUCI)
        {
            bool pitkaLinna = false;
            bool lyhytLinna = false;

            if (syote == "e1c1" || syote == "e8c8")
            {
                pitkaLinna = true;
            }
            else if (syote == "e1g1" || syote == "e8g8")
            {
                lyhytLinna = true;
            }
            
            if (pitkaLinna || lyhytLinna)
            {
                return Siirto(lyhytLinna, pitkaLinna);
            }
            
            if (syote.length() < 4)
            {
                tulostaVirhe("Tuntematon siirto");
                continue;
            }
            
            Ruutu alku(-1, -1);
            Ruutu loppu(-1, -1);
            
            alku.setSarake(syote[0] - 'a');
            alku.setRivi(syote[1] - '0' - 1);
            
            loppu.setSarake(syote[2] - 'a');
            loppu.setRivi(syote[3] - '0' - 1);
            
            if (!alku.ok() || !loppu.ok())
            {
                tulostaVirhe("Virheelliset ruudut");
                continue;
            }
            
            Siirto siirto(alku, loppu);
            
            if (syote.length() > 4)
            {
                string korotusMerkki = syote.substr(4);
                Nappula* korotusNappula = tarkistaNappula(korotusMerkki);
                
                if (korotusNappula == nullptr || korotusNappula == &Asema::vs || korotusNappula == &Asema::ms)
                {
                    tulostaVirhe("Siirron lopussa on virheellisia merkkeja.");
                    continue;
                }
                
                siirto.miksiKorotetaan = korotusNappula;
            }
            
            return siirto;
        }
        else
        {
            bool pitkaLinna = syote == "O-O-O";
            bool lyhytLinna = syote == "O-O";
            
            if (pitkaLinna || lyhytLinna)
            {
                return Siirto(lyhytLinna, pitkaLinna);
            }
            
            if (syote.length() < 6)
            {
                tulostaVirhe("Tuntematon siirto");
                continue;
            }
            
            auto viivaIdx = syote.find('-');
            
            if (viivaIdx == string::npos)
            {
                tulostaVirhe("Siirrosta puuttuu -");
                continue;
            }
            
            if (viivaIdx + 2 > syote.length() - 1 || viivaIdx - 2 < 1)
            {
                tulostaVirhe("Virheellinen siirto");
                continue;
            }
            
            Ruutu alku(-1, -1);
            Ruutu loppu(-1, -1);
            
            alku.setSarake(syote[viivaIdx - 2] - 'a');
            alku.setRivi(syote[viivaIdx - 1] - '0' - 1);
            
            loppu.setSarake(syote[viivaIdx + 1] - 'a');
            loppu.setRivi(syote[viivaIdx + 2] - '0' - 1);
            
            string siirtoMerkki = syote.substr(0, viivaIdx - 2);
            Nappula* siirtoNappula = tarkistaNappula(siirtoMerkki);
            
            if (siirtoNappula == nullptr)
            {
                tulostaVirhe("Siirron alussa ei mainita nappulaa.");
                continue;
            }
            
            if (!alku.ok() || !loppu.ok())
            {
                tulostaVirhe("Virheelliset ruudut");
                continue;
            }
            
            Siirto siirto(alku, loppu);
            
            if (syote.length() - 1 > viivaIdx + 2)
            {
                string korotusMerkki = syote.substr(viivaIdx + 2 + 1);
                Nappula* korotusNappula = tarkistaNappula(korotusMerkki);
                
                if (korotusNappula == nullptr || korotusNappula == &Asema::vs || korotusNappula == &Asema::ms)
                {
                    tulostaVirhe("Siirron lopussa on virheellisia merkkeja.");
                    continue;
                }
                
                if (siirtoNappula != &Asema::vs && siirtoNappula != &Asema::ms)
                {
                    tulostaVirhe("Vain sotilas voidaan korottaa.");
                    continue;
                }
                
                siirto.miksiKorotetaan = korotusNappula;
            }
            
            return siirto;
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
    
    if (komento == "/siirrot"_k)
    {
        vector<Siirto> siirrot = peli.asema.annaLaillisetSiirrot();
        
        for (Siirto& siirto : siirrot)
        {
            if (!siirto.onkoLyhytLinna() && !siirto.onkoPitkaLinna())
            {
                int y = siirto.getAlkuruutu().getRivi();
                int x = siirto.getAlkuruutu().getSarake();
                string merkki = peli.asema.lauta[y][x]->getSiirtoMerkki();
                transform(merkki.begin(), merkki.end(), merkki.begin(), ::toupper);
                cout << merkki;
            }
            
            cout << siirto << endl;
        }
        
        return true;
    }
    else if (komento == "/fen")
    {
        
    }
    
    return false;
}

optional<Peli> Kayttoliittyma::kysyFEN() const
{
    cout << "annaFEN"_k << ": ";
    
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
                return nappula->getFENMerkki()[0] == kirjain;
            });
            
            if (nappulaItr == Asema::nappulat.end())
            {
                tulostaVirhe("Virheellinen FEN (lauta)");
                return nullopt;
            }
            
            int rivi = 7 - y;
            
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
    int siirtoaSyonnistaTaiSotilaanSiirrosta = -1;
    cin >> siirtoaSyonnistaTaiSotilaanSiirrosta;
    
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (siirtoaSyonnistaTaiSotilaanSiirrosta < 0)
    {
        tulostaVirhe("Virheellinen FEN (siirtolaskuri)");
        return nullopt;
    }
    
    // Siirtoparilaskuri
    int siirtoparilaskuri = -1;
    cin >> siirtoparilaskuri;
    
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (siirtoparilaskuri < 0)
    {
        tulostaVirhe("Virheellinen FEN (vuorolaskuri)");
        return nullopt;
    }
    
    peli.siirtoparilaskuri = siirtoparilaskuri;
    
    return peli;
}

void Kayttoliittyma::kysySiirronMerkintatapa()
{
    cout << "0" << endl;
    tulostaSiirtoOhje(false, 3);
    cout << endl << "1" << endl;
    tulostaSiirtoOhje(true, 3);
    cout << endl << "valitseSiirronMerkintatapa"_k << ": ";
    
    bool uci = false;
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
            uci = syote;
            break;
        }
    }
    
    _onkoUCI = uci;
}

void Kayttoliittyma::setOnkoUCI(bool uci)
{
    _onkoUCI = uci;
}