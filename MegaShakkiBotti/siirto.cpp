#include "siirto.h"
#include <algorithm>
#include <sstream>
#include "kayttoliittyma.h"

/*
 Aliohjelma tarkistaa ett� k�ytt�j�n antama sy�te siirroksi on
 muodollisesti korrekti (ei tarkista aseman laillisuutta)
 
 UCI:
 Ottaa irti myös korotettaessa nappulan kirjaimen (D/L/R/T), tarkistaa että kirjain korrekti
 
 Ei UCI:
 Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
 */
std::optional<Siirto> Siirto::lue(std::string merkinta, int vari)
{
    auto& kayttoliittyma = Kayttoliittyma::getInstance();
    auto tarkistaNappula = [vari](std::string merkki) -> Nappula*
    {
        transform(merkki.begin(), merkki.end(), merkki.begin(), ::tolower);
        
        for (auto nappula : Asema::nappulat)
        {
            if (nappula->getVari() == vari)
            {
                std::string siirtoMerkki = nappula->getSiirtoMerkki();
                transform(siirtoMerkki.begin(), siirtoMerkki.end(), siirtoMerkki.begin(), ::tolower);
                
                if (siirtoMerkki == merkki)
                {
                    return nappula;
                }
            }
        }
        
        return nullptr;
    };
    
    if (kayttoliittyma.getOnkoUCI())
    {
        bool pitkaLinna = false;
        bool lyhytLinna = false;
        
        if (merkinta == "e1c1" || merkinta == "e8c8")
        {
            pitkaLinna = true;
        }
        else if (merkinta == "e1g1" || merkinta == "e8g8")
        {
            lyhytLinna = true;
        }
        
        if (pitkaLinna || lyhytLinna)
        {
            return Siirto(lyhytLinna, pitkaLinna);
        }
        
        if (merkinta.length() < 4)
        {
            kayttoliittyma.tulostaVirhe("Tuntematon siirto");
            return std::nullopt;
        }
        
        Ruutu alku(-1, -1);
        Ruutu loppu(-1, -1);
        
        alku.setSarake(merkinta[0] - 'a');
        alku.setRivi(merkinta[1] - '0' - 1);
        
        loppu.setSarake(merkinta[2] - 'a');
        loppu.setRivi(merkinta[3] - '0' - 1);
        
        if (!alku.ok() || !loppu.ok())
        {
            kayttoliittyma.tulostaVirhe("Virheelliset ruudut");
            return std::nullopt;
        }
        
        Siirto siirto(alku, loppu);
        
        if (merkinta.length() > 4)
        {
            std::string korotusMerkki = merkinta.substr(4);
            Nappula* korotusNappula = tarkistaNappula(korotusMerkki);
            
            if (korotusNappula == nullptr || korotusNappula == &Asema::vs || korotusNappula == &Asema::ms)
            {
                kayttoliittyma.tulostaVirhe("Siirron lopussa on virheellisia merkkeja.");
                return std::nullopt;
            }
            
            siirto.miksiKorotetaan = korotusNappula;
        }
        
        return siirto;
    }
    else
    {
        bool pitkaLinna = merkinta == "O-O-O";
        bool lyhytLinna = merkinta == "O-O";
        
        if (pitkaLinna || lyhytLinna)
        {
            return Siirto(lyhytLinna, pitkaLinna);
        }
        
        if (merkinta.length() < 6)
        {
            kayttoliittyma.tulostaVirhe("Tuntematon siirto");
            return std::nullopt;
        }
        
        auto viivaIdx = merkinta.find('-');
        
        if (viivaIdx == std::string::npos)
        {
            kayttoliittyma.tulostaVirhe("Siirrosta puuttuu -");
            return std::nullopt;
        }
        
        if (viivaIdx + 2 > merkinta.length() - 1 || viivaIdx - 2 < 1)
        {
            kayttoliittyma.tulostaVirhe("Virheellinen siirto");
            return std::nullopt;
        }
        
        Ruutu alku(-1, -1);
        Ruutu loppu(-1, -1);
        
        alku.setSarake(merkinta[viivaIdx - 2] - 'a');
        alku.setRivi(merkinta[viivaIdx - 1] - '0' - 1);
        
        loppu.setSarake(merkinta[viivaIdx + 1] - 'a');
        loppu.setRivi(merkinta[viivaIdx + 2] - '0' - 1);
        
        std::string siirtoMerkki = merkinta.substr(0, viivaIdx - 2);
        Nappula* siirtoNappula = tarkistaNappula(siirtoMerkki);
        
        if (siirtoNappula == nullptr)
        {
            kayttoliittyma.tulostaVirhe("Siirron alussa ei mainita nappulaa.");
            return std::nullopt;
        }
        
        if (!alku.ok() || !loppu.ok())
        {
            kayttoliittyma.tulostaVirhe("Virheelliset ruudut");
            return std::nullopt;
        }
        
        Siirto siirto(alku, loppu);
        
        if (merkinta.length() - 1 > viivaIdx + 2)
        {
            std::string korotusMerkki = merkinta.substr(viivaIdx + 2 + 1);
            Nappula* korotusNappula = tarkistaNappula(korotusMerkki);
            
            if (korotusNappula == nullptr || korotusNappula == &Asema::vs || korotusNappula == &Asema::ms)
            {
                kayttoliittyma.tulostaVirhe("Siirron lopussa on virheellisia merkkeja.");
                return std::nullopt;
            }
            
            if (siirtoNappula != &Asema::vs && siirtoNappula != &Asema::ms)
            {
                kayttoliittyma.tulostaVirhe("Vain sotilas voidaan korottaa.");
                return std::nullopt;
            }
            
            siirto.miksiKorotetaan = korotusNappula;
        }
        
        return siirto;
    }
}

