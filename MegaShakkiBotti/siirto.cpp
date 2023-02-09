#include "siirto.h"

Siirto::Siirto(Ruutu alku, Ruutu loppu)
: _alkuRuutu(alku)
, _loppuRuutu(loppu)
, _lyhytLinna{false}
, _pitkaLinna{false}
, miksikorotetaan{nullptr} {}

Siirto::Siirto(bool lyhytLinna, bool pitkaLinna)
: _lyhytLinna{lyhytLinna}
, _pitkaLinna{pitkaLinna}
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksikorotetaan{nullptr} {}

Siirto::Siirto()
: _lyhytLinna{ false }
, _pitkaLinna{ false }
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksikorotetaan{ nullptr } {}

const Ruutu& Siirto::getAlkuruutu() const { return _alkuRuutu; }
const Ruutu& Siirto::getLoppuruutu() const { return _loppuRuutu; }
bool Siirto::onkoLyhytLinna() const { return _lyhytLinna; }
bool Siirto::onkoPitkaLinna() const { return _pitkaLinna; }
