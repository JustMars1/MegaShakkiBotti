#include <iostream>
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

std::map<char, Nappula*> Asema::charToValkoinenNappula = {
    {'K', &Asema::vk},
    {'D', &Asema::vd},
    {'T', &Asema::vt},
    {'L', &Asema::vl},
    {'R', &Asema::vr},
    {'S', &Asema::vs}
};

std::map<char, Nappula*> Asema::charToMustaNappula = {
    {'K', &Asema::mk},
    {'D', &Asema::md},
    {'T', &Asema::mt},
    {'L', &Asema::ml},
    {'R', &Asema::mr},
    {'S', &Asema::ms}
};

std::map<NappulaKoodi, float> Asema::nappulaKoodiToArvo = {
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


// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
// Asetetaan alkuaseman mukaisesti nappulat ruuduille
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
, _onkoMustaKTliikkunut{false} {}

bool Asema::paivitaAsema(const Siirto& siirto)
{
    // Tarkastetaan on siirto lyhyt linna tai pitkä linna
    
    if (siirto.onkoLyhytLinna() || siirto.onkoPitkalinna() )
    {
        if (_siirtovuoro == 0)
        {
            // vuoro on valkoisen
            if (siirto.onkoLyhytLinna() && lauta[0][5] == NULL && lauta[0][6] == NULL && !onkoValkeaKTliikkunut() && !onkoValkeaKuningasLiikkunut())
            {
                _onkoValkeaKTliikkunut = true;
                _onkoValkeaKuningasLiikkunut = true;
                
                lauta[0][5] = &Asema::vt;
                lauta[0][6] = &Asema::vk;
                lauta[0][4] = NULL;
                lauta[0][7] = NULL;
                
                _siirtovuoro = 1;
                return true;
            }
            else if (siirto.onkoPitkalinna() && lauta[0][3] == NULL && lauta[0][2] == NULL && lauta[0][1] == NULL && !onkoValkeaDTliikkunut() && !onkoValkeaKuningasLiikkunut())
            {
                _onkoValkeaDTliikkunut = true;
                _onkoValkeaKuningasLiikkunut = true;
                
                lauta[0][3] = &Asema::vt;
                lauta[0][2] = &Asema::vk;
                lauta[0][0] = NULL;
                lauta[0][4] = NULL;
                
                _siirtovuoro = 1;
                return true;
            }
        }
        else
        {
            // on mustan vuoro
            if (siirto.onkoLyhytLinna() && lauta[7][5] == NULL && lauta[7][6] == NULL && !onkoMustaKTliikkunut() && !onkoMustaKuningasLiikkunut())
            {
                _onkoMustaKTliikkunut = true;
                _onkoMustaKuningasLiikkunut = true;
                
                lauta[7][5] = &Asema::mt;
                lauta[7][6] = &Asema::mk;
                lauta[7][7] = NULL;
                lauta[7][4] = NULL;
                
                _siirtovuoro = 0;
                return true;
            }
            else if (siirto.onkoPitkalinna() && lauta[7][3] == NULL && lauta[7][2] == NULL && lauta[7][1] == NULL && !onkoMustaDTliikkunut() && !onkoMustaKuningasLiikkunut())
            {
                _onkoMustaDTliikkunut = true;
                _onkoMustaKuningasLiikkunut = true;
                
                lauta[7][3] = &Asema::mt;
                lauta[7][2] = &Asema::mk;
                lauta[7][0] = NULL;
                lauta[7][4] = NULL;
                
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
    
    if (alkuY < 0 || alkuY > 7 || alkuX < 0 || alkuX > 7) {
        Kayttoliittyma::tulostaVirhe("Siirron alkuruutu laudan ulkopuolella.");
        return false;
    }
    
    if (loppuY < 0 || loppuY > 7 || loppuX < 0 || loppuX > 7) {
        Kayttoliittyma::tulostaVirhe("Siirron loppuruutu laudan ulkopuolella.");
        return false;
    }
    
    Nappula* nappulaPtr = lauta[alkuY][alkuX];
    
    if (nappulaPtr == nullptr) {
        Kayttoliittyma::tulostaVirhe("Siirron alkuruudussa ei ole nappulaa.");
        return false;
    }
    
    if (nappulaPtr->getVari() != _siirtovuoro) {
        Kayttoliittyma::tulostaVirhe("Et voi siirtää vastustajan nappulaa.");
        return false;
    }
    
    if (nappulaPtr == &Asema::vk)
    {
        _onkoValkeaKuningasLiikkunut = true;
    }
    else if (nappulaPtr == &Asema::mk)
    {
        _onkoMustaKuningasLiikkunut = true;
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
        if ((alkuY == 1 || alkuY == 6) && (loppuY == 3 || loppuY == 4))
        {
            kaksoisAskel = loppuX;
        }
        else if (_siirtovuoro == 0 && kaksoisAskel != -1 && lauta[loppuY][loppuX] == lauta[5][kaksoisAskel])
        {
            // Valkosen sotilaan ohestalyönti
            lauta[loppuY - 1][kaksoisAskel] = NULL;
        }
        else if (_siirtovuoro == 1 && kaksoisAskel != -1 && lauta[loppuY][loppuX] == lauta[2][kaksoisAskel])
        {
            // Mustan sotilaan ohestalyönti
            lauta[loppuY + 1][kaksoisAskel] = NULL;
        }
        else {
            // Ohestalyönti ei ole mahdollinen
            kaksoisAskel = -1;
        }
    }
    
    if (siirto.miksikorotetaan != nullptr) {
        lauta[loppuY][loppuX] = siirto.miksikorotetaan;
    }
    else {
        lauta[loppuY][loppuX] = nappulaPtr;
    }
    
    lauta[alkuY][alkuX] = NULL;
    
    _siirtovuoro = 1 - _siirtovuoro;
    
    return true;
    
    // Kaikki muut siirrot
    
    
    //Ottaa siirron alkuruudussa olleen nappulan talteen
    
    
    //Laittaa talteen otetun nappulan uuteen ruutuun
    
    //// Katsotaan jos nappula on sotilas ja rivi on päätyrivi niin ei vaihdeta nappulaa
    ////eli alkuruutuun laitetaan null ja loppuruudussa on jo kliittymän laittama nappula MIIKKA, ei taida minmaxin kanssa hehkua?
    
    //
    ////muissa tapauksissa alkuruutuun null ja loppuruutuun sama alkuruudusta lähtenyt nappula
    
    // katsotaan jos liikkunut nappula on kuningas niin muutetaan onkoKuningasLiikkunut arvo (molemmille väreille)
    
    // katsotaan jos liikkunut nappula on torni niin muutetaan onkoTorniLiikkunut arvo (molemmille väreille ja molemmille torneille)
    
    //päivitetään _siirtovuoro
    
    //_siirtovuoro = 1 - _siirtovuoro;
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
    float summa = laskeNappuloidenArvo(_siirtovuoro);
    return summa;
    
    //kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta
    
    //1. Nappuloiden arvo
    
    //2. Kuningas turvassa
    
    //3. Arvosta keskustaa
    
    // 4. Arvosta linjoja
    
}

float Asema::laskeNappuloidenArvo(int vari)
{
    float summa = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Nappula* nappula = lauta[y][x];
            
            if (nappula == nullptr) {
                continue;
            }
            
            float kerroin = _siirtovuoro == 0 ? 1 : -1;
            float arvo = kerroin * nappulaKoodiToArvo[nappula->getKoodi()];
            
            summa += arvo;
        }
    }
    
    return summa;
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
    
//    if (valkoisenUpseerit + mustanUpseerit < 4 || mustanDaami || valkoisenDaami)
//    {
//        return false;
//    }
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

// https://chessprogramming.wikispaces.com/Minimax MinMax-algoritmin pseudokoodi (lisäsin parametrina aseman)
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
    MinMaxPaluu paluuarvo(0, Siirto(Ruutu(0, 0), Ruutu(0, 0)));
    
    // Generoidaan aseman lailliset siirrot.
    
    // Rekursion kantatapaus 1: peli on loppu
    
    // Rekursion kantatapaus 2: katkaisusyvyydessä
    
    // Rekursioaskel: kokeillaan jokaista laillista siirtoa s
    // (alustetaan paluuarvo huonoimmaksi mahdolliseksi).
    
    return paluuarvo;
}

MinMaxPaluu Asema::maxi(int syvyys)
{
    MinMaxPaluu paluu(0, Siirto(Ruutu(0, 0), Ruutu(0, 0)));
    return paluu;
}

MinMaxPaluu Asema::mini(int syvyys)
{
    MinMaxPaluu paluu(0, Siirto(Ruutu(0, 0), Ruutu(0, 0)));
    return paluu;
}

bool Asema::onkoRuutuUhattu(const Ruutu& ruutu, int vastustajanVari)
{
    // Tarkistetaan siirtojen laillisuus. Eli asettaako siirto kunkinaan uhatuksi.
    
    // Valkoinen = 0, musta = 1.
    
    int x = ruutu.getSarake();
    int y = ruutu.getRivi();
    
    // Tarkistetaan asettuuko valkoinen kuningas uhatuksi.
    if (lauta[y][x] == &vk && vastustajanVari == 1)
    {
        //std::cout << "\n|| Laiton siirto. Valkoinen kuningas uhattu. ||\n";
        return true;
    }
    // Tarkistetaan asettuuko musta kuningas uhatuksi.
    else if (lauta[y][x] == &mk && vastustajanVari == 0)
    {
        //std::cout << "\n|| Laiton siirto. Musta kuningas uhattu. ||\n";
        return true;
    }
    
    return false;
}

void Asema::annaLinnoitusSiirrot(std::list<Siirto>& lista, int vari)
{
    if (vari == 0) {
        // valkea, lyhyt linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaKTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 0), 1)
            && onkoRuutuUhattu(Ruutu(6, 0), 1)
            && lauta[0][5] == NULL
            && lauta[0][6] == NULL)
        {
            lista.push_back(Siirto(true, false));
        }
        
        // valkea, pitkä linna
        if (!onkoValkeaKuningasLiikkunut() && !onkoValkeaDTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 0), 1)
            && onkoRuutuUhattu(Ruutu(2, 0), 1)
            && lauta[0][1] == NULL
            && lauta[0][2] == NULL
            && lauta[0][3] == NULL)
        {
            lista.push_back(Siirto(false, true));
        }
        
    }
    
    if (vari == 1)
    {
        // musta, lyhyt linna
        if (!onkoMustaKuningasLiikkunut() && !onkoMustaKTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 7), 0)
            && onkoRuutuUhattu(Ruutu(5, 7), 0)
            && lauta[7][5] == NULL
            && lauta[7][6] == NULL)
        {
            lista.push_back(Siirto(true, false));
        }
        
        // musta, pitkä linna
        if (!onkoMustaKuningasLiikkunut() && !onkoMustaDTliikkunut()
            && onkoRuutuUhattu(Ruutu(4, 7), 0)
            && onkoRuutuUhattu(Ruutu(3, 7), 0)
            && lauta[7][1] == NULL
            && lauta[7][2] == NULL
            && lauta[7][3] == NULL)
        {
            lista.push_back(Siirto(false, true));
        }
    }
}

