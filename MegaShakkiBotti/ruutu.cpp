#include "ruutu.h"
#include "asema.h"

Ruutu::Ruutu(int sarake, int rivi)
: _sarake{sarake}
, _rivi{rivi} {}

int Ruutu::getRivi() const { return _rivi; }
int Ruutu::getSarake() const { return _sarake; }

void Ruutu::setRivi(int rivi)
{
    _rivi = rivi;
}

void Ruutu::setSarake(int sarake)
{
    _sarake = sarake;
}

bool Ruutu::operator==(const Ruutu& rhs) const
{
    return _sarake == rhs._sarake && _rivi == rhs._rivi;
}

bool Ruutu::ok() const
{
    return _sarake >= 0 && _sarake < 8 && _rivi >= 0 && _rivi < 8;
}

Nappula* Ruutu::getNappula(const Asema& asema) const
{
    return asema.lauta[_rivi][_sarake];
}

std::ostream& operator<<(std::ostream& os, const Ruutu& ruutu)
{
    os << "abcdefgh"[ruutu.getSarake()] << ruutu.getRivi() + 1;
    return os;
}
