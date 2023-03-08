#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <future>
#include <atomic>
#include <iterator>
#include <numeric>

#include "asema.h"
#include "minmaxpaluu.h"
#include "nappula.h"
#include "ruutu.h"
#include "kayttoliittyma.h"

Kuningas Asema::vk = Kuningas("\xe2\x99\x94", 0, VK,   0.0f,   8, "vk", "K");
Daami Asema::vd = Daami(      "\xe2\x99\x95", 0, VD,   9.0f,  27, "vd", "Q");
Torni Asema::vt = Torni(      "\xe2\x99\x96", 0, VT,   5.0f,  14, "vt", "R");
Lahetti Asema::vl = Lahetti(  "\xe2\x99\x97", 0, VL,  3.25f,  13, "vl", "B");
Ratsu Asema::vr = Ratsu(      "\xe2\x99\x98", 0, VR,   3.0f,   8, "vr", "N");
Sotilas Asema::vs = Sotilas(  "\xe2\x99\x99", 0, VS,   1.0f,   4, "vs", "P");
Kuningas Asema::mk = Kuningas("\xe2\x99\x9a", 1, MK,   0.0f,   8, "mk", "k");
Daami Asema::md = Daami(      "\xe2\x99\x9b", 1, MD,  -9.0f,  27, "md", "q");
Torni Asema::mt = Torni(      "\xe2\x99\x9c", 1, MT,  -5.0f,  14, "mt", "r");
Lahetti Asema::ml = Lahetti(  "\xe2\x99\x9d", 1, ML, -3.25f,  13, "ml", "b");
Ratsu Asema::mr = Ratsu(      "\xe2\x99\x9e", 1, MR,  -3.0f,   8, "mr", "n");
Sotilas Asema::ms = Sotilas(  "\xe2\x99\x9f", 1, MS,  -1.0f,   4, "ms", "p");

std::array<Nappula*, NappulaKoodiLkm> Asema::nappulat =
{
    &vt, &vr, &vl, &vd, &vk, &vs,
    &mt, &mr, &ml, &md, &mk, &ms
};

const float Asema::maxArvo = vd.getArvo() + vt.getArvo() * 2 + vl.getArvo() * 2 + vr.getArvo() * 2 + vs.getArvo() * 8;

Asema::Asema(std::array<std::array<Nappula*, 8>, 8> lauta)
: lauta(lauta)
, _siirtovuoro{0}
, _onkoValkeaKuningasLiikkunut{false}
, _onkoMustaKuningasLiikkunut{false}
, _onkoValkeaDTliikkunut{false}
, _onkoValkeaKTliikkunut{false}
, _onkoMustaDTliikkunut{false}
, _onkoMustaKTliikkunut{false}
, _valkeanKuninkaanRuutu(4, 0)
, _mustanKuninkaanRuutu(4, 7)
, _kaksoisaskelSarake{-1} {}

int Asema::getSiirtovuoro() const { return _siirtovuoro; }
void Asema::setSiirtovuoro(int vuoro)
{
    _siirtovuoro = vuoro;
}

Ruutu Asema::getValkeanKuninkaanRuutu() const
{
    return _valkeanKuninkaanRuutu;
}

Ruutu Asema::getMustanKuninkaanRuutu() const
{
    return _mustanKuninkaanRuutu;
}

int Asema::getKaksoisaskelSarake() const { return _kaksoisaskelSarake; }
void Asema::setKaksoisaskelSarake(int sarake)
{
    _kaksoisaskelSarake = sarake;
}

bool Asema::onkoValkeaKuningasLiikkunut() const { return _onkoValkeaKuningasLiikkunut; }
bool Asema::onkoMustaKuningasLiikkunut() const { return _onkoMustaKuningasLiikkunut; }
bool Asema::onkoValkeaDTliikkunut() const { return _onkoValkeaDTliikkunut; }
bool Asema::onkoValkeaKTliikkunut() const { return _onkoValkeaKTliikkunut; }
bool Asema::onkoMustaDTliikkunut() const { return _onkoMustaDTliikkunut; }
bool Asema::onkoMustaKTliikkunut() const { return _onkoMustaKTliikkunut; }

void Asema::setValkeaKuningasLiikkunut(bool onkoLiikkunut)
{
    _onkoValkeaKuningasLiikkunut = onkoLiikkunut;
};

void Asema::setMustaKuningasLiikkunut(bool onkoLiikkunut)
{
    _onkoMustaKuningasLiikkunut = onkoLiikkunut;
}

