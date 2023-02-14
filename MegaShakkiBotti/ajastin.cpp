#include "ajastin.hpp"

using namespace std;

Ajastin::Ajastin(string_view nimi)
: aloitusAika(Kello::now())
, nimi(nimi) { }

Ajastin::~Ajastin()
{
    auto lopetusAika = Kello::now();
    chrono::duration<double, milli> kesto = lopetusAika - aloitusAika;
    cout << nimi << " : " << kesto.count() << " ms" << endl;
}
