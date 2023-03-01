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
#include <array>
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
        else if (pelimuoto == PELAAJA_VS_KONE || pelimuoto == KONE_VS_KONE || pelimuoto == PELAAJA_VS_PELAAJA)
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
    
    float laudanArvo = peli.asema.evaluoi();
    
    array<string, 8> sivupalkki =
    {
        to_string(peli.siirtoparilaskuri) + ". siirtopari",
        string(peli.asema.getSiirtovuoro() == 0 ? "Valkoisen" : "Mustan") + " siirtovuoro",
        (laudanArvo > 0 ? "+" : "") + to_string(laudanArvo),
        "Komennot:",
        "/siirrot = listaa lailliset siirrot",
        "/fen     = tulosta asema FEN-merkint\xc3\xa4n\xc3\xa4",
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
            cout << " " << (nappula == nullptr ? " " : nappula->getMerkki()) << " ";
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
 Aliohjelma tarkistaa että käyttäjän antama syšte siirroksi on
 muodollisesti korrekti (ei tarkista aseman laillisuutta)
 Ottaa irti myös korotettaessa nappulan kirjaimen (D/L/R/T), tarkistaa että kirjain korrekti
 */
Siirto Kayttoliittyma::kysyVastustajanSiirto(const Peli& peli)
{
    auto tarkistaNappula = [this](char kirjain) -> Nappula*
    {
        kirjain = tolower(kirjain);
        
        for (auto nappula : Asema::nappulat)
        {
            char nappulaKirjain = nappula->getKirjainSuomi();
            // Tää if oli alunperin != tsekkaus. Sillä ei toiminut, etsi aina tornin, tai kun yritti korottaa torniksi niin etsi ratsun
            // Nyt tuolla == tsekkauksella korotus toimii.
            if (tolower(nappulaKirjain) == kirjain)
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
            cout << "Sy\xc3\xb6t\xc3\xa4 siirto: ";
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
        
        bool pitkaLinna = syote == "O-O-O";
        bool lyhytLinna = syote == "O-O";
        
        if (pitkaLinna || lyhytLinna)
        {
            return Siirto(lyhytLinna, pitkaLinna);
        }
        
        // UCI-protokollalle sopiva syötemuoto.
        // Syötteeseen riittää vain ruudut. Esim. h2-h4.
        if (syote.length() == 5)
        {       
            Ruutu alku(-1, -1);
            Ruutu loppu(-1, -1);
            
            alku.setSarake(syote[0] - 'a');
            alku.setRivi(syote[1] - '0' - 1);
            loppu.setSarake(syote[3] - 'a');
            loppu.setRivi(syote[4] - '0' - 1);
            
            if (alku.ok() && loppu.ok())
            {
                Siirto siirto(alku, loppu);
                
                return siirto;
            }
        }

        // UCI-protokollaan sopiva korotus
        // Korotus muodossa siirto + mihin korotetaan. Esim. h7-h8d
        else if (syote.length() == 6)
        {
            Ruutu alku(-1, -1);
            Ruutu loppu(-1, -1);

            char korotuskirjain = 'i';

            alku.setSarake(syote[0] - 'a');
            alku.setRivi(syote[1] - '0' - 1);
            loppu.setSarake(syote[3] - 'a');
            loppu.setRivi(syote[4] - '0' - 1);

            // Mihin korotetaan otetaan syötteen vikasta merkistä.
            korotuskirjain = syote[5];

            Nappula* korotus = nullptr;
            // Tarkistetaan että siirrettävä nappula on sotilas. 
            // En vielä testannut mitä tapahtuu jos yrittää korottaa muuta nappulaa. Testaan huomenna.
            Nappula* nappula = peli.asema.lauta[alku.getRivi()][alku.getSarake()];
            if ((alku.ok() && loppu.ok()) && (nappula == &Asema::ms || nappula == &Asema::vs))
            {
                Siirto siirto(alku, loppu);

                // BUGI: Mustan korotukset korottaa valkoisiksi. Katson huomenna.
                korotus = tarkistaNappula(korotuskirjain);
                if (korotus != nullptr && korotus != &Asema::ms && korotus != &Asema::vs)
                {
                    siirto.miksiKorotetaan = korotus;

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
    
    if (komento == "/siirrot")
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
    else if (komento == "/fen")
    {
        
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
