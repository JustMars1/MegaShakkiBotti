#include <list>
#include <string>
#include "asema.h"
#include "nappula.h"

#include <iostream>

using namespace std;

Nappula::Nappula(string merkki, int vari, NappulaKoodi koodi)
: _merkki(merkki)
, _vari{vari}
, _koodi{koodi} {}

NappulaKoodi Nappula::getKoodi() const { return _koodi; }
int Nappula::getVari() const { return _vari; }
const string& Nappula::getMerkki() const { return _merkki; }

void Nappula::setKoodi(NappulaKoodi koodi)
{
    _koodi = koodi;
}

void Nappula::setVari(int vari)
{
    _vari = vari;
}

void Nappula::setMerkki(const string& merkki)
{
    _merkki = merkki;
}

void Torni::annaSiirrot(list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x = ruutu.getSarake();
    int y = ruutu.getRivi();
    
    // Oikea
    for (int i = x + 1; i < 8; i++)
    {
        if (asema.lauta[y][i] == NULL)
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
        if (asema.lauta[y][i] == NULL)
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
    
    // Ylös
    for (int i = y + 1; i < 8; i++)
    {
        if (asema.lauta[i][x] == NULL)
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
        if (asema.lauta[i][x] == NULL)
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
    
    // tornitus:
    
    
    
}

void Ratsu::annaSiirrot(list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x = ruutu.getSarake();
    int y = ruutu.getRivi();
    
    // suhteellinen sijainti mista ruudusta tahansa
    int x_sarake[] = { -1, 1, 1, -1, 2, -2, 2, -2 };
    int y_rivi[] = { 2, 2, -2, -2, 1, 1, -1, -1 };
    
    for (int i = 0; i < 8; i++)
    {
        // valkea ratsu
        int xv = x + x_sarake[i];
        int yv = y + y_rivi[i];
        
        // musta ratsu
        int xm = x - x_sarake[i];
        int ym = y - y_rivi[i];
        
        // onko valkean ratsun siirto laudan ulkopuolella
        if ((xv >= 0 && xv < 8) && (yv >= 0 && yv < 8))
        {
            // tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[yv][xv] == NULL)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xv, yv)));
            }
            
            else if (asema.lauta[yv][xv] != NULL && asema.lauta[yv][xv]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xv, yv)));
            }
        }
        
        // onko mustan ratsun siirto laudan ulkopuolella
        if ((xm >= 0 && xm < 8) && (ym >= 0 && ym < 8))
        {
            //tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[ym][xm] == NULL)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xm, ym)));
            }
            
            else if (asema.lauta[ym][xm] != NULL && asema.lauta[ym][xm]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xm, ym)));
            }
        }
    }
}

