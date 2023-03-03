#include "Uci.h"
#include "uci.h"
using namespace std;


void Uci::positionStartpos()
{
    _asema = Asema();
}

void Uci::uciLoop()
{
    Kayttoliittyma::getInstance().setOnkoUCI(true);
    string line;
    cout << "Waiting for UCI-protocol, press Enter to play manually in CMD." << endl;


    while (getline(cin, line))
    {

        // UCI-protokollan aloitus.
        if (line == "uci")
        {
            cout << "id name M8" << endl;
            cout << "id author Ryhma1" << endl;
            cout << "uciok" << endl;
        }
        else if (line == "quit")
        {
            cout << "Quitting UCI" << endl;
            break;
        }
        else if (line == "isready")
        {
            cout << "readyok" << endl;
        }
        else if (line == "ucinewgame")
        {
            // Tyhjä toistaiseksi.
            // Aloittaa uuden pelin.

        }
        else if (line == "")
        {
            break;
        }



        // UCI-protokollan liikekomennot.
        if (line.find("position startpos") != string::npos)
        {
                Uci::positionStartpos();
        }

        if (line.find("moves") != string::npos)
        {
            string syote = line.substr(line.find_last_of(" ") + 1);

            Ruutu alku(-1, -1);
            Ruutu loppu(-1, -1);

            alku.setSarake(syote[0] - 'a');
            alku.setRivi(syote[1] - '0' - 1);

            loppu.setSarake(syote[2] - 'a');
            loppu.setRivi(syote[3] - '0' - 1);

            Siirto siirto(alku, loppu);

            _asema.paivitaAsema(siirto);

        }
        else if (line.substr(0, 3) == "go ")
        {
            //Saa komennon kuten: "go wtime 300000 btime 300000 winc 0 binc 0".
            bool ok = false;
            Siirto siirto;
            MinMaxPaluu minimax;

            minimax = _asema.alphabetaMinimaxAsync(4);

            siirto = minimax.parasSiirto;

            _asema.paivitaAsema(siirto);

            // Syöttää GUIlle komennon muodossa: "bestmove <siirto>".
            cout << "bestmove " << siirto << endl;
        }
    }
}