void Asema::setValkeaDTliikkunut(bool onkoLiikkunut)
{
    _onkoValkeaDTliikkunut = onkoLiikkunut;
}

void Asema::setValkeaKTliikkunut(bool onkoLiikkunut)
{
    _onkoValkeaKTliikkunut = onkoLiikkunut;
}

void Asema::setMustaDTliikkunut(bool onkoLiikkunut)
{
    _onkoMustaDTliikkunut = onkoLiikkunut;
}

void Asema::setMustaKTliikkunut(bool onkoLiikkunut)
{
    _onkoMustaKTliikkunut = onkoLiikkunut;
}

void Asema::setValkeanKuninkaanRuutu(const Ruutu& ruutu)
{
    _valkeanKuninkaanRuutu = ruutu;
}
void Asema::setMustanKuninkaanRuutu(const Ruutu& ruutu)
{
    _mustanKuninkaanRuutu = ruutu;
}

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
            
            _valkeanKuninkaanRuutu.setSarake(6);
        }
        else
        {
            _onkoMustaKTliikkunut = true;
            _onkoMustaKuningasLiikkunut = true;
            
            lauta[7][5] = &Asema::mt;
            lauta[7][6] = &Asema::mk;
            lauta[7][7] = nullptr;
            lauta[7][4] = nullptr;
            
            _mustanKuninkaanRuutu.setSarake(6);
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
            
            _valkeanKuninkaanRuutu.setSarake(2);
        }
        else
        {
            _onkoMustaDTliikkunut = true;
            _onkoMustaKuningasLiikkunut = true;
            
            lauta[7][3] = &Asema::mt;
            lauta[7][2] = &Asema::mk;
            lauta[7][0] = nullptr;
            lauta[7][4] = nullptr;
            
            _mustanKuninkaanRuutu.setSarake(2);
        }
    }
    else
    {
        int alkuX = siirto.getAlkuruutu().getSarake();
        int alkuY = siirto.getAlkuruutu().getRivi();
        
        int loppuX = siirto.getLoppuruutu().getSarake();
        int loppuY = siirto.getLoppuruutu().getRivi();
        
        Nappula* nappula = lauta[alkuY][alkuX];
        
        if (nappula == &Asema::vk)
        {
            _onkoValkeaKuningasLiikkunut = true;
            _valkeanKuninkaanRuutu.setSarake(loppuX);
            _valkeanKuninkaanRuutu.setRivi(loppuY);
        }
        else if (nappula == &Asema::mk)
        {
            _onkoMustaKuningasLiikkunut = true;
            _mustanKuninkaanRuutu.setSarake(loppuX);
            _mustanKuninkaanRuutu.setRivi(loppuY);
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
            if (alkuX == 0 && !_onkoMustaDTliikkunut)
            {
                _onkoMustaDTliikkunut = true;
            }
            else if (alkuX == 7 && !_onkoMustaKTliikkunut)
            {
                _onkoMustaKTliikkunut = true;
            }
        }
        else if (nappula == &Asema::vs)
        {
            if (alkuY == 1 && loppuY == 3)
            {
                _kaksoisaskelSarake = loppuX;
            }
            else
            {
                if (_kaksoisaskelSarake != -1 && loppuY == 5 && loppuX == _kaksoisaskelSarake)
                {
                    // Valkosen sotilaan ohestalyönti
                    lauta[loppuY - 1][_kaksoisaskelSarake] = nullptr;
                }
                _kaksoisaskelSarake = -1;
            }
        }
        else if (nappula == &Asema::ms)
        {
            if (alkuY == 6 && loppuY == 4)
            {
                _kaksoisaskelSarake = loppuX;
            }
            else
            {
                if (_kaksoisaskelSarake != -1 && loppuY == 2 && loppuX == _kaksoisaskelSarake)
                {
                    // Mustan sotilaan ohestalyönti
                    lauta[loppuY + 1][_kaksoisaskelSarake] = nullptr;
                }
                _kaksoisaskelSarake = -1;
            }
        }
        
        if (nappula != &Asema::vs && nappula != &Asema::ms)
        {
            _kaksoisaskelSarake = -1;
        }
        
        if (siirto.miksiKorotetaan != nullptr)
        {
            lauta[loppuY][loppuX] = siirto.miksiKorotetaan;
        }
        else
        {
            lauta[loppuY][loppuX] = nappula;
        }
        
        lauta[alkuY][alkuX] = nullptr;
    }
    
    _siirtovuoro = 1 - _siirtovuoro;
}

