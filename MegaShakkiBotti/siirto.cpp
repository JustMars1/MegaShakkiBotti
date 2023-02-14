#include "siirto.h"
#include "kayttoliittyma.h"

Siirto::Siirto(Ruutu alku, Ruutu loppu)
: _alkuRuutu(alku)
, _loppuRuutu(loppu)
, _lyhytLinna{false}
, _pitkaLinna{false}
, miksiKorotetaan{nullptr} {}

Siirto::Siirto(bool lyhytLinna, bool pitkaLinna)
: _lyhytLinna{lyhytLinna}
, _pitkaLinna{pitkaLinna}
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksiKorotetaan{nullptr} {}

Siirto::Siirto()
: _lyhytLinna{ false }
, _pitkaLinna{ false }
, _alkuRuutu(0, 0)
, _loppuRuutu(0, 0)
, miksiKorotetaan{ nullptr } {}

bool Siirto::operator==(const Siirto& rhs) const
{
    return _alkuRuutu == rhs._alkuRuutu && _loppuRuutu == rhs._loppuRuutu && _lyhytLinna == rhs._lyhytLinna && _pitkaLinna == rhs._pitkaLinna;
}

const Ruutu& Siirto::getAlkuruutu() const { return _alkuRuutu; }
const Ruutu& Siirto::getLoppuruutu() const { return _loppuRuutu; }
bool Siirto::onkoLyhytLinna() const { return _lyhytLinna; }
bool Siirto::onkoPitkaLinna() const { return _pitkaLinna; }

std::ostream& operator<<(std::ostream& os, const Siirto& siirto)
{
    if (siirto.onkoLyhytLinna())
    {
        os << "O-O-O";
    }
    else if (siirto.onkoLyhytLinna())
    {
        os << "O-O";
    }
    else
    {
        os << siirto.getAlkuruutu() << "-" << siirto.getLoppuruutu();
    }
    
    return os;
}
