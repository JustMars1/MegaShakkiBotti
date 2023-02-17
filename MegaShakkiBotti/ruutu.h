#pragma once
#include <iostream>
#include <cstdint>

// Shakkilaudan ruutu tietyssä (rivi, sarake) -koordinaatissa.
class Ruutu
{
public:
	Ruutu(int8_t sarake, int8_t rivi);
    
	int8_t getRivi() const;
    int8_t getSarake() const;
    
	void setRivi(int);
	void setSarake(int);
    
    bool ok() const;
    
    bool operator==(const Ruutu& rhs) const;
private:
	int8_t _sarake;
    int8_t _rivi;
};

std::ostream& operator<<(std::ostream& os, const Ruutu& ruutu);
