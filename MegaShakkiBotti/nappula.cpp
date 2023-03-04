#include <list>
#include <string>
#include "asema.h"
#include "nappula.h"

#include <iostream>
#include "kayttoliittyma.h"

using namespace std;

Nappula::Nappula(const string& merkki, int vari, NappulaKoodi koodi, float arvo, int maxSiirrot, const std::string& siirtoMerkkiAvain, const string& fenMerkki)
: _merkki(merkki)
, _vari{vari}
, _koodi{koodi}
, _arvo{arvo}
, _maxSiirrot{maxSiirrot}
, _siirtoMerkkiAvain{siirtoMerkkiAvain}
, _fenMerkki{fenMerkki} {}

Daami::Daami(const string& merkki, int vari, NappulaKoodi koodi, float arvo, int maxSiirrot, const std::string& siirtoMerkkiAvain, const string& fenMerkki)
: Nappula(merkki, vari, koodi, arvo, maxSiirrot, siirtoMerkkiAvain, fenMerkki)
, Torni(merkki, vari, koodi, arvo, maxSiirrot, siirtoMerkkiAvain, fenMerkki)
, Lahetti(merkki, vari, koodi, arvo, maxSiirrot, siirtoMerkkiAvain, fenMerkki) {}

NappulaKoodi Nappula::getKoodi() const { return _koodi; }
int Nappula::getVari() const { return _vari; }
float Nappula::getArvo() const { return _arvo; }

const string& Nappula::getLautaMerkki() const { return _merkki; }

const std::string& Nappula::getSiirtoMerkki() const
{
    return getSiirtoMerkki(Kayttoliittyma::getInstance().getOnkoUCI());
}

const std::string& Nappula::getSiirtoMerkki(bool uci) const
{
    if (uci)
    {
        return getFENMerkki();
    }
    
    return Kayttoliittyma::getInstance().getKieli().get(_siirtoMerkkiAvain);
}

const string& Nappula::getFENMerkki() const { return _fenMerkki; }

int Nappula::getMaxSiirrot() const { return _maxSiirrot; }

void Torni::annaSiirrot(vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x = ruutu.getSarake();
    int y = ruutu.getRivi();
    
    // Oikea
    for (int i = x + 1; i < 8; i++)
    {
        if (asema.lauta[y][i] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(i, y)));
        }
        else
        {
            if (asema.lauta[y][i]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(i, y)));
            }
            
            break;
        }
    }
    
    // Vasen
    for (int i = x - 1; i >= 0; i--)
    {
        if (asema.lauta[y][i] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(i, y)));
        }
        else
        {
            if (asema.lauta[y][i]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(i, y)));
            }
            
            break;
        }
    }
    
    // Yl�s
    for (int i = y + 1; i < 8; i++)
    {
        if (asema.lauta[i][x] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, i)));
        }
        else
        {
            if (asema.lauta[i][x]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, i)));
            }
            
            break;
        }
    }
    
    // Alas
    for (int i = y - 1; i >= 0; i--)
    {
        if (asema.lauta[i][x] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, i)));
        }
        else
        {
            if (asema.lauta[i][x]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, i)));
            }
            
            break;
        }
    }
}

void Ratsu::annaSiirrot(vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x0 = ruutu.getSarake();
    int y0 = ruutu.getRivi();
    
    // suhteellinen sijainti mista ruudusta tahansa
    int xSarake[8] = { -1, 1,  1, -1, 2, -2,  2, -2 };
    int yRivi[8] =   {  2, 2, -2, -2, 1,  1, -1, -1 };
    
    for (int i = 0; i < 8; i++)
    {
        int x = x0 + xSarake[i];
        int y = y0 + yRivi[i];
        
        // onko valkean ratsun siirto laudan ulkopuolella
        if ((x >= 0 && x < 8) && (y >= 0 && y < 8))
        {
            // tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[y][x] == nullptr)
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
            else if (asema.lauta[y][x] != nullptr && asema.lauta[y][x]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
        }
    }
}

void Lahetti::annaSiirrot(vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x0 = ruutu.getSarake();
    int y0 = ruutu.getRivi();
    
    // Oikea-Yl�:
    for (int i = 1; i < 8; i++)
    {
        int y = y0 + i;
        int x = x0 + i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, y)));
        }
        else
        {
            if (asema.lauta[y][x]->getVari() != vari )
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
            
            break;
        }
    }
    
    // Vasen- Yl�:
    for (int i = 1; i < 8; i++)
    {
        int y = y0 + i;
        int x = x0 - i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, y)));
        }
        else
        {
            if (asema.lauta[y][x]->getVari() != vari )
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
            
            break;
        }
    }
    
    // Oikea -Alas:
    for (int i = 1; i < 8; i++)
    {
        int y = y0 - i;
        int x = x0 + i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, y)));
        }
        else
        {
            if (asema.lauta[y][x]->getVari() != vari )
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
            
            break;
        }
    }
    
    // Vasen -Alas:
    for (int i = 1; i < 8; i++)
    {
        int y = y0 - i;
        int x = x0 - i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x] == nullptr)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, y)));
        }
        else
        {
            if (asema.lauta[y][x]->getVari() != vari )
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
            
            break;
        }
    }
}

void Daami::annaSiirrot(vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    Torni::annaSiirrot(lista, ruutu, asema, vari);
    Lahetti::annaSiirrot(lista, ruutu, asema, vari);
}

