#pragma once
#include <string>

// Vastustajan tiedot.
class Vastustaja
{
public:
	Vastustaja(std::string);
	std::string getNimimerkki();

private:
	std::string _nimimerkki;
};
