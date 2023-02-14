#pragma once
#include "asema.h"
#include "nappula.h"
#include "peli.h"
#include "siirto.h"
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include "minmaxpaluu.h"

// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.

class Kayttoliittyma
{
public:
    static Kayttoliittyma& getInstance();
    
    void piirraLauta(const std::list<Siirto>& siirrot = {});
    Siirto annaVastustajanSiirto();
    int kysyVastustajanVari();
    
    static void tulostaVirhe(std::string virhe);
    
    const Asema& getAsema() const;
    Asema& getAsema();

    void lisaaTehtava(std::function<void()> tehtava);
private:
    std::vector<std::thread> saikeet;
    Kayttoliittyma();
    void saieLuuppi();
    std::queue<std::function<void()>> tehtavat;
	Asema _asema;
};
