#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <future>

#include "asema.h"
#include "minmaxpaluu.h"
#include "nappula.h"
#include "ruutu.h"
#include "kayttoliittyma.h"

Kuningas Asema::vk = Kuningas("\xe2\x99\x94", 0, VK, 0.0f, 8);
Daami Asema::vd = Daami(      "\xe2\x99\x95", 0, VD, 9.0f, 27);
Torni Asema::vt = Torni(      "\xe2\x99\x96", 0, VT, 5.0f, 14);
Lahetti Asema::vl = Lahetti(  "\xe2\x99\x97", 0, VL, 3.25f, 13);
Ratsu Asema::vr = Ratsu(      "\xe2\x99\x98", 0, VR, 3.0f, 8);
Sotilas Asema::vs = Sotilas(  "\xe2\x99\x99", 0, VS, 1.0f, 4);

Kuningas Asema::mk = Kuningas("\xe2\x99\x9a", 1, MK, 0.0f, 8);
Daami Asema::md = Daami(      "\xe2\x99\x9b", 1, MD, -9.0f, 27);
Torni Asema::mt = Torni(      "\xe2\x99\x9c", 1, MT, -5.0f, 14);
Lahetti Asema::ml = Lahetti(  "\xe2\x99\x9d", 1, ML, -3.25f, 13);
Ratsu Asema::mr = Ratsu(      "\xe2\x99\x9e", 1, MR, -3.0f, 8);
Sotilas Asema::ms = Sotilas(  "\xe2\x99\x9f", 1, MS, -1.0f, 4);

const std::unordered_map<char, Nappula*> Asema::valkeaNappulaMap =
{
    {'k', &vk},
    {'d', &vd},
    {'t', &vt},
    {'l', &vl},
    {'r', &vr},
    {'s', &vs}
};

const std::unordered_map<char, Nappula*> Asema::mustaNappulaMap =
{
    {'k', &mk},
    {'d', &md},
    {'t', &mt},
    {'l', &ml},
    {'r', &mr},
    {'s', &ms}
};

const std::unordered_map<char, Nappula*> Asema::fenNappulaMap =
{
    {'r', &mt},
    {'n', &mr},
    {'b', &ml},
    {'q', &md},
    {'k', &mk},
    {'p', &ms},
    {'R', &vt},
    {'N', &vr},
    {'B', &vl},
    {'Q', &vd},
    {'K', &vk},
    {'P', &vs}
};

const float Asema::maxArvo = vd.getArvo() + vt.getArvo() * 2 + vl.getArvo() * 2 + vr.getArvo() * 2 + vs.getArvo() * 8;

Asema::Asema()
: lauta
{
    &vt, &vr, &vl, &vd, &vk, &vl, &vr, &vt,
    &vs, &vs, &vs, &vs, &vs, &vs, &vs, &vs,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    &ms, &ms, &ms, &ms, &ms, &ms, &ms, &ms,
    &mt, &mr, &ml, &md, &mk, &ml, &mr, &mt
}
, _siirtovuoro{0}
, _onkoValkeaKuningasLiikkunut{false}
, _onkoMustaKuningasLiikkunut{false}
, _onkoValkeaDTliikkunut{false}
, _onkoValkeaKTliikkunut{false}
, _onkoMustaDTliikkunut{false}
, _onkoMustaKTliikkunut{false}
, _valkeaKuningasRuutu(4, 0)
, _mustaKuningasRuutu(4, 7)
, kaksoisaskel{-1} {}