bool Asema::tarkistaSiirto(const Siirto& siirto) const
{
    auto& kayttoliittyma = Kayttoliittyma::getInstance();
    if (siirto.onkoLyhytLinna())
    {
        if (_siirtovuoro == 0)
        {
            if (onkoValkeaKuningasLiikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoValkeaKTliikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[0][5] != nullptr || lauta[0][6] != nullptr)
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
                return false;
            }
        }
        else
        {
            if (onkoMustaKuningasLiikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoMustaKTliikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[7][5] != nullptr || lauta[7][6] != nullptr)
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
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
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoValkeaDTliikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[0][3] != nullptr || lauta[0][2] != nullptr || lauta[0][1] != nullptr)
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
                return false;
            }
        }
        else
        {
            if (onkoMustaKuningasLiikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, kuningas on jo liikkunut.");
                return false;
            }
            
            if (onkoMustaDTliikkunut())
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, torni on jo liikkunut.");
            }
            
            if (lauta[7][3] != nullptr || lauta[7][2] != nullptr || lauta[7][1] != nullptr)
            {
                kayttoliittyma.tulostaVirhe("Tornitus ei ole mahdollinen, siihen ei ole tilaa.");
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
            kayttoliittyma.tulostaVirhe("Siirron alkuruutu on laudan ulkopuolella.");
        }
        
        if (!loppu.ok())
        {
            kayttoliittyma.tulostaVirhe("Siirron loppuruutu on laudan ulkopuolella.");
        }
        
        Nappula* nappula = lauta[alku.getRivi()][alku.getSarake()];
        
        if (nappula == nullptr)
        {
            kayttoliittyma.tulostaVirhe("Siirron alkuruudussa ei ole nappulaa.");
            return false;
        }
        
        if (nappula->getVari() != _siirtovuoro)
        {
            kayttoliittyma.tulostaVirhe("Et voi siirtää vastustajan nappulaa.");
            return false;
        }
    }
    
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    
    if (std::find(siirrot.begin(), siirrot.end(), siirto) != siirrot.end())
    {
        return true;
    }
    else
    {
        kayttoliittyma.tulostaVirhe("Siirto ei ole laillinen.");
        return false;
    }
}

float Asema::evaluoi() const
{
    return laskeNappuloidenArvo();
//    int vari = getSiirtovuoro();
//    float summa = laskeNappuloidenArvo() + nappuloitaKeskella() +linjat(vari);
//
//    //jos avaus tai keskipeli, niin hyva etta kuningas on ruudussa g1 tai b1/c1
//    if (onkoAvausTaiKeskipeli(vari)) {
//        //onko valkean f ja g sotilas paikallaan
//        if (lauta[0][6] != NULL && lauta[1][5] != NULL && lauta[1][6] != NULL)
//        {
//            if (lauta[0][6]->getKoodi() == VK && (lauta[1][5]->getKoodi() == VS && (lauta[1][6]->getKoodi() == VS)))
//                summa += 2;
//        }
//
//        //onko valkean c ja b sotilas paikallaan
//        if (lauta[0][1] != NULL && lauta[0][2] != NULL && lauta[1][1] != NULL && lauta[1][2] != NULL)
//        {
//            if ((lauta[0][1]->getKoodi() == VK || lauta[0][2]->getKoodi() == VK) && (lauta[1][1]->getKoodi() == VS && (lauta[1][2]->getKoodi() == VS)))
//                summa += 1;
//        }
//
//        //onko mustan f ja g sotilas paikallaan
//        if (lauta[7][6] != NULL && lauta[6][5] != NULL && lauta[6][6] != NULL)
//        {
//            if (lauta[7][6]->getKoodi() == MK && (lauta[6][5]->getKoodi() == MS && (lauta[6][6]->getKoodi() == MS)))
//                summa -= 2;
//        }
//
//        //onko mustan c ja b sotilas paikallaan
//        if (lauta[7][1] != NULL && lauta[7][2] != NULL && lauta[7][1] != NULL && lauta[7][2] != NULL)
//        {
//            if ((lauta[7][1]->getKoodi() == MK || lauta[7][2]->getKoodi() == MK) && (lauta[7][1]->getKoodi() == MS && (lauta[7][2]->getKoodi() == MS)))
//                summa -= 1;
//        }
//    }
//
//    return summa;
}

float Asema::laskeNappuloidenArvo() const
{
    float summa = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            if (nappula == nullptr)
            {
                continue;
            }
            
            summa += nappula->getArvo();
        }
    }
    
    return summa;
}

