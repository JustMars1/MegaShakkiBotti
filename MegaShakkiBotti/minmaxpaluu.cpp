#include "minmaxpaluu.h"

MinMaxPaluu::MinMaxPaluu(float arvo, Siirto siirto)
    : evaluointiArvo{ arvo }
    , _parasSiirto(siirto) {}

MinMaxPaluu::MinMaxPaluu() : evaluointiArvo{ 0 }, _parasSiirto(Siirto()) {}

std::ostream& operator<<(std::ostream& os, const MinMaxPaluu& minmaxpaluu)
{
    os << minmaxpaluu.evaluointiArvo << " " << minmaxpaluu._parasSiirto;
    return os;
}
