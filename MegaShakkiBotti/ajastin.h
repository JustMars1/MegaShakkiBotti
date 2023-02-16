#pragma once
#include <chrono>
#include <string_view>
#include <iostream>

struct Ajastin
{
    using Kello = std::chrono::steady_clock;
    
    Kello::time_point aloitusAika;
    std::string_view nimi;
    
    Ajastin(std::string_view nimi);
    
    ~Ajastin();
};
