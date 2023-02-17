#include "ruutu.h"

Ruutu::Ruutu(int8_t sarake, int8_t rivi)
: _sarake{sarake}
, _rivi{rivi} {}

int8_t Ruutu::getRivi() const { return _rivi; }
int8_t Ruutu::getSarake() const { return _sarake; }

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

std::ostream& operator<<(std::ostream& os, const Ruutu& ruutu)
{
    os << "abcdefgh"[ruutu.getSarake()] << static_cast<int>(ruutu.getRivi()) + 1;
    return os;
}
