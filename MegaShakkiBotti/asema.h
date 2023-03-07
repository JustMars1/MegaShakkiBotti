#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <array>
#include <mutex>

#include "minmaxpaluu.h"
#include "siirto.h"
#include "nappula.h"

// Asema sis‰lt‰‰ kaiken tarvittavan informaation pelitilanteen kuvaamiseksi
// (nappuloiden sijainti, siirtovuoro, linnoitusoikeudet jne.).
class Asema
{
public:    
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
    
    static std::array<Nappula*, NappulaKoodiLkm> nappulat;
    
    static const float maxArvo;
    
    // Pelilauta sis‰lt‰‰ osoittimet kunkin ruudun nappula-olioon (nullptr/NULL/0 jos ruutu on tyhj‰).
    // Public-m‰‰reell‰, koska t‰t‰ k‰ytet‰‰n paljon muualla.
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
    
    void annaLaillisetSiirrot(std::vector<Siirto>& siirrot) const;
    std::vector<Siirto> annaLaillisetSiirrot() const;
private:
    // Lis‰informaatio pelitilanteesta.
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
    bool onkoAvausTaiKeskipeli(int vari);
    float nappuloitaKeskella() const;
    float linjat(int vari) const;
    bool onkoRuutuUhattu(const Ruutu& ruutu, int vastustajanVari) const;
    void annaLinnoitusSiirrot(std::vector<Siirto>& siirrot, int vari) const;
    
    // Karsii siirrot, jotka j‰tt‰v‰t oman K:n shakkiin.
    void huolehdiKuninkaanShakeista(std::vector<Siirto>& siirrot) const;
};
