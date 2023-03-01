#include "kieli.h"

const std::string& Kieli::get(const std::string& avain) const
{
    return kaannokset.at(avain);
}
