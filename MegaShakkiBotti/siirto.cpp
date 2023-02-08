#include "siirto.h"

Siirto::Siirto(Ruutu alkuRuutu, Ruutu loppuRuutu)
: _alkuRuutu(alkuRuutu)
, _loppuRuutu(loppuRuutu)
, _lyhytLinna{false}
, _pitkaLinna{false}
, miksikorotetaan{nullptr} {}

Siirto::Siirto(bool lyhytLinna, bool pitkaLinna)
: _lyhytLinna{lyhytLinna}
, _pitkaLinna{pitkaLinna}
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksikorotetaan{nullptr} {}

const Ruutu& Siirto::getAlkuruutu() const { return _alkuRuutu; }
const Ruutu& Siirto::getLoppuruutu() const { return _loppuRuutu; }
bool Siirto::onkoLyhytLinna() const { return _lyhytLinna; }
bool Siirto::onkoPitkaLinna() const { return _pitkaLinna; }
