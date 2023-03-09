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
        string fen;
        
        stringstream syote(rivi);
        string token;

        while(getline(syote, token, ' '))
        {
            if (lukeeSiirtoja)
            {
                optional<Siirto> siirto = Siirto::lue(token, _asema.getSiirtovuoro());
                
                if (siirto.has_value())
                {
                    _asema.paivitaAsema(siirto.value());
                }
            }
            else if (token == "moves")
            {
                if (!fen.empty())
                {
                    optional<Peli> peli = Peli::lue(fen);
                    if (peli.has_value())
                    {
                        _asema = peli.value().asema;
                    }
                }
                
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
                    fen += token;
                }
            }
            else if (token == "position")
            {
                lukeeAsemaa = true;
            }
            else if (token == "go")
            {
                //Saa komennon kuten: "go wtime 300000 btime 300000 winc 0 binc 0".
                MinMaxPaluu minimax = _asema.alphabetaMinimaxAsync(6);
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
