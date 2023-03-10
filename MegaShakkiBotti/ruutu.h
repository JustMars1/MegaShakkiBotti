#pragma once
#include <iostream>
#include <cstdint>

class Asema;
class Nappula;

// Shakkilaudan ruutu tietyss? (rivi, sarake) -koordinaatissa.
class Ruutu
{
public:
    Ruutu(int sarake, int rivi);
    
    int getRivi() const;
    int getSarake() const;
    
    void setRivi(int);
    void setSarake(int);
    
    bool ok() const;
    
    Nappula* getNappula(const Asema& asema) const;
    
    bool operator==(const Ruutu& rhs) const;
private:
    int _sarake;
    int _rivi;
};

std::ostream& operator<<(std::ostream& os, const Ruutu& ruutu);