void Asema::paivitaAsema(const Siirto& siirto)
{
    // Tarkastetaan on siirto lyhyt linna tai pitkä linna
    
    if (siirto.onkoLyhytLinna())
    {
        if (_siirtovuoro == 0)
        {
            _onkoValkeaKTliikkunut = true;
            _onkoValkeaKuningasLiikkunut = true;
            
            lauta[0][5] = &Asema::vt;
            lauta[0][6] = &Asema::vk;
            lauta[0][4] = nullptr;
            lauta[0][7] = nullptr;
            
            _valkeaKuningasRuutu.setSarake(6);
        }
        else
        {
            _onkoMustaKTliikkunut = true;
            _onkoMustaKuningasLiikkunut = true;
            
            lauta[7][5] = &Asema::mt;
            lauta[7][6] = &Asema::mk;
            lauta[7][7] = nullptr;
            lauta[7][4] = nullptr;
            
            _mustaKuningasRuutu.setSarake(6);
        }
    }
    else if (siirto.onkoPitkaLinna())
    {
        if (_siirtovuoro == 0)
        {
            _onkoValkeaDTliikkunut = true;
            _onkoValkeaKuningasLiikkunut = true;
            
            lauta[0][3] = &Asema::vt;
            lauta[0][2] = &Asema::vk;
            lauta[0][0] = nullptr;
            lauta[0][4] = nullptr;
            
            _valkeaKuningasRuutu.setSarake(2);
        }
        else
        {
            _onkoMustaDTliikkunut = true;
            _onkoMustaKuningasLiikkunut = true;
            
            lauta[7][3] = &Asema::mt;
            lauta[7][2] = &Asema::mk;
            lauta[7][0] = nullptr;
            lauta[7][4] = nullptr;
            
            _mustaKuningasRuutu.setSarake(2);
        }
    }
    else
    {
        int8_t alkuX = siirto.getAlkuruutu().getSarake();
        int8_t alkuY = siirto.getAlkuruutu().getRivi();
        
        int8_t loppuX = siirto.getLoppuruutu().getSarake();
        int8_t loppuY = siirto.getLoppuruutu().getRivi();
        
        Nappula* nappula = lauta[alkuY][alkuX];
        
        if (nappula == &Asema::vk)
        {
            _onkoValkeaKuningasLiikkunut = true;
            _valkeaKuningasRuutu.setSarake(loppuX);
            _valkeaKuningasRuutu.setRivi(loppuY);
        }
        else if (nappula == &Asema::mk)
        {
            _onkoMustaKuningasLiikkunut = true;
            _mustaKuningasRuutu.setSarake(loppuX);
            _mustaKuningasRuutu.setRivi(loppuY);
        }
        else if (nappula == &Asema::vt)
        {
            if (alkuX == 0 && !_onkoValkeaDTliikkunut)
            {
                _onkoValkeaDTliikkunut = true;
            }
            else if (alkuX == 7 && !_onkoValkeaKTliikkunut)
            {
                _onkoValkeaKTliikkunut = true;
            }
        }
        else if (nappula == &Asema::mt)
        {
            if (alkuX == 0 && !_onkoMustaKTliikkunut)
            {
                _onkoMustaKTliikkunut = true;
            }
            else if (alkuX == 7 && !_onkoMustaDTliikkunut)
            {
                _onkoMustaDTliikkunut = true;
            }
        }
        else if (nappula == &Asema::vs)
        {
            if (alkuY == 1 && loppuY == 3)
            {
                kaksoisaskel = loppuX;
            }
            else if (kaksoisaskel != -1 && lauta[loppuY][loppuX] == lauta[5][kaksoisaskel])
            {
                // Valkosen sotilaan ohestalyönti
                if (lauta[3][kaksoisaskel] != nullptr && lauta[3][kaksoisaskel] == &Asema::ms)
                {
                    lauta[loppuY - 1][kaksoisaskel] = nullptr;
                    kaksoisaskel = -1;
                }
            }
        }
        else if (nappula == &Asema::ms)
        {
            if (alkuY == 6 && loppuY == 4)
            {
                kaksoisaskel = loppuX;
            }
            else if (kaksoisaskel != -1 && lauta[loppuY][loppuX] == lauta[2][kaksoisaskel])
            {
                if (lauta[4][kaksoisaskel] != nullptr && lauta[4][kaksoisaskel] == &Asema::vs)
                {
                    // Mustan sotilaan ohestalyönti
                    lauta[loppuY + 1][kaksoisaskel] = nullptr;
                    kaksoisaskel = -1;
                }
            }
        }
        
        if (nappula != &Asema::vs && nappula != &Asema::ms)
        {
            kaksoisaskel = -1;
        }
        
        if (siirto.miksiKorotetaan != nullptr)
        {
            lauta[loppuY][loppuX] = siirto.miksiKorotetaan;
        }
        else
        {
            lauta[loppuY][loppuX] = nappula;
        }
        
        lauta[alkuY][alkuX] = NULL;
    }
    
    _siirtovuoro = 1 - _siirtovuoro;
}

