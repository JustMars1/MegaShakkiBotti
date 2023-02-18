#pragma once

#include <list>
#include <string>
#include <unordered_map>

#include "minmaxpaluu.h"
#include "siirto.h"
#include "nappula.h"

// Asema sis‰lt‰‰ kaiken tarvittavan informaation pelitilanteen kuvaamiseksi
// (nappuloiden sijainti, siirtovuoro, linnoitusoikeudet jne.).
class Asema
{
public:
    // Pelilauta sis‰lt‰‰ osoittimet kunkin ruudun nappula-olioon (nullptr/NULL/0 jos ruutu on tyhj‰).
    // Public-m‰‰reell‰, koska t‰t‰ k‰ytet‰‰n paljon muualla.
    Nappula* lauta[8][8];
    int kaksoisaskel;
    Ruutu _valkeaKuningasRuutu;
    Ruutu _mustaKuningasRuutu;
    
    // Nappula-oliot. Huomaa, ett‰ samaa nappulaa voidaan k‰ytt‰‰ useissa eri ruuduissa.
    // M‰‰ritelty static-m‰‰reell‰, joten nappulat ovat kaikkien lauta-olioiden "yhteisk‰ytˆss‰"
    // (suorituskyvyn vuoksi).
    
    static Kuningas vk;
    static Daami vd;
    static Torni vt;
    static Lahetti vl;
    static Ratsu vr;
    static Sotilas vs;
    
    static Kuningas mk;
    static Daami md;
    static Torni mt;
    static Lahetti ml;
    static Ratsu mr;
    static Sotilas ms;
    
    // Ohestalyˆnti‰ varten (-1 = sotilaan kaksoisaskelta ei tapahtunut edellisell‰ siirrolla).
    
    static const std::unordered_map<char, Nappula*> valkeaNappulaMap;
    static const std::unordered_map<char, Nappula*> mustaNappulaMap;
    
    static const std::unordered_map<char, Nappula*> fenNappulaMap;
    
    static const float maxArvo;
    
    Asema();                                                // Asettaa alkuaseman.
    void paivitaAsema(const Siirto& siirto);                // P‰ivitt‰‰ aseman annetulla siirrolla.
    
    bool tarkistaSiirto(const Siirto& siirto) const;
    
    float evaluoi() const;                                                // Aseman numeerinen arviointi.
    MinMaxPaluu maxi(int syvyys) const;                                    // Minimax (max:n siirtovuoro).
    MinMaxPaluu mini(int syvyys) const;                                    // Minimax (min:n siirtovuoro).
    MinMaxPaluu minimax(int syvyys) const;                                // Minimax-algoritmi.
    MinMaxPaluu minimaxAsync(int syvyys) const;                            // Minimax-algoritmi.
    void annaLaillisetSiirrot(std::vector<Siirto>& siirrot) const;    // Siirtogeneraattori.
    
    int getSiirtovuoro() const;
    void setSiirtovuoro(int vuoro);
    
    bool onkoValkeaKuningasLiikkunut() const;
    bool onkoMustaKuningasLiikkunut() const;
    bool onkoValkeaDTliikkunut() const;
    bool onkoValkeaKTliikkunut() const;
    bool onkoMustaDTliikkunut() const;
    bool onkoMustaKTliikkunut() const;
    
    void setValkeaKuningasLiikkunut(bool valkeaKuningasLiikkunut);
    void setMustaKuningasLiikkunut(bool mustaKuningasLiikkunut);
    void setValkeaDTliikkunut(bool valkeaDTliikkunut);
    void setValkeaKTliikkunut(bool valkeaKTliikkunut);
    void setMustaDTliikkunut(bool mustaDTliikkunut);
    void setMustaKTliikkunut(bool mustaKTliikkunut);
private:
    // Lis‰informaatio pelitilanteesta.
    int _siirtovuoro;                    // 0 = valkea, 1 = musta.
    bool _onkoValkeaKuningasLiikkunut;    // Linnoitus ei ole sallittu, jos kuningas on liikkunut.
    bool _onkoMustaKuningasLiikkunut;    // Linnoitus ei ole sallittu, jos kuningas on liikkunut.
    bool _onkoValkeaDTliikkunut;        // Linnoitus ei ole sallittu, jos daamisivustan torni on liikkunut.
    bool _onkoValkeaKTliikkunut;        // Linnoitus ei ole sallittu, jos kuningassivustan torni on liikkunut.
    bool _onkoMustaDTliikkunut;            // Linnoitus ei ole sallittu, jos daamisuvustan torni on liikkunut.
    bool _onkoMustaKTliikkunut;            // Linnoitus ei ole sallittu, jos kuningassivustan torni on liikkunut.
    
    float laskeNappuloidenArvo() const;
    bool onkoAvausTaiKeskipeli(int vari);
    float nappuloitaKeskella(int vari);
    float linjat(int vari);
    bool onkoRuutuUhattu(const Ruutu& ruutu, int vastustajanVari) const;
    void annaLinnoitusSiirrot(std::vector<Siirto>& siirrot, int vari) const;
    
    // Karsii siirrot, jotka j‰tt‰v‰t oman K:n shakkiin.
    void huolehdiKuninkaanShakeista(std::vector<Siirto>& siirrot) const;
};
