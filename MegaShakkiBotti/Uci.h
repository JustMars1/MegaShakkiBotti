#pragma once
#include "peli.h"
#include "kayttoliittyma.h"
#include "ajastin.h"
using namespace std;

class Uci
{
public:

	void uciLoop();
	void positionStartpos();

	const char* aloitusFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Asema _asema;
private:

};

