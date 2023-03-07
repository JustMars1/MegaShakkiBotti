#include "peli.h"
#include <sstream>
#include "kayttoliittyma.h"

std::optional<Peli> Peli::lue(std::string fen)
{
    auto& kayttoliittyma = Kayttoliittyma::getInstance();
    std::stringstream syote(fen);
    
    // Lauta
    std::string lauta;
    if (!std::getline(syote, lauta, ' '))
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (lauta puuttuu)");
        return std::nullopt;
    }
    
    Asema asema({nullptr});
    asema.setMustaDTliikkunut(true);
    asema.setMustaKTliikkunut(true);
    asema.setMustaKuningasLiikkunut(true);
    asema.setValkeaDTliikkunut(true);
    asema.setValkeaKTliikkunut(true);
    asema.setValkeaKuningasLiikkunut(true);
    
    int x = 0, y = 0;
    
    for (char kirjain : lauta)
    {
        if (isdigit(kirjain))
        {
            x += kirjain - '0';
        }
        else if (kirjain != '/')
        {
            auto nappulaItr = std::find_if(Asema::nappulat.begin(), Asema::nappulat.end(), [kirjain](auto nappula) {
                return nappula->getFENMerkki()[0] == kirjain;
            });
            
            if (nappulaItr == Asema::nappulat.end())
            {
                kayttoliittyma.tulostaVirhe("Virheellinen FEN (lauta)");
                return std::nullopt;
            }
            
            int rivi = 7 - y;
            
            if (*nappulaItr == &Asema::vk)
            {
                asema.setValkeanKuninkaanRuutu(Ruutu(x, rivi));
            }
            else if (*nappulaItr == &Asema::mk)
            {
                asema.setMustanKuninkaanRuutu(Ruutu(x, rivi));
            }
            
            asema.lauta[rivi][x] = *nappulaItr;
            x++;
        }
        
        y += x / 8;
        x %= 8;
    }
    
    // Vuoro
    std::string vuoro = "";
    if (!std::getline(syote, vuoro, ' '))
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (vuoro puuttuu)");
        return std::nullopt;
    }
    
    if (vuoro == "w")
    {
        asema.setSiirtovuoro(0);
    }
    else if (vuoro == "b")
    {
        asema.setSiirtovuoro(1);
    }
    else
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (vuoro)");
        return std::nullopt;
    }
    
    // Linnoitus
    
    std::string linnoitus;
    if (!std::getline(syote, linnoitus, ' '))
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (linnoitus puuttuu)");
        return std::nullopt;
    }
    
    if (linnoitus != "-")
    {
        for (char kirjain : linnoitus)
        {
            switch (kirjain) {
                case 'K':
                {
                    asema.setValkeaKTliikkunut(false);
                    asema.setValkeaKuningasLiikkunut(false);
                    break;
                }
                case 'Q':
                {
                    asema.setValkeaDTliikkunut(false);
                    asema.setValkeaKuningasLiikkunut(false);
                    break;
                }
                case 'k':
                {
                    asema.setMustaKTliikkunut(false);
                    asema.setMustaKuningasLiikkunut(false);
                    break;
                }
                case 'q':
                {
                    asema.setMustaDTliikkunut(false);
                    asema.setMustaKuningasLiikkunut(false);
                    break;
                }
                default:
                {
                    kayttoliittyma.tulostaVirhe("Virheellinen FEN (linnoitus)");
                    return std::nullopt;
                }
            }
        }
    }
    
    // Ohestalyonti
    
    std::string ohestalyonti;
    if (!std::getline(syote, ohestalyonti, ' '))
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (ohestalyonti puuttuu)");
        return std::nullopt;
    }
    
    if (ohestalyonti != "-")
    {
        if (ohestalyonti.length() == 2)
        {
            int sarake = ohestalyonti[0] - 'a';
            if (sarake >= 0 && sarake < 7)
            {
                asema.setKaksoisaskelSarake(sarake);
            }
            else
            {
                kayttoliittyma.tulostaVirhe("Virheellinen FEN (ohestalyonti, sarake)");
                return std::nullopt;
            }
            
            int rivi = ohestalyonti[1] - '0';
            
            if (rivi < 0 || rivi > 7)
            {
                kayttoliittyma.tulostaVirhe("Virheellinen FEN (ohestalyonti, rivi)");
                return std::nullopt;
            }
        }
    }
    
    // Siirtolaskuri
    int siirtoaSyonnistaTaiSotilaanSiirrosta = -1;
    syote >> siirtoaSyonnistaTaiSotilaanSiirrosta;
    if (syote.fail())
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (siirtolaskuri)");
        return std::nullopt;
    }
    
    if (siirtoaSyonnistaTaiSotilaanSiirrosta < 0)
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (siirtolaskuri)");
        return std::nullopt;
    }
    
    // Siirtoparilaskuri
    int siirtoparilaskuri = -1;
    syote >> siirtoparilaskuri;
    
    if (syote.fail())
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (vuorolaskuri)");
        return std::nullopt;
    }
    
    if (siirtoparilaskuri < 0)
    {
        kayttoliittyma.tulostaVirhe("Virheellinen FEN (vuorolaskuri)");
        return std::nullopt;
    }
    
    return Peli(Pelaaja(false, 0), Pelaaja(true, 4), asema);
}

Peli::Peli(Pelaaja valkoinen, Pelaaja musta, Asema aloitusAsema)
: valkoinen(valkoinen)
, musta(musta)
, _aloitusAsema(aloitusAsema)
, asema(aloitusAsema) {}

Peli::Peli() : Peli(Pelaaja(false, 0), Pelaaja(true, 4), Asema()) {}

const Asema& Peli::getAloitusAsema()
{
    return _aloitusAsema;
}
