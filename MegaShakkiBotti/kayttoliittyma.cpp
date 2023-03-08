#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <iostream>
#include <limits>
#include <cctype>
#include <array>
#include <exception>
#include <algorithm>
#include <iomanip>
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
    const char* kieletCSV =
#include "kielet.csv"
    ;
    
    stringstream data(kieletCSV);
    
    std::vector<Kieli> kielet;
    string tmp;
    
    {
        getline(data, tmp);
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
    
    while(getline(data, tmp))
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

Kayttoliittyma& Kayttoliittyma::getInstance()
{
    static Kayttoliittyma instance;
    return instance;
}

const Kieli& Kayttoliittyma::getKieli() const { return _kielet[_kieli]; }

bool Kayttoliittyma::getVaritaRuudut() const { return _varitaRuudut; }
void Kayttoliittyma::setVaritaRuudut(bool varita)
{
    _varitaRuudut = varita;
}

bool Kayttoliittyma::getOnkoUCI() const { return _onkoUCI; }
void Kayttoliittyma::setOnkoUCI(bool uci)
{
    _onkoUCI = uci;
}

void Kayttoliittyma::kysyAsetukset()
{
    kysyKieli();
    cout << endl;
    kysySiirronMerkintatapa();
    cout << endl;
}

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

void Kayttoliittyma::tulostaSiirtoOhje(bool uci, int sisennys) const
{
    string nappulaOhje = "[";
    int nappulaLkm = Asema::nappulat.size() / 2;
    
    for (int i = 0; i < nappulaLkm - 1; i++)
    {
        Nappula* nappula = Asema::nappulat[i];
        string merkki = uci ? nappula->getFENMerkki() : nappula->getSiirtoMerkki();
        transform(merkki.begin(), merkki.end(), merkki.begin(), (uci ? ::tolower : ::toupper));
        nappulaOhje += merkki + "|";
    }
    
    {
        Nappula* nappula = Asema::nappulat[nappulaLkm - 1];
        string merkki = uci ? nappula->getFENMerkki() : nappula->getSiirtoMerkki();
        transform(merkki.begin(), merkki.end(), merkki.begin(), (uci ? ::tolower : ::toupper));
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
    
    int leveys = static_cast<int>(max(siirtoOhje.length(), nappulaOhje.length()));
    leveys = max(leveys, static_cast<int>(korotusOhje.length()));
    
    cout
    << left << setw(sisennys) << ""
    << setw(leveys) << nappulaOhje << " = " << "nappula"_k << endl
    << setw(sisennys) << ""
    << setw(leveys) << ruutuOhje << " = " << "ruutu"_k << " (" << "sarake"_k << ", " << "rivi"_k << ")" << endl
    << setw(sisennys) << ""
    << setw(leveys) << siirtoOhje << " = " << "siirto"_k << endl
    << setw(sisennys) << ""
    << setw(leveys) << korotusOhje << " = " << "korotus"_k << endl
    << setw(sisennys) << ""
    << setw(leveys) << lyhytLinnoitusOhje << " = " << "lyhytLinnoitus"_k << endl
    << setw(sisennys) << ""
    << setw(leveys) << pitkaLinnoitusOhje << " = " << "pitkaLinnoitus"_k << endl;
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
    
    if (!peli.siirtoHistoria.empty())
    {
        const Siirto& siirto = peli.siirtoHistoria.back();
        int x = siirto.getAlkuruutu().getSarake();
        int y = siirto.getAlkuruutu().getRivi();
        ruutujenTyypit[y][x] = VIIMEISIN_SIIRTO_RUUTU;
        
        x = siirto.getLoppuruutu().getSarake();
        y = siirto.getLoppuruutu().getRivi();
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
    
    string siirrotKomento = "/" + "siirrot"_k;
    string fenKomento = "/fen";
    string kumoaKomento = "/" + "kumoa"_k;
    string asetuksetKomento = "/" + "asetukset"_k;
    string historiaKomento = "/" + "historia"_k;
    size_t leveys = max(siirrotKomento.length(), fenKomento.length());
    leveys = max(leveys, kumoaKomento.length());
    leveys = max(leveys, asetuksetKomento.length());
    leveys = max(leveys, historiaKomento.length());
    
    array<string, 9> sivupalkki =
    {
        to_string(peli.siirtoHistoria.size() / 2 + 1) + ". " + "siirtopari"_k,
        (peli.asema.getSiirtovuoro() == 0 ? "valkoisenSiirtovuoro"_k : "mustanSiirtovuoro"_k),
        (laudanArvo > 0 ? "+" : "") + to_string(laudanArvo),
        "komennot"_k + ": ",
        siirrotKomento + string(leveys - siirrotKomento.length(), ' ') + " = " + "listaaLaillisetSiirrot"_k,
        fenKomento + string(leveys - fenKomento.length(), ' ') + " = " + "tulostaAsemaFEN-merkintana"_k,
        kumoaKomento + string(leveys - kumoaKomento.length(), ' ') + " = " + "kumoa2EdellistaSiirtoa"_k,
        asetuksetKomento + string(leveys - asetuksetKomento.length(), ' ') + " = " + "paivitaAsetukset"_k,
        historiaKomento + string(leveys - historiaKomento.length(), ' ') + " = " + "listaaSiirtoHistoria"_k
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
    
    cout << sivupalkki.back() << endl;
    
    cout << endl;
}

void Kayttoliittyma::tulostaVirhe(string virhe) const
{
    cout << tekstivari(punainen) << "! " << virhe <<  " !" << resetoiVarit() << endl;
}

bool Kayttoliittyma::tarkistaKomento(const string& syote, Peli& peli)
{
    if (syote.empty() || syote[0] != '/')
    {
        return false;
    }
    
    string komento(syote.begin() + 1, syote.end());
    transform(komento.begin(), komento.end(), komento.begin(), ::tolower);
    
    if (komento == "siirrot"_k)
    {
        vector<Siirto> siirrot = peli.asema.annaLaillisetSiirrot();
        
        int lkm = 0;
        
        cout << left;
        for (auto& siirto : siirrot)
        {
            Asema tmpAsema = peli.asema;
            tmpAsema.paivitaAsema(siirto);
            float arvo = tmpAsema.evaluoi();
            string etumerkki = arvo > 0 ? "+" : (arvo == 0 ? " " : "");
            cout << setw(15) << siirto.getMerkinta(peli.asema) << setw(15) << etumerkki + to_string(arvo);
            
            lkm++;
            if (lkm == 2)
            {
                lkm = 0;
                cout << endl;
            }
        }
        
        cout << endl;
        
        for (int syvyys = 1; syvyys <= 6; syvyys++)
        {
            MinMaxPaluu minMax = peli.asema.alphabetaMinimaxAsync(syvyys);
            Siirto siirto = minMax.parasSiirto;
            float arvo = minMax.evaluointiArvo;
            
            string teksti = "suositus"_k + " ("  + "hakusyvyys"_k + " " + to_string(syvyys) + "): ";
            string etumerkki = arvo > 0 ? "+" : (arvo == 0 ? " " : "");
            
            cout << left << teksti << setw(15) << siirto.getMerkinta(peli.asema) << etumerkki + to_string(minMax.evaluointiArvo) << endl;
        }
    }
    else if (komento == "fen")
    {
        cout << "FEN: " << peli << endl;
    }
    else if (komento == "kumoa"_k)
    {
        if (!peli.siirtoHistoria.empty())
        {
            for (int i = 0; i < 2; i++)
            {
                cout << "kumottu"_k << " " << peli.siirtoHistoria.back() << endl;
                peli.siirtoHistoria.pop_back();
            }
            
            Asema asema = peli.getAloitusAsema();
            for (auto& siirto : peli.siirtoHistoria)
            {
                asema.paivitaAsema(siirto);
            }
            
            peli.asema = asema;
            cout << endl;
            piirra(peli);
            cout << endl;
        }
    }
    else if (komento == "asetukset"_k)
    {
        cout << endl;
        kysyAsetukset();
        piirra(peli);
        cout << endl;
    }
    else if (komento == "historia"_k)
    {
        Asema asema = peli.getAloitusAsema();
        for (auto& siirto : peli.siirtoHistoria)
        {
            cout << siirto.getMerkinta(asema) << endl;
            asema.paivitaAsema(siirto);
        }
    }
    else
    {
        tulostaVirhe("tuntematonKomento"_k);
    }
    
    return true;
}

optional<Peli> Kayttoliittyma::kysyFEN() const
{
    string fen;
    cout << "annaFEN"_k << ": ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, fen);
    
    return Peli::lue(fen);
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