Siirto::Siirto(Ruutu alku, Ruutu loppu)
: _alkuRuutu(alku)
, _loppuRuutu(loppu)
, _lyhytLinna{false}
, _pitkaLinna{false}
, miksiKorotetaan{nullptr} {}

Siirto::Siirto(bool lyhytLinna, bool pitkaLinna)
: _lyhytLinna{lyhytLinna}
, _pitkaLinna{pitkaLinna}
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksiKorotetaan{nullptr} {}

Siirto::Siirto()
: _lyhytLinna{ false }
, _pitkaLinna{ false }
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksiKorotetaan{ nullptr } {}

bool Siirto::operator==(const Siirto& rhs) const
{
    return _alkuRuutu == rhs._alkuRuutu &&
    _loppuRuutu == rhs._loppuRuutu &&
    _lyhytLinna == rhs._lyhytLinna &&
    _pitkaLinna == rhs._pitkaLinna &&
    miksiKorotetaan == rhs.miksiKorotetaan;
}

const Ruutu& Siirto::getAlkuruutu() const { return _alkuRuutu; }
const Ruutu& Siirto::getLoppuruutu() const { return _loppuRuutu; }
bool Siirto::onkoLyhytLinna() const { return _lyhytLinna; }
bool Siirto::onkoPitkaLinna() const { return _pitkaLinna; }

Nappula* Siirto::getNappula(const Asema& alkuAsema) const
{
    return _lyhytLinna || _pitkaLinna ? nullptr : _alkuRuutu.getNappula(alkuAsema);
}

std::string Siirto::getMerkinta(const Asema& asema) const
{
    std::stringstream merkinta;
    Nappula* nappula = getNappula(asema);
    
    if (nappula != nullptr)
    {
        merkinta << nappula->getLautaMerkki() << " ";
        if (!Kayttoliittyma::getInstance().getOnkoUCI())
        {
            std::string merkki = nappula->getSiirtoMerkki();
            transform(merkki.begin(), merkki.end(), merkki.begin(), ::toupper);
            merkinta << merkki;
        }
    }
    
    merkinta << *this;
    
    return merkinta.str();
}

std::ostream& operator<<(std::ostream& os, const Siirto& siirto)
{
    bool uci = Kayttoliittyma::getInstance().getOnkoUCI();
    
    if (siirto.onkoPitkaLinna())
    {
        os << (uci ? "e1c1" : "O-O-O");
    }
    else if (siirto.onkoPitkaLinna())
    {
        os << (uci ? "e8c8" : "O-O-O");
    }
    else if (siirto.onkoLyhytLinna())
    {
        os << (uci ? "e1g1" : "O-O");
    }
    else if (siirto.onkoLyhytLinna())
    {
        os << (uci ? "e8g8" : "O-O");
    }
    else
    {
        os << siirto.getAlkuruutu() << (uci ? "" : "-") << siirto.getLoppuruutu();
        if (siirto.miksiKorotetaan != nullptr)
        {
            std::string merkki = siirto.miksiKorotetaan->getSiirtoMerkki();
            std::transform(merkki.begin(), merkki.end(), merkki.begin(), (uci ? ::tolower : ::toupper));
            os << merkki;
        }
    }
    
    return os;
}