void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>& lista, int vari)
{
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista)
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (lauta[y][x] != NULL && lauta[y][x]->getVari() == getSiirtovuoro())
            {
                Ruutu ruutu(x, y);
                lauta[y][x]->annaSiirrot(lista, ruutu, *this, lauta[y][x]->getVari());
            }
        }
    }
    
    // Kuninkaiden ruudut.
    Ruutu vkRuutu(0, 0);
    Ruutu mkRuutu(0, 0);
    
    // Asema missä katsotaan siirtojen laillisuutta.
    Asema uusiAsema;
    
    // Mustan siirrot.
    std::list<Siirto> mSiirrot;
    // Valkoisen siirrot.
    std::list<Siirto> vSiirrot;
    
    int index = 0;
    // Käydään läpi siirtojen lista.
    for (auto& s : lista)
    {
        int xAlku = s.getAlkuruutu().getSarake();
        int yAlku = s.getAlkuruutu().getRivi();
        
        // Valkoisen siirtovuoro, tsekataan mustan siirrot.
        if (_siirtovuoro == 1)
        {
            if (lauta[yAlku][xAlku]->getVari() == 1)
            {
                // Jos ruutu ei uhattu, lisätään laillisten siirtojen listaan.
                if (!onkoRuutuUhattu(Ruutu(xAlku, yAlku), 0))
                {
                    auto listaFront = lista.begin();
                    std::advance(listaFront, index);
                    
                    mSiirrot.push_back(*listaFront);
                }
            }
        }
        // Mustan siirtovuoro, tsekataan valkoisen siirrot.
        if (_siirtovuoro == 0)
        {
            if (lauta[yAlku][xAlku]->getVari() == 0)
            {
                // Jos ruutu ei uhattu, lisätään laillisten siirtojen listaan.
                if (!onkoRuutuUhattu(Ruutu(xAlku, yAlku), 1))
                {
                    auto listaFront = lista.begin();
                    std::advance(listaFront, index);
                    
                    vSiirrot.push_back(*listaFront);                  
                }
            }
        }
        
        index++;
    }
    
    annaLinnoitusSiirrot(lista, _siirtovuoro);
    
    // Tyhjennetään siirtolista ja lisätään sinne sen puolen siirrot, kenen vuoro on.
    if (_siirtovuoro == 1)
    {
        lista.clear();
        lista = mSiirrot;
    }
    else if (_siirtovuoro == 0)
    {
        lista.clear();
        lista = vSiirrot;
    }
   
    // Apulistojen tyhjennys.
    vSiirrot.clear();
    mSiirrot.clear();
}