bool Asema::onkoAvausTaiKeskipeli(int vari) const
{
    int valkeaUpseeriLkm = 0;
    int mustaUpseeriLkm = 0;
    bool valkeaDaami = false;
    bool mustaDaami = false;
    
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (lauta[y][x] == NULL) {
                continue;
            }
            
            Nappula* nappula = lauta[y][x];
            
            switch (nappula->getKoodi())
            {
                case VD:
                    valkeaUpseeriLkm += 1;
                    valkeaDaami = true;
                    break;
                case VT:
                    valkeaUpseeriLkm += 1;
                    break;
                case VL:
                    valkeaUpseeriLkm += 1;
                    break;
                case VR:
                    valkeaUpseeriLkm += 1;
                    break;
                case MD:
                    mustaUpseeriLkm += 1;
                    break;
                case MT:
                    mustaUpseeriLkm += 1;
                    mustaDaami = true;
                    break;
                case ML:
                    mustaUpseeriLkm += 1;
                    break;
                case MR:
                    mustaUpseeriLkm += 1;
                    break;
                default:
                    break;
            }
        }
    }
    
    if (vari == 0) {
        if (mustaUpseeriLkm > 2 || (mustaDaami == true && mustaUpseeriLkm > 1))
            return true;
        else
            return false;
    }
    else {
        if (valkeaUpseeriLkm > 2 || (valkeaDaami == true && valkeaUpseeriLkm > 1))
            return true;
        else
            return false;
    }
}

float Asema::nappuloitaKeskella() const
{
    float summa = 0;
    
    //Ydinkeskusta
    for (int y = 3; y < 5; y++)
    {
        for (int x = 3; x < 5; x++)
        {
            if (lauta[y][x] != NULL && (lauta[y][x]->getKoodi() == VS || lauta[y][x]->getKoodi() == VR)) {
                summa += 0.25;
            }
            
            else if (lauta[y][x] != NULL && (lauta[y][x]->getKoodi() == MS || lauta[y][x]->getKoodi() == MR))
            {
                summa -= 0.25;
            }
        }
    }
    
    //Laitakeskusta
    for (int x = 2; x < 6; x++)
    {
        if (lauta[2][x] != NULL)
        {
            if (lauta[2][x]->getKoodi() == VS || lauta[2][x]->getKoodi() == VR) summa += 0.11;
            else if (lauta[2][x]->getKoodi() == MS || lauta[2][x]->getKoodi() == MR) summa -= 0.11;
        }
        
        if (lauta[5][x] != NULL)
        {
            if (lauta[5][x]->getKoodi() == VS || lauta[5][x]->getKoodi() == VR) summa += 0.11;
            else if (lauta[5][x]->getKoodi() == MS || lauta[5][x]->getKoodi() == MR) summa -= 0.11;
        }
    }
    
    for (int y = 3; y < 5; y++) {
        if (lauta[y][2] != NULL)
        {
            if (lauta[y][2]->getKoodi() == VS || lauta[y][2]->getKoodi() == VR) summa += 0.11;
            else if (lauta[y][2]->getKoodi() == MS || lauta[y][2]->getKoodi() == MR) summa -= 0.11;
        }
        
        if (lauta[y][5] != NULL)
        {
            if (lauta[y][5]->getKoodi() == VS || lauta[y][5]->getKoodi() == VR) summa += 0.11;
            else if (lauta[y][5]->getKoodi() == MS || lauta[y][5]->getKoodi() == MR) summa -= 0.11;
        }
    }
    
    return summa;
}

float Asema::linjat(int vari) const
{
    // daami, torni ja lähetti viihtyvät avoimilla linjoilla
    
    int valkeanLinjat = 0;
    int mustanLinjat = 0;
    
    std::vector<Siirto> valkeat;
    std::vector<Siirto> mustat;
    
    
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            
            if (lauta[y][x] == NULL)
            {
                continue;
            }
            
            // valkoiset
            if (lauta[y][x]->getKoodi() == VT)
            {
                nappula->annaSiirrot(valkeat, Ruutu(x, y), *this, 0);
            }
            if (lauta[y][x]->getKoodi() == VL)
            {
                nappula->annaSiirrot(valkeat, Ruutu(x, y), *this, 0);
            }
            if (lauta[y][x]->getKoodi() == VD)
            {
                nappula->annaSiirrot(valkeat, Ruutu(x, y), *this, 0);
            }
            
            // mustat
            if (lauta[y][x]->getKoodi() == MT)
            {
                nappula->annaSiirrot(mustat, Ruutu(x, y), *this, 1);
            }
            if (lauta[y][x]->getKoodi() == ML)
            {
                nappula->annaSiirrot(mustat, Ruutu(x, y), *this, 1);
            }
            if (lauta[y][x]->getKoodi() == MD)
            {
                nappula->annaSiirrot(mustat, Ruutu(x, y), *this, 1);
            }
            
        }
    }
    
    valkeanLinjat = valkeat.size();
    mustanLinjat = mustat.size();
    
    if (vari == 0)
    {
        return valkeanLinjat;
    }
    else
    {
        return mustanLinjat;
    }
    
}