bool Asema::tarkistaSiirto(const Siirto& siirto) const
{
    if (siirto.onkoLyhytLinna())
    {
        if (_siirtovuoro == 0)
        {
            if (onkoValkeaKuningasLiikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoValkeaKTliikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[0][5] != nullptr || lauta[0][6] != nullptr)
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
                return false;
            }
        }
        else
        {
            if (onkoMustaKuningasLiikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoMustaKTliikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[7][5] != nullptr || lauta[7][6] != nullptr)
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
                return false;
            }
        }
    }
    else if (siirto.onkoPitkaLinna())
    {
        if (_siirtovuoro == 0)
        {
            if (onkoValkeaKuningasLiikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoValkeaDTliikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[0][3] != nullptr || lauta[0][2] != nullptr || lauta[0][1] != nullptr)
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
                return false;
            }
        }
        else
        {
            if (onkoMustaKuningasLiikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoMustaDTliikkunut())
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[7][3] != nullptr || lauta[7][2] != nullptr || lauta[7][1] != nullptr)
            {
                Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
                return false;
            }
        }
    }
    else
    {
        Ruutu alku = siirto.getAlkuruutu();
        Ruutu loppu = siirto.getLoppuruutu();
        
        if (!alku.ok())
        {
            Kayttoliittyma::tulostaVirhe("Siirron alkuruutu on laudan ulkopuolella.");
        }
        
        if (!loppu.ok())
        {
            Kayttoliittyma::tulostaVirhe("Siirron loppuruutu on laudan ulkopuolella.");
        }
        
        Nappula* nappula = lauta[alku.getRivi()][alku.getSarake()];
        
        if (nappula == nullptr)
        {
            Kayttoliittyma::tulostaVirhe("Siirron alkuruudussa ei ole nappulaa.");
            return false;
        }
        
        if (nappula->getVari() != _siirtovuoro)
        {
            Kayttoliittyma::tulostaVirhe("Et voi siirtää vastustajan nappulaa.");
            return false;
        }
    }
    
    std::vector<Siirto> siirrot;
    siirrot.reserve(50);
    annaLaillisetSiirrot(siirrot);
    
    if (std::find(siirrot.begin(), siirrot.end(), siirto) != siirrot.end())
    {
        return true;
    }
    else
    {
        Kayttoliittyma::tulostaVirhe("Siirto ei ole laillinen.");
        return false;
    }
}

int Asema::getSiirtovuoro() const { return _siirtovuoro; }
void Asema::setSiirtovuoro(int vuoro)
{
    _siirtovuoro = vuoro;
}

bool Asema::onkoValkeaKuningasLiikkunut() const { return _onkoValkeaKuningasLiikkunut; }
bool Asema::onkoMustaKuningasLiikkunut() const { return _onkoMustaKuningasLiikkunut; }
bool Asema::onkoValkeaDTliikkunut() const { return _onkoValkeaDTliikkunut; }
bool Asema::onkoValkeaKTliikkunut() const { return _onkoValkeaKTliikkunut; }
bool Asema::onkoMustaDTliikkunut() const { return _onkoMustaDTliikkunut; }
bool Asema::onkoMustaKTliikkunut() const { return _onkoMustaKTliikkunut; }

void Asema::setValkeaKuningasLiikkunut(bool valkeaKuningasLiikkunut)
{
    _onkoValkeaKuningasLiikkunut = valkeaKuningasLiikkunut;
};

