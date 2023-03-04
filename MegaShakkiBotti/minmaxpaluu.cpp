#include "minmaxpaluu.h"

MinMaxPaluu::MinMaxPaluu(float arvo, Siirto siirto)
: evaluointiArvo{ arvo }
, parasSiirto(siirto) {}

MinMaxPaluu::MinMaxPaluu() : evaluointiArvo{0}, parasSiirto(Siirto()) {}

std::ostream& operator<<(std::ostream& os, const MinMaxPaluu& minmaxpaluu)
{
    os << minmaxpaluu.evaluointiArvo << " " << minmaxpaluu.parasSiirto;
    return os;
}
