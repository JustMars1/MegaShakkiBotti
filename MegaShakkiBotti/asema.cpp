#include <iostream>
#include <limits>
#include "asema.h"
#include "minmaxpaluu.h"
#include "nappula.h"
#include "ruutu.h"
#include "kayttoliittyma.h"

Kuningas Asema::vk = Kuningas("\xe2\x99\x94", 0, VK);
Daami Asema::vd = Daami("\xe2\x99\x95", 0, VD);
Torni Asema::vt = Torni("\xe2\x99\x96", 0, VT);
Lahetti Asema::vl = Lahetti("\xe2\x99\x97", 0, VL);
Ratsu Asema::vr = Ratsu("\xe2\x99\x98", 0, VR);
Sotilas Asema::vs = Sotilas("\xe2\x99\x99", 0, VS);

Kuningas Asema::mk = Kuningas("\xe2\x99\x9a", 1, MK);
Daami Asema::md = Daami("\xe2\x99\x9b", 1, MD);
Torni Asema::mt = Torni("\xe2\x99\x9c", 1, MT);
Lahetti Asema::ml = Lahetti("\xe2\x99\x9d", 1, ML);
Ratsu Asema::mr = Ratsu("\xe2\x99\x9e", 1, MR);
Sotilas Asema::ms = Sotilas("\xe2\x99\x9f", 1, MS);

const std::unordered_map<char, Nappula*> Asema::valkoinenNappulaMap = {
    {'k', &Asema::vk},
    {'d', &Asema::vd},
    {'t', &Asema::vt},
    {'l', &Asema::vl},
    {'r', &Asema::vr},
    {'s', &Asema::vs}
};

const std::unordered_map<char, Nappula*> Asema::mustaNappulaMap = {
    {'k', &Asema::mk},
    {'d', &Asema::md},
    {'t', &Asema::mt},
    {'l', &Asema::ml},
    {'r', &Asema::mr},
    {'s', &Asema::ms}
};

const std::unordered_map<NappulaKoodi, float> Asema::arvoMap = {
    {VD, 9},
    {VT, 5},
    {VL, 3.25},
    {VR, 3},
    {VS, 1},
    {MD, -9},
    {MT, -5},
    {ML, -3.25},
    {MR, -3},
    {MS, -1}
};

const float Asema::maxArvo = Asema::arvoMap.at(VD) + Asema::arvoMap.at(VT) * 2 + Asema::arvoMap.at(VL) * 2 + Asema::arvoMap.at(VR) * 2 + Asema::arvoMap.at(VS) * 8;

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