void Asema::setMustaKuningasLiikkunut(bool mustaKuningasLiikkunut)
{
    _onkoMustaKuningasLiikkunut = mustaKuningasLiikkunut;
}

void Asema::setValkeaDTliikkunut(bool valkeaDTliikkunut)
{
    _onkoValkeaDTliikkunut = valkeaDTliikkunut;
}

void Asema::setValkeaKTliikkunut(bool valkeaKTliikkunut)
{
    _onkoValkeaKTliikkunut = valkeaKTliikkunut;
}

void Asema::setMustaDTliikkunut(bool mustaDTliikkunut)
{
    _onkoMustaDTliikkunut = mustaDTliikkunut;
}

void Asema::setMustaKTliikkunut(bool mustaKTliikkunut)
{
    _onkoMustaKTliikkunut = mustaKTliikkunut;
}

/* 1. Laske nappuloiden arvo
 Daami = 9
 Torni = 5
 Lähetti = 3,25
 Ratsu = 3
 Sotilas = 1
 
 2. Kuninkaan hyvyys
 Jos avaus tai keskipeli, niin hyvä että kunigas g1 tai b1/c1
 Loppupelissä vaikea sanoa halutaanko olla auttamassa omaa sotilasta korottumaan
 vai olla estämässä vastustajan korotusta siksi ei oteta kantaa
 3. Arvosta keskustaa sotilailla ja ratsuilla
 4. Arvosta pitkiä linjoja daami, torni ja lähetti
 */
float Asema::evaluoi() const
{
    float summa = laskeNappuloidenArvo();
    return summa;
    
    //kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta
    
    //1. Nappuloiden arvo
    
    //2. Kuningas turvassa
    
    //3. Arvosta keskustaa
    
    // 4. Arvosta linjoja
    
}

float Asema::laskeNappuloidenArvo() const
{
    float summa = 0;
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            if (nappula == nullptr)
            {
                continue;
            }
            
            summa += nappula->getArvo();
        }
    }
    
    return summa / maxArvo;
}

bool Asema::onkoAvausTaiKeskipeli(int vari)
{
    int mustanUpseerit = 0;
    int valkoisenUpseerit = 0;
    
    bool mustanDaami = false;
    bool valkoisenDaami = false;
    
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            
            if (nappula == &vd)
            {
                valkoisenDaami = true;
                valkoisenUpseerit++;
            }
            if (nappula == &md)
            {
                mustanDaami = true;
                mustanUpseerit++;
            }
            
            switch (nappula->getKoodi())
            {
                case VT:
                case MT:
                case VL:
                case ML:
                case VR:
                case MR:
                    mustanUpseerit++;
                    break;
                default:
                    break;
            }
        }
    }
    
    if (valkoisenUpseerit + mustanUpseerit < 4 && (mustanDaami || valkoisenDaami))
    {
        return false;
    }
    //
    //
    //    if (vari == 0)
    //    {
    //        return valkoisenUpseerit < 4 || mustanUpseerit > 2 || (mustanDaami && mustanUpseerit > 1);
    //    }
    //    else {
    //        return mustanUpseerit < 4;
    //    }
    
    // Jos upseereita 3 tai vähemmän on loppupeli
    // mutta jos daami laudalla on loppueli vasta kun kuin vain daami jäljellä
    
    //Jos vari on 0 eli valkoiset
    //niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1
    
    return false;
}

float Asema::nappuloitaKeskella(int vari)
{
    return 0;
    
    //sotilaat ydinkeskustassa + 0.25/napa
    //ratsut ydinkeskustassa + 0.25/napa
    //sotilaat laitakeskustassa + 0.11/napa
    //ratsut laitakeskustassa + 0.11/napa
    
    //valkeille ydinkeskusta
    
    //valkeille laitakeskusta
    
    //mustille ydinkeskusta
    
    //mustille laitakeskusta
    
}

float Asema::linjat(int vari)
{
    return 0;
    
    //valkoiset
    
    //mustat
    
}

