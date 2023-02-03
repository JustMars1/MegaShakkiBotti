#include "vastustaja.h"

using namespace std;

Vastustaja::Vastustaja(string nimimerkki) {
	this->_nimimerkki = nimimerkki;
}

string Vastustaja::getNimimerkki() {
	return _nimimerkki;
}