bool Asema::paivitaAsema(const Siirto& siirto)
{
    // Tarkastetaan on siirto lyhyt linna tai pitkä linna
    
    bool lyhytlinna = siirto.onkoLyhytLinna();
    bool pitkaLinna = siirto.onkoPitkaLinna();
    
    if (lyhytlinna || pitkaLinna)
    {
        if (_siirtovuoro == 0)
        {
            // vuoro on valkoisen
            if (lyhytlinna && lauta[0][5] == NULL && lauta[0][6] == NULL && !onkoValkeaKTliikkunut() && !onkoValkeaKuningasLiikkunut())
            {
                _onkoValkeaKTliikkunut = true;
                _onkoValkeaKuningasLiikkunut = true;
                
                lauta[0][5] = &Asema::vt;
                lauta[0][6] = &Asema::vk;
                lauta[0][4] = NULL;
                lauta[0][7] = NULL;
                
                _valkeaKuningasRuutu.setSarake(6);
                _siirtovuoro = 1;
                return true;
            }
            else if (pitkaLinna && lauta[0][3] == NULL && lauta[0][2] == NULL && lauta[0][1] == NULL && !onkoValkeaDTliikkunut() && !onkoValkeaKuningasLiikkunut())
            {
                _onkoValkeaDTliikkunut = true;
                _onkoValkeaKuningasLiikkunut = true;
                
                lauta[0][3] = &Asema::vt;
                lauta[0][2] = &Asema::vk;
                lauta[0][0] = NULL;
                lauta[0][4] = NULL;
                
                _valkeaKuningasRuutu.setSarake(2);
                _siirtovuoro = 1;
                return true;
            }
        }
        else
        {
            // on mustan vuoro
            if (lyhytlinna && lauta[7][5] == NULL && lauta[7][6] == NULL && !onkoMustaKTliikkunut() && !onkoMustaKuningasLiikkunut())
            {
                _onkoMustaKTliikkunut = true;
                _onkoMustaKuningasLiikkunut = true;
                
                lauta[7][5] = &Asema::mt;
                lauta[7][6] = &Asema::mk;
                lauta[7][7] = NULL;
                lauta[7][4] = NULL;
                
                _mustaKuningasRuutu.setSarake(6);
                _siirtovuoro = 0;
                return true;
            }
            else if (pitkaLinna && lauta[7][3] == NULL && lauta[7][2] == NULL && lauta[7][1] == NULL && !onkoMustaDTliikkunut() && !onkoMustaKuningasLiikkunut())
            {
                _onkoMustaDTliikkunut = true;
                _onkoMustaKuningasLiikkunut = true;
                
                lauta[7][3] = &Asema::mt;
                lauta[7][2] = &Asema::mk;
                lauta[7][0] = NULL;
                lauta[7][4] = NULL;
                
                _mustaKuningasRuutu.setSarake(2);
                _siirtovuoro = 0;
                return true;
            }
        }
        
        Kayttoliittyma::tulostaVirhe("Tornitus ei ole mahdollinen.");
        return false;
    }
    
    int alkuX = siirto.getAlkuruutu().getSarake();
    int alkuY = siirto.getAlkuruutu().getRivi();
    
    int loppuX = siirto.getLoppuruutu().getSarake();
    int loppuY = siirto.getLoppuruutu().getRivi();
    
    if (alkuY < 0 || alkuY > 7 || alkuX < 0 || alkuX > 7)
    {
        Kayttoliittyma::tulostaVirhe("Siirron alkuruutu laudan ulkopuolella.");
        return false;
    }
    
    if (loppuY < 0 || loppuY > 7 || loppuX < 0 || loppuX > 7)
    {
        Kayttoliittyma::tulostaVirhe("Siirron loppuruutu laudan ulkopuolella.");
        return false;
    }
    
    Nappula* nappulaPtr = lauta[alkuY][alkuX];
    
    if (nappulaPtr == nullptr)
    {
        Kayttoliittyma::tulostaVirhe("Siirron alkuruudussa ei ole nappulaa.");
        return false;
    }
    
    if (nappulaPtr->getVari() != _siirtovuoro)
    {
        Kayttoliittyma::tulostaVirhe("Et voi siirtää vastustajan nappulaa.");
        return false;
    }
    
    if (nappulaPtr == &Asema::vk)
    {
        _onkoValkeaKuningasLiikkunut = true;
        _valkeaKuningasRuutu.setSarake(loppuX);
        _valkeaKuningasRuutu.setRivi(loppuY);
    }
    else if (nappulaPtr == &Asema::mk)
    {
        _onkoMustaKuningasLiikkunut = true;
        _mustaKuningasRuutu.setSarake(loppuX);
        _mustaKuningasRuutu.setRivi(loppuY);
    }
    else if (nappulaPtr == &Asema::vt)
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
    else if (nappulaPtr == &Asema::mt)
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
    else if (nappulaPtr == &Asema::vs || nappulaPtr == &Asema::ms)
    {
        // Tarkistetaan, onko ohestalyönti mahdollinen
        if ((alkuY == 1 && loppuY == 3) || (alkuY == 6 && loppuY == 4))
        {
            kaksoisaskel = loppuX;
        }
        else if (kaksoisaskel != -1)
        {
            if (_siirtovuoro == 0 && lauta[loppuY][loppuX] == lauta[5][kaksoisaskel] )
            {
                if (lauta[3][kaksoisaskel] != NULL && lauta[3][kaksoisaskel]->getKoodi() == MS)
                {
                    // Valkosen sotilaan ohestalyönti
                    lauta[loppuY - 1][kaksoisaskel] = NULL;
                }
                
            }
            else if (_siirtovuoro == 1 && lauta[loppuY][loppuX] == lauta[2][kaksoisaskel])
            {
                if (lauta[4][kaksoisaskel] != NULL && lauta[4][kaksoisaskel]->getKoodi() == VS)
                {
                    // Mustan sotilaan ohestalyönti
                    lauta[loppuY + 1][kaksoisaskel] = NULL;
                }
                
            }
            
            kaksoisaskel = -1;
        }
    }
    
    if (nappulaPtr != &Asema::vs && nappulaPtr != &Asema::ms)
    {
        kaksoisaskel = -1;
    }
    
    if (siirto.miksiKorotetaan != nullptr)
    {
        lauta[loppuY][loppuX] = siirto.miksiKorotetaan;
    }
    else
    {
        lauta[loppuY][loppuX] = nappulaPtr;
    }
    
    lauta[alkuY][alkuX] = NULL;
    
    _siirtovuoro = 1 - _siirtovuoro;
    
    return true;
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
float Asema::evaluoi()
{
    float summa = laskeNappuloidenArvo();
    return summa;
    
    //kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta
    
    //1. Nappuloiden arvo
    
    //2. Kuningas turvassa
    
    //3. Arvosta keskustaa
    
    // 4. Arvosta linjoja
    
}

float Asema::laskeNappuloidenArvo()
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
            
            if (nappula == &Asema::vk || nappula == &Asema::mk)
            {
                continue;
            }
            
            summa += arvoMap.at(nappula->getKoodi());
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
    
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
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
//
//int maxi(int depth, asema a) 
//	if (depth == 0) return evaluate();
//	int max = -oo;
//	for (all moves ) {
//		score = mini(depth - 1, seuraaja);
//		if (score > max)
//			max = score;
//	}
//	return max;
//}

//int mini(int depth, asema a) {
//	if (depth == 0) return -evaluate();
//	int min = +oo;
//	for (all moves) {
//		score = maxi(depth - 1);
//		if (score < min)
//			min = score;
//	}
//	return min;
//}
MinMaxPaluu Asema::minimax(int syvyys)
{
    // Generoidaan aseman lailliset siirrot.
    
    // Rekursion kantatapaus 1: peli on loppu
    
    // Rekursion kantatapaus 2: katkaisusyvyydessä
    
    // Rekursioaskel: kokeillaan jokaista laillista siirtoa s
    // (alustetaan paluuarvo huonoimmaksi mahdolliseksi).
    
    if (_siirtovuoro == 0)
    {
        return maxi(syvyys);
    }
    else
    {
        return mini(syvyys);
    }
}

MinMaxPaluu Asema::maxi(int syvyys)
{
    if (syvyys == 0)
    {
        return MinMaxPaluu(evaluoi(), Siirto());
    }
    
    MinMaxPaluu maxi;
    maxi.evaluointiArvo = std::numeric_limits<float>::lowest();
    
    std::list<Siirto> siirrot;
    annaLaillisetSiirrot(siirrot);
    
    if (siirrot.size() < 1)
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

MinMaxPaluu Asema::mini(int syvyys)
{
    if (syvyys == 0)
    {
        return MinMaxPaluu(evaluoi(), Siirto());
    }
    
    MinMaxPaluu mini;
    mini.evaluointiArvo = std::numeric_limits<float>::max();
    
    std::list<Siirto> siirrot;
    annaLaillisetSiirrot(siirrot);
    
    if (siirrot.size() < 1)
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
                std::list<Siirto> vastustajanSiirrot;
                nappula->annaSiirrot(vastustajanSiirrot, Ruutu(x, y), *this, vastustajanVari);
                
                for (auto& siirto : vastustajanSiirrot)
                {
                    // ohestalyöntiä varten, toimii ehkä?
                    /*
                    Nappula* uhattuNappula = lauta[ruutu.getSarake()][ruutu.getRivi()];
                    if ((uhattuNappula == &Asema::vs && nappula == &Asema::ms) || (uhattuNappula == &Asema::ms && nappula == &Asema::vs))
                    {
                        if (kaksoisaskel != -1)
                        {
                            int loppuY = siirto.getLoppuruutu().getRivi();
                            int loppuX = siirto.getLoppuruutu().getSarake();
                            if (_siirtovuoro == 0 && lauta[loppuY][loppuX] == lauta[5][kaksoisaskel] && ruutu == Ruutu(kaksoisaskel, loppuY - 1))
                            {
                                return true;
                            }
                            else if (_siirtovuoro == 1 && lauta[loppuY][loppuX] == lauta[2][kaksoisaskel] && ruutu == Ruutu(kaksoisaskel, loppuY + 1))
                            {
                                return true;
                            }
                        }
                    }
                    */
                    
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

void Asema::annaLinnoitusSiirrot(std::list<Siirto>& siirrot, int vari)
{
    if (vari == 0) {
        // valkea, lyhyt linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaKTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 0), 1)
            && onkoRuutuUhattu(Ruutu(6, 0), 1)
            && onkoRuutuUhattu(Ruutu(5, 0), 1)
            && lauta[0][5] == NULL
            && lauta[0][6] == NULL)
        {
            siirrot.push_back(Siirto(true, false));
        }
        
        // valkea, pitkä linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaDTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 0), 1)
            && onkoRuutuUhattu(Ruutu(3, 0), 1)
            && onkoRuutuUhattu(Ruutu(2, 0), 1)
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
            && onkoRuutuUhattu(Ruutu(4, 7), 0)
            && onkoRuutuUhattu(Ruutu(5, 7), 0)
            && onkoRuutuUhattu(Ruutu(6, 7), 0)
            && lauta[7][5] == NULL
            && lauta[7][6] == NULL)
        {
            siirrot.push_back(Siirto(true, false));
        }
        
        // musta, pitkä linna
        if (!onkoMustaKuningasLiikkunut() && !onkoMustaDTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 7), 0)
            && onkoRuutuUhattu(Ruutu(3, 7), 0)
            && onkoRuutuUhattu(Ruutu(2, 7), 0)
            && lauta[7][1] == NULL
            && lauta[7][2] == NULL
            && lauta[7][3] == NULL)
        {
            siirrot.push_back(Siirto(false, true));
        }
    }
}

void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>& siirrot)
{
    siirrot.remove_if([this](Siirto& siirto) {
        Asema tmpAsema = *this;
        tmpAsema.paivitaAsema(siirto);
        
        if (_siirtovuoro == 0)
        {
            if (tmpAsema.onkoRuutuUhattu(tmpAsema._valkeaKuningasRuutu, 1))
            {
                return true;
            }
        }
        else if (tmpAsema.onkoRuutuUhattu(tmpAsema._mustaKuningasRuutu, 0))
        {
            return true;
        }
        
        return false;
    });
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& siirrot)
{
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
}