MinMaxPaluu Asema::minimax(int syvyys) const
{
    if (_siirtovuoro == 0)
    {
        return maxi(syvyys);
    }
    else
    {
        return mini(syvyys);
    }
}

MinMaxPaluu Asema::minimaxAsync(int syvyys) const
{
    auto miniAsync = [this, syvyys](Siirto& siirto) -> MinMaxPaluu 
    { 
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        MinMaxPaluu arvo = tmpAsema.maxi(syvyys - 1);
        arvo._parasSiirto = siirto;
        return arvo;
    };
    auto maxiAsync = [this, syvyys](Siirto& siirto) -> MinMaxPaluu 
    {  
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        MinMaxPaluu arvo = tmpAsema.mini(syvyys - 1);
        arvo._parasSiirto = siirto;
        return arvo;
    };

    std::vector<Siirto> siirrot;
    siirrot.reserve(50);
    annaLaillisetSiirrot(siirrot);
    std::vector<std::future<MinMaxPaluu>> tehtavat;
    tehtavat.reserve(siirrot.size());

    if (_siirtovuoro == 0)
    {
        MinMaxPaluu maxi;
        maxi.evaluointiArvo = std::numeric_limits<float>::lowest();

        for (Siirto& siirto : siirrot)
        {
            auto tehtava = std::bind(maxiAsync, siirto);
            tehtavat.push_back(std::async(std::launch::async, tehtava));
        }
        for (auto& tehtava : tehtavat)
        {
            MinMaxPaluu arvo = tehtava.get();
            if (arvo.evaluointiArvo > maxi.evaluointiArvo)
            {
                maxi = arvo;
            }
        }
        return maxi;
    }
    else
    {
        MinMaxPaluu mini;
        mini.evaluointiArvo = std::numeric_limits<float>::max();

        for (Siirto& siirto : siirrot)
        {
            auto tehtava = std::bind(miniAsync, siirto);
            tehtavat.push_back(std::async(std::launch::async, tehtava));
        }

        for (auto& tehtava : tehtavat)
        {
            MinMaxPaluu arvo = tehtava.get();
            if (arvo.evaluointiArvo < mini.evaluointiArvo)
            {
                mini = arvo;
            }
        }
        return mini;
    }
}