void Kuningas::annaSiirrot(vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    /*perusidea on ett� kaikki viereiset ruudut ovat sallittuja. kuten tornilla ja l�hetill�,
     oman nappulan p��lle ei voi menn� ja vastustajan nappulan voi sy�d�.
     
     Kaikki muu kuninkaaseen liittyv� tarkistus tehd��n eri paikassa*/
    int x = ruutu.getSarake();
    int y = ruutu.getRivi();
    
    // Suhteellinen sijainti mista ruudusta tahansa.
    int xSarake[] = { -1, 0, 1, 1, 1, 0, -1, -1};
    int yRivi[] = { 1, 1, 1, 0, -1, -1, -1, 0};
    
    for (int i = 0; i < 8; i++)
    {
        // Valkea kuningas
        int xv = x + xSarake[i];
        int yv = y + yRivi[i];
        
        // Musta kuningas
        int xm = x - xSarake[i];
        int ym = y - yRivi[i];
        
        // Onko valkean kuninkaan siirto laudan ulkopuolella
        if ((xv >= 0 && xv < 8) && (yv >= 0 && yv < 8))
        {
            // Tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[yv][xv] == nullptr)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xv, yv)));
            }
            else if (asema.lauta[yv][xv] != nullptr && asema.lauta[yv][xv]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xv, yv)));
            }
        }
        
        // Onko mustan kuninkaan siirto laudan ulkopuolella
        if ((xm >= 0 && xm < 8) && (ym >= 0 && ym < 8))
        {
            // Tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[ym][xm] == nullptr)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xm, ym)));
            }
            else if (asema.lauta[ym][xm] != nullptr && asema.lauta[ym][xm]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xm, ym)));
            }
        }
    }
}

void Sotilas::annaSiirrot(vector<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x0 = ruutu.getSarake();
    int y0 = ruutu.getRivi();
    
    int suunta = 1 - 2 * vari;
    
    int maxLiike = 1;
    if (vari == 0 && y0 == 1)
    {
        maxLiike++;
    }
    else if (y0 == 6)
    {
        maxLiike++;
    }
    
    for (int i = 1; i <= maxLiike; i++)
    {
        int y = y0 + suunta * i;
        
        if (y < 0 || y > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x0] != nullptr)
        {
            break;
        }
        
        Siirto siirto(ruutu, Ruutu(x0, y));
        if (y == 7 || y == 0)
        {
            lisaaSotilaanKorotukset(siirto, lista, asema);
        }
        else
        {
            lista.push_back(siirto);
        }
    }
    
    {
        int y = y0 + suunta;
        int x = x0 + 1;
        
        if (y >= 0 && y < 8 && x >= 0 && x < 8)
        {
            if (asema.lauta[y][x] != nullptr && asema.lauta[y][x]->getVari() != vari)
            {
                Siirto siirto(ruutu, Ruutu(x, y));
                if (y == 7 || y == 0)
                {
                    lisaaSotilaanKorotukset(siirto, lista, asema);
                }
                else
                {
                    lista.push_back(siirto);
                }
            }
        }
    }
    
    {
        int y = y0 + suunta;
        int x = x0 - 1;
        
        if (y >= 0 && y < 8 && x >= 0 && x < 8)
        {
            if (asema.lauta[y][x] != nullptr && asema.lauta[y][x]->getVari() != vari)
            {
                Siirto siirto(ruutu, Ruutu(x, y));
                if (y == 7 || y == 0)
                {
                    lisaaSotilaanKorotukset(siirto, lista, asema);
                }
                else
                {
                    lista.push_back(siirto);
                }
            }
        }
    }
    
    //Ohestaly�nnin siirrot
    
    int x = asema.getKaksoisaskelSarake();
    if (x != -1 && (x == x0 - 1 || x == x0 + 1))
    {
        if (vari == 0)
        {
            if (y0 == 4 && asema.lauta[4][x] == &Asema::ms)
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, 5)));
            }
        }
        else if (y0 == 3 && asema.lauta[3][x] == &Asema::vs)
        {
            lista.push_back(Siirto(ruutu, Ruutu(x, 2)));
        }
    }
}

void Sotilas::lisaaSotilaanKorotukset(const Siirto& siirto, vector<Siirto>& lista, const Asema& asema)
{
    if (siirto.getLoppuruutu().getRivi() == 7)
    {
        Siirto daami = siirto;
        daami.miksiKorotetaan = &Asema::vd;
        lista.push_back(daami);
        
        Siirto torni = siirto;
        torni.miksiKorotetaan = &Asema::vt;
        lista.push_back(torni);
        
        Siirto lahetti = siirto;
        lahetti.miksiKorotetaan = &Asema::vl;
        lista.push_back(lahetti);
        
        Siirto ratsu = siirto;
        ratsu.miksiKorotetaan = &Asema::vr;
        lista.push_back(ratsu);
    }
    else if (siirto.getLoppuruutu().getRivi() == 0)
    {
        Siirto daami = siirto;
        daami.miksiKorotetaan = &Asema::md;
        lista.push_back(daami);
        
        Siirto torni = siirto;
        torni.miksiKorotetaan = &Asema::mt;
        lista.push_back(torni);
        
        Siirto lahetti = siirto;
        lahetti.miksiKorotetaan = &Asema::ml;
        lista.push_back(lahetti);
        
        Siirto ratsu = siirto;
        ratsu.miksiKorotetaan = &Asema::mr;
        lista.push_back(ratsu);
    }
}
