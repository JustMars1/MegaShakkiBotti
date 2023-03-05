#include "uci.h"

#include <sstream>

using namespace std;

void UCI::uciLoop()
{
    Kayttoliittyma::getInstance().setOnkoUCI(true);
    cout << "Waiting for UCI-protocol, press Enter to play manually in CMD." << endl;
    
    bool lopeta = false;
    
    string rivi;
    while (getline(cin, rivi))
    {
        if (rivi == "" || lopeta)
        {
            break;
        }
        
        bool lukeeSiirtoja = false;
        bool lukeeAsemaa = false;
        
        stringstream syote(rivi);
        string token;

        while(getline(syote, token, ' '))
        {
            if (lukeeSiirtoja)
            {
                Ruutu alku(-1, -1);
                Ruutu loppu(-1, -1);
                
                alku.setSarake(token[0] - 'a');
                alku.setRivi(token[1] - '0' - 1);
                
                loppu.setSarake(token[2] - 'a');
                loppu.setRivi(token[3] - '0' - 1);
                
                Siirto siirto(alku, loppu);
                _asema.paivitaAsema(siirto);
            }
            else if (token == "moves")
            {
                lukeeSiirtoja = true;
            }
            else if (lukeeAsemaa)
            {
                if (token == "startpos")
                {
                    _asema = Asema();
                }
                else
                {
                    // FEN
                }
            }
            else if (token == "position")
            {
                lukeeAsemaa = true;
            }
            else if (token == "go")
            {
                //Saa komennon kuten: "go wtime 300000 btime 300000 winc 0 binc 0".
                MinMaxPaluu minimax = _asema.alphabetaMinimaxAsync(4);
                Siirto siirto = minimax.parasSiirto;
                
                _asema.paivitaAsema(siirto);
                
                // Syöttää GUIlle komennon muodossa: "bestmove <siirto>".
                cout << "bestmove " << siirto << endl;
            }
            else if (token == "uci")
            {
                // UCI-protokollan aloitus.
                cout << "id name M8" << endl;
                cout << "id author Ryhma1" << endl;
                cout << "uciok" << endl;
            }
            else if (token == "quit")
            {
                cout << "Quitting UCI" << endl;
                lopeta = true;
                break;
            }
            else if (token == "isready")
            {
                cout << "readyok" << endl;
            }
            else if (token == "ucinewgame")
            {
                // Tyhjä toistaiseksi.
                // Aloittaa uuden pelin.
            }
        }
    }
}