std::vector<size_t> Asema::jaaSiirrotSaikeidenKesken(size_t siirtoLkm) const
{
    uint32_t saieLkm = std::max(1u, std::thread::hardware_concurrency());
    
    std::vector<size_t> siirtoaPerSaie;
    siirtoaPerSaie.reserve(siirtoLkm);
    
    if (siirtoLkm < saieLkm)
    {
        for (size_t i = 0; i < siirtoLkm; i++)
        {
            siirtoaPerSaie.push_back(1);
        }
    }
    else if (siirtoLkm % saieLkm == 0)
    {
        for (size_t i = 0; i < saieLkm; i++)
        {
            siirtoaPerSaie.push_back(siirtoLkm / saieLkm);
        }
    }
    else
    {
        size_t pieniLkm = siirtoLkm / saieLkm;
        size_t suuriLkm = saieLkm - (siirtoLkm % saieLkm);
        
        for (size_t i = 0; i < suuriLkm; i++)
        {
            siirtoaPerSaie.push_back(pieniLkm);
        }
        
        for (size_t i = suuriLkm; i < saieLkm; i++)
        {
            siirtoaPerSaie.push_back(pieniLkm + 1);
        }
    }
    
    return siirtoaPerSaie;
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
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    
    if (siirrot.empty())
    {
        if (_siirtovuoro == 0 && onkoRuutuUhattu(_valkeanKuninkaanRuutu, 1))
        {
            return MinMaxPaluu(std::numeric_limits<float>::lowest(), Siirto());
        }
        else if (_siirtovuoro == 1 && onkoRuutuUhattu(_mustanKuninkaanRuutu, 0))
        {
            return MinMaxPaluu(std::numeric_limits<float>::max(), Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    size_t siirtoLkm = siirrot.size();
    
    std::vector<size_t> siirtoaPerSaie = jaaSiirrotSaikeidenKesken(siirtoLkm);
    std::vector<std::future<MinMaxPaluu>> tehtavat;
    tehtavat.reserve(siirtoLkm);
    
    if (_siirtovuoro == 0)
    {
        using Iteraattori = std::vector<Siirto>::iterator;
        auto maxiAsync = [this, syvyys](Iteraattori alku, Iteraattori loppu) -> MinMaxPaluu
        {
            MinMaxPaluu maxi(std::numeric_limits<float>::lowest(), *alku);
            for (auto& iter = alku; iter != loppu; ++iter)
            {
                Siirto& siirto = *iter;
                Asema tmpAsema = *this;
                tmpAsema.paivitaAsema(siirto);
                MinMaxPaluu arvo = tmpAsema.mini(syvyys - 1);
                arvo.parasSiirto = siirto;
                
                if (arvo.evaluointiArvo > maxi.evaluointiArvo)
                {
                    maxi = arvo;
                }
            }
            
            return maxi;
        };
        
        Iteraattori iter = siirrot.begin();
        for (size_t i = 0; i < siirtoaPerSaie.size(); i++)
        {
            auto tehtava = std::bind(maxiAsync, iter, iter + siirtoaPerSaie[i]);
            tehtavat.push_back(std::async(std::launch::async, tehtava));
            iter += siirtoaPerSaie[i];
        }
        
        MinMaxPaluu maxi = tehtavat[0].get();
        for (size_t i = 1; i < tehtavat.size(); i++)
        {
            MinMaxPaluu arvo = tehtavat[i].get();
            if (arvo.evaluointiArvo > maxi.evaluointiArvo)
            {
                maxi = arvo;
            }
        }
        
        return maxi;
    }
    else
    {
        using KaanteisIteraattori = std::vector<Siirto>::reverse_iterator;
        auto miniAsync = [this, syvyys](KaanteisIteraattori alku, KaanteisIteraattori loppu) -> MinMaxPaluu
        {
            MinMaxPaluu mini(std::numeric_limits<float>::max(), *alku);
            
            for (auto& iter = alku; iter != loppu; ++iter)
            {
                Siirto& siirto = *iter;
                Asema tmpAsema = *this;
                tmpAsema.paivitaAsema(siirto);
                MinMaxPaluu arvo = tmpAsema.maxi(syvyys - 1);
                arvo.parasSiirto = siirto;
                
                if (arvo.evaluointiArvo < mini.evaluointiArvo)
                {
                    mini = arvo;
                }
            }
            
            return mini;
        };
        
        KaanteisIteraattori iter = siirrot.rbegin();
        for (size_t i = 0; i < siirtoaPerSaie.size(); i++)
        {
            auto tehtava = std::bind(miniAsync, iter, iter + siirtoaPerSaie[i]);
            tehtavat.push_back(std::async(std::launch::async, tehtava));
            iter += siirtoaPerSaie[i];
        }
        
        MinMaxPaluu mini = tehtavat[0].get();
        
        // Reverse for loop.                    ;)
        for (size_t i = tehtavat.size(); i-- > 1;)
        {
            MinMaxPaluu arvo = tehtavat[i].get();
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
    
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    
    if (siirrot.empty())
    {
        if (onkoRuutuUhattu(_valkeanKuninkaanRuutu, 1))
        {
            return MinMaxPaluu(std::numeric_limits<float>::lowest(), Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    MinMaxPaluu maxi(std::numeric_limits<float>::lowest(), siirrot.front());
    
    for (auto& siirto : siirrot)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        MinMaxPaluu arvo = tmpAsema.mini(syvyys - 1);
        arvo.parasSiirto = siirto;
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
    
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    if (siirrot.empty())
    {
        if (onkoRuutuUhattu(_mustanKuninkaanRuutu, 0))
        {
            return MinMaxPaluu(std::numeric_limits<float>::max(), Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    MinMaxPaluu mini(std::numeric_limits<float>::max(), siirrot.back());
    
    // Reverse for loop.                    ;)
    for (size_t i = siirrot.size(); i-- > 0 ;)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirrot[i]);
        MinMaxPaluu arvo = tmpAsema.maxi(syvyys - 1);
        arvo.parasSiirto = siirrot[i];
        if (arvo.evaluointiArvo < mini.evaluointiArvo)
        {
            mini = arvo;
        }
    }
    return mini;
}

MinMaxPaluu Asema::alphabetaMinimax(int syvyys) const
{
    if (_siirtovuoro == 0)
    {
        return alphabetaMaxi(syvyys, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
    }
    else
    {
        return alphabetaMini(syvyys, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
    }
}

MinMaxPaluu Asema::alphabetaMinimaxAsync(int syvyys) const
{
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    
    if (siirrot.empty())
    {
        if (_siirtovuoro == 0 && onkoRuutuUhattu(_valkeanKuninkaanRuutu, 1))
        {
            return MinMaxPaluu(std::numeric_limits<float>::lowest(), Siirto());
        }
        else if (_siirtovuoro == 1 && onkoRuutuUhattu(_mustanKuninkaanRuutu, 0))
        {
            return MinMaxPaluu(std::numeric_limits<float>::max(), Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    size_t siirtoLkm = siirrot.size();
    
    std::vector<size_t> siirtoaPerSaie = jaaSiirrotSaikeidenKesken(siirtoLkm);
    
    std::vector<std::future<MinMaxPaluu>> tehtavat;
    tehtavat.reserve(siirtoLkm);
    
    if (_siirtovuoro == 0)
    {
        using Iteraattori = std::vector<Siirto>::iterator;
        auto maxiAsync = [this, syvyys](Iteraattori alku, Iteraattori loppu) -> MinMaxPaluu
        {
            MinMaxPaluu maxi(std::numeric_limits<float>::lowest(), *alku);
            
            constexpr float beta = std::numeric_limits<float>::max();
            
            for (auto& iter = alku; iter != loppu; ++iter)
            {
                Siirto& siirto = *iter;
                Asema tmpAsema = *this;
                tmpAsema.paivitaAsema(siirto);
                MinMaxPaluu arvo = tmpAsema.alphabetaMini(syvyys - 1, maxi.evaluointiArvo, beta);
                arvo.parasSiirto = siirto;
                
                if (arvo.evaluointiArvo >= beta)
                {
                    return MinMaxPaluu(beta, siirto);
                }
                
                if (arvo.evaluointiArvo > maxi.evaluointiArvo)
                {
                    maxi = arvo;
                }
            }
            
            return maxi;
        };
        
        auto iter = siirrot.begin();
        for (size_t i = 0; i < siirtoaPerSaie.size(); i++)
        {
            auto tehtava = std::bind(maxiAsync, iter, iter + siirtoaPerSaie[i]);
            tehtavat.push_back(std::async(std::launch::async, tehtava));
            iter += siirtoaPerSaie[i];
        }
        
        MinMaxPaluu maxi = tehtavat[0].get();
        
        for (size_t i = 1; i < tehtavat.size(); i++)
        {
            MinMaxPaluu arvo = tehtavat[i].get();
            if (arvo.evaluointiArvo > maxi.evaluointiArvo)
            {
                maxi = arvo;
            }
        }
        
        return maxi;
    }
    else
    {
        using KaanteisIteraattori = std::vector<Siirto>::reverse_iterator;
        auto miniAsync = [this, syvyys](KaanteisIteraattori alku, KaanteisIteraattori loppu) -> MinMaxPaluu
        {
            MinMaxPaluu mini(std::numeric_limits<float>::max(), *alku);
            
            constexpr float alpha = std::numeric_limits<float>::lowest();
            
            for (auto& iter = alku; iter != loppu; ++iter)
            {
                Siirto& siirto = *iter;
                Asema tmpAsema = *this;
                tmpAsema.paivitaAsema(siirto);
                MinMaxPaluu arvo = tmpAsema.alphabetaMaxi(syvyys - 1, alpha, mini.evaluointiArvo);
                arvo.parasSiirto = siirto;
                
                if (arvo.evaluointiArvo <= alpha)
                {
                    return MinMaxPaluu(alpha, siirto);
                }
                
                if (arvo.evaluointiArvo < mini.evaluointiArvo)
                {
                    mini = arvo;
                }
            }
            
            return mini;
        };
        
        KaanteisIteraattori iter = siirrot.rbegin();
        for (size_t i = 0; i < siirtoaPerSaie.size(); i++)
        {
            auto tehtava = std::bind(miniAsync, iter, iter + siirtoaPerSaie[i]);
            tehtavat.push_back(std::async(std::launch::async, tehtava));
            iter += siirtoaPerSaie[i];
        }
        
        MinMaxPaluu mini = tehtavat[0].get();
        
        // Reverse for loop.                    ;)
        for (size_t i = tehtavat.size(); i-- > 1;)
        {
            MinMaxPaluu arvo = tehtavat[i].get();
            if (arvo.evaluointiArvo < mini.evaluointiArvo)
            {
                mini = arvo;
            }
        }
        
        return mini;
    }
}

MinMaxPaluu Asema::alphabetaMaxi(int syvyys, float alpha, float beta) const
{
    if (syvyys == 0)
    {
        return MinMaxPaluu(evaluoi(), Siirto());
    }
    
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    
    if (siirrot.empty())
    {
        if (onkoRuutuUhattu(_valkeanKuninkaanRuutu, 1))
        {
            return MinMaxPaluu(std::numeric_limits<float>::lowest(), Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    MinMaxPaluu maxi(alpha, siirrot.front());
    
    for (auto& siirto : siirrot)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        MinMaxPaluu arvo = tmpAsema.alphabetaMini(syvyys - 1, maxi.evaluointiArvo, beta);
        arvo.parasSiirto = siirto;
        
        if (arvo.evaluointiArvo >= beta)
        {
            return MinMaxPaluu(beta, siirto);
        }
        
        if (arvo.evaluointiArvo > maxi.evaluointiArvo)
        {
            maxi = arvo;
        }
    }
    
    return maxi;
}

MinMaxPaluu Asema::alphabetaMini(int syvyys, float alpha, float beta) const
{
    if (syvyys == 0)
    {
        return MinMaxPaluu(evaluoi(), Siirto());
    }
    
    std::vector<Siirto> siirrot = annaLaillisetSiirrot();
    
    if (siirrot.empty())
    {
        if (onkoRuutuUhattu(_mustanKuninkaanRuutu, 0))
        {
            return MinMaxPaluu(std::numeric_limits<float>::max(), Siirto());
        }
        else
        {
            return MinMaxPaluu(0, Siirto());
        }
    }
    
    MinMaxPaluu mini(beta, siirrot.back());
    
    // Reverse for loop.                    ;)
    for (size_t i = siirrot.size(); i-- > 0 ;)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirrot[i]);
        MinMaxPaluu arvo = tmpAsema.alphabetaMaxi(syvyys - 1, alpha, mini.evaluointiArvo);
        arvo.parasSiirto = siirrot[i];
        
        if (arvo.evaluointiArvo <= alpha)
        {
            return MinMaxPaluu(alpha, siirrot[i]);
        }
        
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
            && lauta[0][5] == nullptr
            && lauta[0][6] == nullptr)
        {
            siirrot.push_back(Siirto(true, false));
        }
        
        // valkea, pitkä linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaDTliikkunut()
            && !onkoRuutuUhattu(Ruutu(4, 0), 1)
            && !onkoRuutuUhattu(Ruutu(3, 0), 1)
            && !onkoRuutuUhattu(Ruutu(2, 0), 1)
            && lauta[0][1] == nullptr
            && lauta[0][2] == nullptr
            && lauta[0][3] == nullptr)
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
            && lauta[7][5] == nullptr
            && lauta[7][6] == nullptr)
        {
            siirrot.push_back(Siirto(true, false));
        }
        
        // musta, pitkä linna
        if (!onkoMustaKuningasLiikkunut() && !onkoMustaDTliikkunut()
            && !onkoRuutuUhattu(Ruutu(4, 7), 0)
            && !onkoRuutuUhattu(Ruutu(3, 7), 0)
            && !onkoRuutuUhattu(Ruutu(2, 7), 0)
            && lauta[7][1] == nullptr
            && lauta[7][2] == nullptr
            && lauta[7][3] == nullptr)
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
            if (tmpAsema.onkoRuutuUhattu(tmpAsema._valkeanKuninkaanRuutu, 1))
            {
                continue;
            }
        }
        else if (tmpAsema.onkoRuutuUhattu(tmpAsema._mustanKuninkaanRuutu, 0))
        {
            continue;
        }
        
        laillisetSiirrot.push_back(siirto);
    }
    
    siirrot = laillisetSiirrot;
}

std::vector<Siirto> Asema::annaLaillisetSiirrot() const
{
    std::vector<Siirto> siirrot;
    siirrot.reserve(40);
    
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
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
    
    std::vector<float> arvot(siirrot.size());
    std::vector<size_t> indeksit(siirrot.size());
    for (size_t i = 0; i < siirrot.size(); i++)
    {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirrot[i]);
        arvot[i] = tmpAsema.evaluoi();
        indeksit[i] = i;
    }
    
    std::sort(indeksit.begin(), indeksit.end(), [this, &arvot](auto a, auto b) {
        return arvot[a] > arvot[b];
    });
    
    for (size_t i = 0; i < siirrot.size(); i++) {
        Siirto tmpSiirto = siirrot[i];
        siirrot[i] = siirrot[indeksit[i]];
        siirrot[indeksit[i]] = tmpSiirto;
    }
    
    return siirrot;
}

std::ostream& operator<<(std::ostream& os, const Asema& asema)
{
    // Lauta
    int tyhjaa = 0;
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            Nappula* nappula = asema.lauta[y][x];
            if (nappula == nullptr)
            {
                tyhjaa++;
            }
            else
            {
                if (tyhjaa > 0)
                {
                    os << tyhjaa;
                    tyhjaa = 0;
                }
                
                os << nappula->getFENMerkki();
            }
        }
        
        if (tyhjaa > 0)
        {
            os << tyhjaa;
            tyhjaa = 0;
        }
        
        if (y > 0)
        {
            os << "/";
        }
    }
    
    // Siirtovuoro
    os << " " << (asema.getSiirtovuoro() == 0 ? "w" : "b");
    
    // Linnoitus
    std::string linnoitus;
    
    if (!asema.onkoValkeaKuningasLiikkunut())
    {
        if (!asema.onkoValkeaKTliikkunut())
        {
            linnoitus += "K";
        }
        
        if (!asema.onkoValkeaDTliikkunut())
        {
            linnoitus += "Q";
        }
    }
    
    if (!asema.onkoMustaKuningasLiikkunut())
    {
        if (!asema.onkoMustaKTliikkunut())
        {
            linnoitus += "k";
        }
        
        if (!asema.onkoValkeaKTliikkunut())
        {
            linnoitus += "q";
        }
    }
    
    os << " " << (linnoitus.empty() ? "-" : linnoitus);
    
    // Ohestalyonti
    os << " ";
    if (asema.getKaksoisaskelSarake() == -1)
    {
        os << "-";
    }
    else
    {
        int rivi = asema.getSiirtovuoro() == 0 ? 5 : 2;
        os << Ruutu(asema.getKaksoisaskelSarake(), rivi);
    }
    
    return os;
}
