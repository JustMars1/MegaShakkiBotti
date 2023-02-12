#pragma once
#include <iostream>

// Shakkilaudan ruutu tietyssä (rivi, sarake) -koordinaatissa.
class Ruutu
{
public:
	Ruutu(int sarake, int rivi);
    
	int getRivi() const;
	int getSarake() const;
    
	void setRivi(int);
	void setSarake(int);
    
    bool ok() const;
    
    bool operator==(const Ruutu& rhs) const;
private:
	int _sarake;
	int _rivi;
};

std::ostream& operator<<(std::ostream& os, const Ruutu& ruutu);
