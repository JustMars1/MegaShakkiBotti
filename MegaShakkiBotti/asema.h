#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <array>
#include <mutex>

#include "minmaxpaluu.h"
#include "siirto.h"
#include "nappula.h"

// Asema sisältää kaiken tarvittavan informaation pelitilanteen kuvaamiseksi
// (nappuloiden sijainti, siirtovuoro, linnoitusoikeudet jne.).
class Asema
{
public:
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
    
    // Ohestalyöntiä varten (-1 = sotilaan kaksoisaskelta ei tapahtunut edellisellä siirrolla).
    
    static std::array<Nappula*, NappulaKoodiLkm> nappulat;
    
    static const float maxArvo;
    
    std::array<std::array<Nappula*, 8>, 8> lauta;
    
    Asema(std::array<std::array<Nappula*, 8>, 8> lauta = {
        &vt, &vr, &vl, &vd, &vk, &vl, &vr, &vt,
        &vs, &vs, &vs, &vs, &vs, &vs, &vs, &vs,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &ms, &ms, &ms, &ms, &ms, &ms, &ms, &ms,
        &mt, &mr, &ml, &md, &mk, &ml, &mr, &mt
    });
    
    int getSiirtovuoro() const;
    void setSiirtovuoro(int vuoro);

    const Ruutu& getValkeanKuninkaanRuutu() const;
    const Ruutu& getMustanKuninkaanRuutu() const;
    
    int getKaksoisaskelSarake() const;
    void setKaksoisaskelSarake(int sarake);
    
    bool onkoValkeaKuningasLiikkunut() const;
    bool onkoMustaKuningasLiikkunut() const;
    bool onkoValkeaDTliikkunut() const;
    bool onkoValkeaKTliikkunut() const;
    bool onkoMustaDTliikkunut() const;
    bool onkoMustaKTliikkunut() const;
    
    void setValkeaKuningasLiikkunut(bool onkoLiikkunut);
    void setMustaKuningasLiikkunut(bool onkoLiikkunut);
    void setValkeaDTliikkunut(bool onkoLiikkunut);
    void setValkeaKTliikkunut(bool onkoLiikkunut);
    void setMustaDTliikkunut(bool onkoLiikkunut);
    void setMustaKTliikkunut(bool onkoLiikkunut);
    
    void setValkeanKuninkaanRuutu(const Ruutu& ruutu);
    void setMustanKuninkaanRuutu(const Ruutu& ruutu);
    
    void paivitaAsema(const Siirto& siirto);
    bool tarkistaSiirto(const Siirto& siirto) const;
    bool onkoRuutuUhattu(const Ruutu& ruutu, int vastustajanVari) const;
    
    float evaluoi() const;
    MinMaxPaluu maxi(int syvyys) const;
    MinMaxPaluu mini(int syvyys) const;
    MinMaxPaluu minimax(int syvyys) const;
    MinMaxPaluu minimaxAsync(int syvyys) const;
    
    MinMaxPaluu alphabetaMaxi(int syvyys, float alpha, float beta) const;
    MinMaxPaluu alphabetaMini(int syvyys, float alpha, float beta) const;
    MinMaxPaluu alphabetaMinimax(int syvyys) const;
    MinMaxPaluu alphabetaMinimaxAsync(int syvyys) const;
    
    std::vector<size_t> jaaSiirrotSaikeidenKesken(size_t siirtoLkm) const;
    
    std::vector<Siirto> annaLaillisetSiirrot() const;
private:
    // Lisäinformaatio pelitilanteesta.
    int _siirtovuoro;                    // 0 = valkea, 1 = musta.
    bool _onkoValkeaKuningasLiikkunut;    // Linnoitus ei ole sallittu, jos kuningas on liikkunut.
    bool _onkoMustaKuningasLiikkunut;    // Linnoitus ei ole sallittu, jos kuningas on liikkunut.
    bool _onkoValkeaDTliikkunut;        // Linnoitus ei ole sallittu, jos daamisivustan torni on liikkunut.
    bool _onkoValkeaKTliikkunut;        // Linnoitus ei ole sallittu, jos kuningassivustan torni on liikkunut.
    bool _onkoMustaDTliikkunut;            // Linnoitus ei ole sallittu, jos daamisuvustan torni on liikkunut.
    bool _onkoMustaKTliikkunut;            // Linnoitus ei ole sallittu, jos kuningassivustan torni on liikkunut.
    
    Ruutu _valkeanKuninkaanRuutu;
    Ruutu _mustanKuninkaanRuutu;
    
    int _kaksoisaskelSarake;
    
    float laskeNappuloidenArvo() const;
    bool onkoAvausTaiKeskipeli(int vari) const;
    float nappuloitaKeskella() const;
    float linjat() const;
    
    void annaLinnoitusSiirrot(std::vector<Siirto>& siirrot, int vari) const;
    
    // Karsii siirrot, jotka jättävät oman K:n shakkiin.
    void huolehdiKuninkaanShakeista(std::vector<Siirto>& siirrot) const;
};

std::ostream& operator<<(std::ostream& os, const Asema& asema);