MinMaxPaluu Asema::maxi(int syvyys) const
{
    if (syvyys == 0)
    {
        return MinMaxPaluu(evaluoi(), Siirto());
    }
    
    MinMaxPaluu maxi;
    maxi.evaluointiArvo = std::numeric_limits<float>::lowest();
    
    std::vector<Siirto> siirrot;
    siirrot.reserve(50);
    annaLaillisetSiirrot(siirrot);
    
    if (siirrot.empty())
    {
        if (onkoRuutuUhattu(_valkeaKuningasRuutu, 1))
        {
            return MinMaxPaluu(-1, Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    for (auto& siirto : siirrot)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        MinMaxPaluu arvo = tmpAsema.mini(syvyys - 1);
        arvo._parasSiirto = siirto;
        if (arvo.evaluointiArvo > maxi.evaluointiArvo)
        {
            maxi = arvo;
        }
    }
    return maxi;
}

MinMaxPaluu Asema::mini(int syvyys) const
{
    if (syvyys == 0)
    {
        return MinMaxPaluu(evaluoi(), Siirto());
    }
    
    MinMaxPaluu mini;
    mini.evaluointiArvo = std::numeric_limits<float>::max();
    
    std::vector<Siirto> siirrot;
    siirrot.reserve(50);
    annaLaillisetSiirrot(siirrot);
    
    if (siirrot.empty())
    {
        if (onkoRuutuUhattu(_mustaKuningasRuutu, 0))
        {
            return MinMaxPaluu(1, Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    for (auto& siirto : siirrot)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        MinMaxPaluu arvo = tmpAsema.maxi(syvyys - 1);
        arvo._parasSiirto = siirto;
        if (arvo.evaluointiArvo < mini.evaluointiArvo)
        {
            mini = arvo;
        }
    }
    return mini;
}

bool Asema::onkoRuutuUhattu(const Ruutu& ruutu, int vastustajanVari) const
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            if (nappula != nullptr && nappula->getVari() == vastustajanVari)
            {
                std::vector<Siirto> vastustajanSiirrot;
                vastustajanSiirrot.reserve(nappula->getMaxSiirrot());
                
                nappula->annaSiirrot(vastustajanSiirrot, Ruutu(x, y), *this, vastustajanVari);
                
                for (auto& siirto : vastustajanSiirrot)
                {
                    if (siirto.getLoppuruutu() == ruutu)
                    {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

void Asema::annaLinnoitusSiirrot(std::vector<Siirto>& siirrot, int vari) const
{
    if (vari == 0) {
        // valkea, lyhyt linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaKTliikkunut()
            && !onkoRuutuUhattu(Ruutu(4, 0), 1)
            && !onkoRuutuUhattu(Ruutu(6, 0), 1)
            && !onkoRuutuUhattu(Ruutu(5, 0), 1)
            && lauta[0][5] == NULL
            && lauta[0][6] == NULL)
        {
            siirrot.push_back(Siirto(true, false));
        }
        
        // valkea, pitkä linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaDTliikkunut()
            && !onkoRuutuUhattu(Ruutu(4, 0), 1)
            && !onkoRuutuUhattu(Ruutu(3, 0), 1)
            && !onkoRuutuUhattu(Ruutu(2, 0), 1)
            && lauta[0][1] == NULL
            && lauta[0][2] == NULL
            && lauta[0][3] == NULL)
        {
            siirrot.push_back(Siirto(false, true));
        }
    }
    
    if (vari == 1)
    {
        // musta, lyhyt linna
        if (!onkoMustaKuningasLiikkunut() && !onkoMustaKTliikkunut()
            && !onkoRuutuUhattu(Ruutu(4, 7), 0)
            && !onkoRuutuUhattu(Ruutu(5, 7), 0)
            && !onkoRuutuUhattu(Ruutu(6, 7), 0)
            && lauta[7][5] == NULL
            && lauta[7][6] == NULL)
        {
            siirrot.push_back(Siirto(true, false));
        }
        
        // musta, pitkä linna
        if (!onkoMustaKuningasLiikkunut() && !onkoMustaDTliikkunut()
            && !onkoRuutuUhattu(Ruutu(4, 7), 0)
            && !onkoRuutuUhattu(Ruutu(3, 7), 0)
            && !onkoRuutuUhattu(Ruutu(2, 7), 0)
            && lauta[7][1] == NULL
            && lauta[7][2] == NULL
            && lauta[7][3] == NULL)
        {
            siirrot.push_back(Siirto(false, true));
        }
    }
}

void Asema::huolehdiKuninkaanShakeista(std::vector<Siirto>& siirrot) const
{
    std::vector<Siirto> laillisetSiirrot;
    laillisetSiirrot.reserve(siirrot.size());
    
    for (auto& siirto : siirrot) {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        
        if (_siirtovuoro == 0)
        {
            if (tmpAsema.onkoRuutuUhattu(tmpAsema._valkeaKuningasRuutu, 1))
            {
                continue;
            }
        }
        else if (tmpAsema.onkoRuutuUhattu(tmpAsema._mustaKuningasRuutu, 0))
        {
            continue;
        }
        
        laillisetSiirrot.push_back(siirto);
    }
    
    siirrot = laillisetSiirrot;
}

void Asema::annaLaillisetSiirrot(std::vector<Siirto>& siirrot) const
{
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            if (nappula != nullptr && nappula->getVari() == getSiirtovuoro())
            {
                nappula->annaSiirrot(siirrot, Ruutu(x, y), *this, nappula->getVari());
            }
        }
    }
    
    annaLinnoitusSiirrot(siirrot, _siirtovuoro);
    huolehdiKuninkaanShakeista(siirrot);
}
