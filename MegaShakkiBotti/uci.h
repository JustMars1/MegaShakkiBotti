#pragma once
#include "peli.h"
#include "kayttoliittyma.h"
#include "ajastin.h"

class UCI
{
public:
    Asema _asema;
    void uciLoop();
    
    const char* aloitusFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};