void Lahetti::annaSiirrot(list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    int x0 = ruutu.getSarake();
    int y0 = ruutu.getRivi();
    
    // Oikea-Ylä:
    for (int i = 1; i < 8; i++)
    {
        int y = y0 + i;
        int x = x0 + i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x] == NULL)
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
    
    // Vasen- Ylä:
    for (int i = 1; i < 8; i++)
    {
        int y = y0 + i;
        int x = x0 - i;
        
        if (y < 0 || y > 7 || x < 0 || x > 7)
        {
            break;
        }
        
        if (asema.lauta[y][x] == NULL)
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
        
        if (asema.lauta[y][x] == NULL)
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
        
        if (asema.lauta[y][x] == NULL)
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

Daami::Daami(string merkki, int vari, NappulaKoodi koodi)
: Nappula(merkki, vari, koodi)
, Torni(merkki, vari, koodi)
, Lahetti(merkki, vari, koodi) {}

void Daami::annaSiirrot(list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    Torni::annaSiirrot(lista, ruutu, asema, vari);
    Lahetti::annaSiirrot(lista, ruutu, asema, vari);
}

void Kuningas::annaSiirrot(list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
{
    /*perusidea on että kaikki viereiset ruudut ovat sallittuja. kuten tornilla ja lähetillä,
     oman nappulan päälle ei voi mennä ja vastustajan nappulan voi syödä.
     
     Kaikki muu kuninkaaseen liittyvä tarkistus tehdään eri paikassa*/
    int x = ruutu.getSarake();
    int y = ruutu.getRivi();
    
    // Suhteellinen sijainti mista ruudusta tahansa.
    int x_sarake[] = { -1, 0, 1, 1, 1, 0, -1, -1};
    int y_rivi[] = { 1, 1, 1, 0, -1, -1, -1, 0};
    
    for (int i = 0; i < 8; i++)
    {
        // Valkea kuningas
        int xv = x + x_sarake[i];
        int yv = y + y_rivi[i];
        
        // Musta kuningas
        int xm = x - x_sarake[i];
        int ym = y - y_rivi[i];
        
        // Onko valkean kuninkaan siirto laudan ulkopuolella
        if ((xv >= 0 && xv < 8) && (yv >= 0 && yv < 8))
        {
            // Tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[yv][xv] == NULL)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xv, yv)));
            }
            else if (asema.lauta[yv][xv] != NULL && asema.lauta[yv][xv]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xv, yv)));
            }
        }
        
        // Onko mustan kuninkaan siirto laudan ulkopuolella
        if ((xm >= 0 && xm < 8) && (ym >= 0 && ym < 8))
        {
            // Tarkistetaan, voiko ruutuun liikkua
            if (asema.lauta[ym][xm] == NULL)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xm, ym)));
            }
            else if (asema.lauta[ym][xm] != NULL && asema.lauta[ym][xm]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(xm, ym)));
            }
        }
    }
}

void Sotilas::annaSiirrot(list<Siirto>& lista, const Ruutu& ruutu, const Asema& asema, int vari)
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
        
        if (asema.lauta[y][x0] != NULL)
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
            if (asema.lauta[y][x] != NULL && asema.lauta[y][x]->getVari() != vari)
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
            if (asema.lauta[y][x] != NULL && asema.lauta[y][x]->getVari() != vari)
            {
                lista.push_back(Siirto(ruutu, Ruutu(x, y)));
            }
        }
    }
    
    //Ohestalyönnin siirrot
    if (asema.kaksoisAskel != -1)
    {
        if (vari == 0 && y0 == 4)
        {
            if (asema.lauta[4][asema.kaksoisAskel] && asema.lauta[4][asema.kaksoisAskel]->getKoodi() == MS)
                lista.push_back(Siirto(ruutu, Ruutu(asema.kaksoisAskel, 5)));
        }
        
        if (vari == 1 && y0 == 3)
        {
            if (asema.lauta[3][asema.kaksoisAskel] && asema.lauta[3][asema.kaksoisAskel]->getKoodi() == VS)
                lista.push_back(Siirto(ruutu, Ruutu(asema.kaksoisAskel, 2)));
        }
    }
}

void Sotilas::lisaaSotilaanKorotukset(const Siirto& siirto, list<Siirto>& lista, const Asema& asema)
{
    if (siirto.getLoppuruutu().getRivi() == 7)
    {
        Siirto daami = siirto;
        daami.miksikorotetaan = &Asema::vd;
        lista.push_back(daami);
        
        Siirto torni = siirto;
        torni.miksikorotetaan = &Asema::vt;
        lista.push_back(torni);
        
        Siirto lahetti = siirto;
        lahetti.miksikorotetaan = &Asema::vl;
        lista.push_back(lahetti);
        
        Siirto ratsu = siirto;
        ratsu.miksikorotetaan = &Asema::vr;
        lista.push_back(ratsu);
    }
    else if (siirto.getLoppuruutu().getRivi() == 0)
    {
        Siirto daami = siirto;
        daami.miksikorotetaan = &Asema::md;
        lista.push_back(daami);
        
        Siirto torni = siirto;
        torni.miksikorotetaan = &Asema::mt;
        lista.push_back(torni);
        
        Siirto lahetti = siirto;
        lahetti.miksikorotetaan = &Asema::ml;
        lista.push_back(lahetti);
        
        Siirto ratsu = siirto;
        ratsu.miksikorotetaan = &Asema::mr;
        lista.push_back(ratsu);
    }
}
