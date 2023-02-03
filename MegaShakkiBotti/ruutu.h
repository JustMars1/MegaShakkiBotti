#pragma once


// Shakkilaudan ruutu tietyssä (rivi, sarake) -koordinaatissa.
class Ruutu {
public:
	Ruutu(int sarake, int rivi);
    
	int getRivi() const;
	int getSarake() const;
    
	void setRivi(int);
	void setSarake(int);
private:
	int _sarake;
	int _rivi;
};
