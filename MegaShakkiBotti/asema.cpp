#include <iostream>
#include "asema.h"
#include "minmaxpaluu.h"
#include "nappula.h"
#include "ruutu.h"

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

// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
// Asetetaan alkuaseman mukaisesti nappulat ruuduille
Asema::Asema()
: lauta {
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

void Asema::paivitaAsema(const Siirto& siirto) {
    // Kaksoisaskel-lippu on oletusarvoisesti pois päältä.
    // Asetetaan myöhemmin, jos tarvii.
    // Tarkastetaan on siirto lyhyt linna tai pitkä linna
    
    if (siirto.onkoLyhytLinna() || siirto.onkoPitkalinna() ) {
        if (_siirtovuoro == 0)
        {
            // vuoro on valkoisen
            if (siirto.onkoLyhytLinna())
            {
                _onkoValkeaKTliikkunut = true;
                _onkoValkeaKuningasLiikkunut = true;
                
                lauta[7][5] = &Asema::vt;
                lauta[7][6] = &Asema::vk;
                lauta[7][4] = NULL;
                lauta[7][7] = NULL;
            }
            else if (siirto.onkoPitkalinna())
            {
                _onkoValkeaDTliikkunut = true;
                _onkoValkeaKuningasLiikkunut = true;
                
                lauta[7][3] = &Asema::vt;
                lauta[7][2] = &Asema::vk;
                lauta[7][0] = NULL;
                lauta[7][4] = NULL;
            }
        }
        else {
            // on mustan vuoro
            if (siirto.onkoLyhytLinna())
            {
                _onkoMustaKTliikkunut = true;
                _onkoMustaKuningasLiikkunut = true;
                
                lauta[0][5] = &Asema::mt;
                lauta[0][6] = &Asema::mk;
                lauta[0][7] = NULL;
                lauta[0][4] = NULL;
            }
            else if (siirto.onkoPitkalinna())
            {
                _onkoMustaDTliikkunut = true;
                _onkoMustaKuningasLiikkunut = true;
                
                lauta[0][3] = &Asema::mt;
                lauta[0][2] = &Asema::mk;
                lauta[0][0] = NULL;
                lauta[0][4] = NULL;
            }
        }
    }
    else
    {
        int alkuX = siirto.getAlkuruutu().getSarake();
        int alkuY = siirto.getAlkuruutu().getRivi();
        
        int loppuX = siirto.getLoppuruutu().getSarake();
        int loppuY = siirto.getLoppuruutu().getRivi();
        
        Nappula* nappulaPtr = lauta[alkuY][alkuX];
        
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
        
        lauta[loppuY][loppuX] = nappulaPtr;
        lauta[alkuY][alkuX] = NULL;
    }
    
    // Kaikki muut siirrot
    
    
    //Ottaa siirron alkuruudussa olleen nappulan talteen
    
    
    //Laittaa talteen otetun nappulan uuteen ruutuun
    
    
    // Tarkistetaan oliko sotilaan kaksoisaskel
    // (asetetaan kaksoisaskel-lippu)
    
    // Ohestalyönti on tyhjään ruutuun. Vieressä oleva (sotilas) poistetaan.
    
    //// Katsotaan jos nappula on sotilas ja rivi on päätyrivi niin ei vaihdeta nappulaa
    ////eli alkuruutuun laitetaan null ja loppuruudussa on jo kliittymän laittama nappula MIIKKA, ei taida minmaxin kanssa hehkua?
    
    //
    ////muissa tapauksissa alkuruutuun null ja loppuruutuun sama alkuruudusta lähtenyt nappula
    
    // katsotaan jos liikkunut nappula on kuningas niin muutetaan onkoKuningasLiikkunut arvo (molemmille väreille)
    
    // katsotaan jos liikkunut nappula on torni niin muutetaan onkoTorniLiikkunut arvo (molemmille väreille ja molemmille torneille)
    
    //päivitetään _siirtovuoro
    
    _siirtovuoro = 1 - _siirtovuoro;
}

int Asema::getSiirtovuoro() const { return _siirtovuoro; }
void Asema::setSiirtovuoro(int vuoro) {
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
double Asema::evaluoi()
{
    return 0;
    
    //kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta
    
    //1. Nappuloiden arvo
    
    //2. Kuningas turvassa
    
    //3. Arvosta keskustaa
    
    // 4. Arvosta linjoja
    
}

double Asema::laskeNappuloidenArvo(int vari)
{
    return 0;
    
}

bool Asema::onkoAvausTaiKeskipeli(int vari)
{
    return 0;
    // Jos upseereita 3 tai vähemmän on loppupeli
    // mutta jos daami laudalla on loppueli vasta kun kuin vain daami jäljellä
    
    //Jos vari on 0 eli valkoiset
    //niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1
}

double Asema::nappuloitaKeskella(int vari)
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

double Asema::linjat(int vari)
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

bool Asema::onkoRuutuUhattu(Ruutu* ruutu, int vastustajanVari)
{
    // Tarkistetaan siirtojen laillisuus. Eli asettaako siirto kunkinaan uhatuksi.
    
    // Valkoinen = 0, musta = 1.
    
    int x = ruutu->getSarake();
    int y = ruutu->getRivi();
    
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

void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>& lista, int vari)
{
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista) {
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (lauta[y][x] != NULL && lauta[y][x]->getVari() == getSiirtovuoro()) {
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
    //for (int y = 0; y < 8; y++)
    //{
    //    for (int x = 0; x < 8; x++)
    //    {
    //        if (lauta[y][x] != NULL) {
    //            Ruutu ruutu(x, y);
    //            lauta[y][x]->annaSiirrot(lista, ruutu, *this, lauta[y][x]->getVari());

    //            // Tallennetaan kuninkaiden ruudut.
    //            // Valkea kuningas
    //            if (lauta[y][x] == &vk)
    //            {
    //                vkRuutu.setRivi(y);
    //                vkRuutu.setSarake(x);
    //            }
    //            // Musta kuningas
    //            if (lauta[y][x] == &mk)
    //            {
    //                mkRuutu.setRivi(y);
    //                mkRuutu.setSarake(x);
    //            }
    //        }
    //    }
    //}

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
                Ruutu ruutuM(xAlku, yAlku);
                // Jos ruutu ei uhattu, lisätään laillisten siirtojen listaan.
                if (!onkoRuutuUhattu(&ruutuM, 0))
                {
                    auto listaFront = lista.begin();
                    std::advance(listaFront, index);

                    mSiirrot.push_back(*listaFront);
                    //lauta[yAlku][xAlku]->annaSiirrot(mSiirrot, ruutuM, uusiAsema, lauta[yAlku][xAlku]->getVari());
                }
            }
        }
        // Mustan siirtovuoro, tsekataan valkoisen siirrot.
        if (_siirtovuoro == 0)
        {
            if (lauta[yAlku][xAlku]->getVari() == 0)
            {
                Ruutu ruutuV(xAlku, yAlku);                
                // Jos ruutu ei uhattu, lisätään laillisten siirtojen listaan.
                if (!onkoRuutuUhattu(&ruutuV, 1))
                {
                    auto listaFront = lista.begin();
                    std::advance(listaFront, index);

                    vSiirrot.push_back(*listaFront);
                    //lauta[yAlku][xAlku]->annaSiirrot(vSiirrot, ruutuV, uusiAsema, lauta[yAlku][xAlku]->getVari());

                }
            }
        }

        //uusiAsema.paivitaAsema(s);
        index++;
    }

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

    //// Helpottamaan valkoisen siirtojen havainnollistamista.
    //std::cout << "Valkoisen lailliset siirrot: \n";
    //bool ruudutV[8][8] = { false };

    //for (auto& siirto : vSiirrot)
    //{
    //    int x = siirto.getLoppuruutu().getSarake();
    //    int y = siirto.getLoppuruutu().getRivi();
    //    ruudutV[y][x] = true;
    //}

    //for (int y = 7; y >= 0; y--)
    //{
    //    std::cout << std::to_string(y + 1) << " ";
    //    for (int x = 0; x < 8; x++)
    //    {
    //        if (ruudutV[y][x]) {
    //            std::cout << "v";
    //        }
    //        else {
    //            std::cout << "-";
    //        }
    //    }
    //    std::cout << "\n";
    //}

    //std::cout << "  abcdefgh\n";


    //// Helpottamaan mustan siirtojen havainnointia.
    //std::cout << "Mustan lailliset siirrot: \n";
    //bool ruudutM[8][8] = { false };

    //for (auto& siirto : mSiirrot)
    //{
    //    int x = siirto.getLoppuruutu().getSarake();
    //    int y = siirto.getLoppuruutu().getRivi();
    //    ruudutM[y][x] = true;
    //}

    //for (int y = 7; y >= 0; y--)
    //{
    //    std::cout << std::to_string(y + 1) << " ";
    //    for (int x = 0; x < 8; x++)
    //    {
    //        if (ruudutM[y][x]) {
    //            std::cout << "m";
    //        }
    //        else {
    //            std::cout << "-";
    //        }
    //    }
    //    std::cout << "\n";
    //}

    //std::cout << "  abcdefgh\n";

    // Apulistojen tyhjennys.
    vSiirrot.clear();
    mSiirrot.clear();
}
