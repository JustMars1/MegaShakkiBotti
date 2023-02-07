#include "evaluointi.h"
#include "ruutu.h"
/* 1. Laske nappuloiden arvo
		Daami = 9
		Torni = 5
		Lähetti = 3,25
		Ratsu = 3
		Sotilas = 1
		
	2. Kuninkaan hyvyys
		Jos avaus tai keskipeli, niin hyvä että kunigas g1 tai b1/c1
		Loppupelissä vaikea sanoa halutaanko olla auttamassa omaa sotilasta korottumaan
		vai olla estämässä vastustajan korotusta siksi ei oteta kantaa
	3. Arvosta keskustaa sotilailla ja ratsuilla
	4. Arvosta pitkiä linjoja daami, torni ja lähetti
*/
double Evaluointi::evaluoi(Asema* asema, LaillisetSiirrot* lailliset) {
	double valkeaArvo = 0;
	double mustaArvo = 0;
	
	//kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta
	double kuningasKerroin = 1;
	double keskustaKerroin = 1;
	double linjaKerroin = 0.05;
	//1. Nappuloiden arvo
	valkeaArvo += laskeNappuloidenArvo(asema, 0);
	mustaArvo += laskeNappuloidenArvo(asema, 1);
	//2. Kuningas turvassa
	//if (onkoAvausTaiKeskipeli(asema, 0)) {
	//	// Jos lyhellä puolella saa lisäarvoa 2 edellyttää että f ja g sotilas  paikallaan
	//	if (asema->lauta[6][0] != NULL && asema->lauta[5][1] != NULL && asema->lauta[6][1] != NULL) {
	//		if (asema->lauta[6][0]->getNimi() == "vk" && (asema->lauta[5][1]->getNimi() == "vs" && (asema->lauta[6][1]->getNimi() == "vs")))
	//			valkeaArvo += 2 * kuningasKerroin;
	//	}
	//	// Jos pitkällä puolella saa lisäarvooa 1 edelyttää että  c ja b sotilas paikallaan
	//	if (asema->lauta[1][0] != NULL && asema->lauta[2][0] != NULL && asema->lauta[1][1] != NULL && asema->lauta[2][1] != NULL) {
	//		if (asema->lauta[1][0]->getNimi() == "vk" || asema->lauta[2][0]->getNimi() == "vk" && (asema->lauta[1][1]->getNimi() == "vs" && (asema->lauta[2][1]->getNimi() == "vs")))
	//			valkeaArvo += 1 * kuningasKerroin;
	//	}
	//}
	//if (onkoAvausTaiKeskipeli(asema, 1)) {
	//	// Jos lyhellä puolella saa lisäarvoa 2 edellyttää että f ja g sotilas  paikallaan
	//	if (asema->lauta[6][7] != NULL && asema->lauta[5][6] != NULL && asema->lauta[6][6] != NULL) {
	//		if (asema->lauta[6][7]->getNimi() == "mk" && (asema->lauta[5][6]->getNimi() == "ms" && (asema->lauta[6][6]->getNimi() == "ms")))
	//			valkeaArvo += 2 * kuningasKerroin;
	//	}
	//	// Jos pitkällä puolella saa lisäarvooa 1 edelyttää että  c ja b sotilas paikallaan
	//	if (asema->lauta[1][7] != NULL && asema->lauta[2][7] != NULL && asema->lauta[1][7] != NULL && asema->lauta[2][7] != NULL) {
	//		if (asema->lauta[1][7]->getNimi() == "mk" || asema->lauta[2][7]->getNimi() == "mk" && (asema->lauta[1][7]->getNimi() == "ms" && (asema->lauta[2][7]->getNimi() == "ms")))
	//			valkeaArvo += 1 * kuningasKerroin;
	//	}
	//}
	//3. Arvosta keskustaa
	valkeaArvo = nappuloitaKeskella(asema, 0) * keskustaKerroin;
	mustaArvo = nappuloitaKeskella(asema, 1) * keskustaKerroin;
	// 4. Arvosta linjoja
	valkeaArvo = linjaKerroin * linjat(asema, 0, lailliset);
	mustaArvo = linjaKerroin *linjat(asema, 1, lailliset);
	
	return valkeaArvo - mustaArvo;
}
double Evaluointi::laskeNappuloidenArvo(Asema* asema, int vari) {
	double valkeaArvo = 0;
	double mustaArvo = 0;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (asema->lauta[x][y] != NULL) {
				std::string nappulanNimi = asema->lauta[x][y]->getNimi();
				//Valkoiset
				if (nappulanNimi == "vd")
					valkeaArvo += 9;
				if (nappulanNimi == "vt")
					valkeaArvo += 5;
				if (nappulanNimi == "vl")
					valkeaArvo += 3.25;
				if (nappulanNimi == "vr")
					valkeaArvo += 3;
				if (nappulanNimi == "vs")
					valkeaArvo += 1;
				//Mustat
				if (nappulanNimi == "md")
					mustaArvo += 9;
				if (nappulanNimi == "mt")
					mustaArvo += 5;
				if (nappulanNimi == "ml")
					mustaArvo += 3.25;
				if (nappulanNimi == "mr")
					mustaArvo += 3;
				if (nappulanNimi == "ms")
					mustaArvo += 1;
			}
		}
	}
	if (vari == 0)
		return valkeaArvo;
	else
		return mustaArvo;
}
bool Evaluointi::onkoAvausTaiKeskipeli(Asema* asema, int vari) {
	// Jos upseereita 3 tai vähemmän on loppupeli
	// mutta jos daami laudalla on loppueli vasta kun kuin vain daami jäljellä
	int valkeaUpseeriLkm = 0;
	int mustaUpseeriLkm = 0;
	bool valkeaDaami = false;
	bool mustaDaami = false;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (asema->lauta[x][y] == NULL) {
				continue;
			}
			std::string nappulanNimi = asema->lauta[x][y]->getNimi();
			//Valkoiset
			if (nappulanNimi == "vd") {
				valkeaUpseeriLkm += 1;
				valkeaDaami = true;
			}
			if (nappulanNimi == "vt")
				valkeaUpseeriLkm += 1;
			if (nappulanNimi == "vl")
				valkeaUpseeriLkm += 1;
			if (nappulanNimi == "vr")
				valkeaUpseeriLkm += 1;

			//Mustat
			if (nappulanNimi == "md") {
				valkeaUpseeriLkm += 1;
				mustaDaami = true;
			}
			if (nappulanNimi == "mt")
				valkeaUpseeriLkm += 1;
			if (nappulanNimi == "ml")
				valkeaUpseeriLkm += 1;
			if (nappulanNimi == "mr")
				valkeaUpseeriLkm += 1;
		}
	}
	//Jos vari on 0 eli valkoiset
	//niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1
	if (vari == 0) {
		if (mustaUpseeriLkm > 2 || (mustaDaami == true && mustaUpseeriLkm > 1))
			return true;
		else
			return false;
	}
	else {
		if (valkeaUpseeriLkm > 2 || (valkeaDaami == true && valkeaUpseeriLkm > 1))
			return true;
		else
			return false;
	}

}
double Evaluointi::nappuloitaKeskella(Asema* asema, int vari) {

	//sotilaat ydinkeskustassa + 0.25/napa
	//ratsut ydinkeskustassa + 0.25/napa
	//sotilaat laitakeskustassa + 0.11/napa
	//ratsut laitakeskustassa + 0.11/napa
	double valkeaKeskella = 0;
	double mustaKeskella = 0;
	//valkeille ydinkeskusta
	
	if (asema->lauta[3][3] != NULL && (asema->lauta[3][3]->getNimi() == "vs" || asema->lauta[3][3]->getNimi() == "vr")) {
		valkeaKeskella +=  0.25;
	}
	if (asema->lauta[4][3] && (asema->lauta[4][3]->getNimi() == "vs" || asema->lauta[4][3]->getNimi() == "vr")) {
		valkeaKeskella += 0.25;
	}
	if (asema->lauta[3][4] != NULL && (asema->lauta[3][4]->getNimi() == "vs" || asema->lauta[3][4]->getNimi() == "vr")) {
		valkeaKeskella += 0.25;
	}
	if (asema->lauta[4][4] != NULL && (asema->lauta[4][4]->getNimi() == "vs" || asema->lauta[4][4]->getNimi() == "vr")) {
		valkeaKeskella += 0.25;
	}
	//valkeille laitakeskusta
	for (int x = 2; x < 6; x++) {
		if (asema->lauta[x][2] != NULL && (asema->lauta[x][2]->getNimi() == "vs" || asema->lauta[x][2]->getNimi() == "vr")) {
			valkeaKeskella += 0.11;
		}
		if (asema->lauta[x][5] != NULL && (asema->lauta[x][5]->getNimi() == "vs" || asema->lauta[x][5]->getNimi() == "vr")) {
			valkeaKeskella += 0.11;
		}
	}
	for (int y = 3; y < 5; y++) {
		if (asema->lauta[2][y] != NULL && (asema->lauta[2][y]->getNimi() == "vs" || asema->lauta[2][y]->getNimi() == "vr")) {
			valkeaKeskella += 0.11;
		}
		if (asema->lauta[5][y] != NULL && (asema->lauta[5][y]->getNimi() == "vs" || asema->lauta[5][y]->getNimi() == "vr")) {
			valkeaKeskella += 0.11;
		}
	}

	//mustille ydinkeskusta
	if (asema->lauta[3][3] != NULL && (asema->lauta[3][3]->getNimi() == "ms" || asema->lauta[3][3]->getNimi() == "mr")) {
		mustaKeskella += 0.25;
	}
	if (asema->lauta[4][3] != NULL && (asema->lauta[4][3]->getNimi() == "ms" || asema->lauta[4][3]->getNimi() == "mr")) {
		mustaKeskella += 0.25;
	}
	if (asema->lauta[3][4] != NULL && (asema->lauta[3][4]->getNimi() == "ms" || asema->lauta[3][4]->getNimi() == "mr")) {
		mustaKeskella += 0.25;
	}
	if (asema->lauta[4][4] != NULL && (asema->lauta[4][4]->getNimi() == "ms" || asema->lauta[4][4]->getNimi() == "mr")) {
		mustaKeskella += 0.25;
	}
	//mustille laitakeskusta
	for (int x = 2; x < 6; x++) {
		if (asema->lauta[x][2] != NULL && (asema->lauta[x][2]->getNimi() == "ms" || asema->lauta[x][2]->getNimi() == "mr")) {
			mustaKeskella += 0.11;
		}
		if (asema->lauta[x][5] != NULL && (asema->lauta[x][5]->getNimi() == "ms" || asema->lauta[x][5]->getNimi() == "mr")) {
			mustaKeskella += 0.11;
		}
	}
	for (int y = 3; y < 5; y++) {
		if (asema->lauta[2][y] != NULL && (asema->lauta[2][y]->getNimi() == "ms" || asema->lauta[2][y]->getNimi() == "mr")) {
			mustaKeskella += 0.11;
		}
		if (asema->lauta[5][y] != NULL && (asema->lauta[5][y]->getNimi() == "ms" || asema->lauta[5][y]->getNimi() == "mr")) {
			mustaKeskella += 0.11;
		}
	}
	if (vari == 0)
		return valkeaKeskella;
	else
		return mustaKeskella;
}
double Evaluointi::linjat(Asema* asema, int vari, LaillisetSiirrot* lailliset) {
	int valkeaLaillisiaSiirtoja = 0;
	int mustaLaillisiaSiirtoja = 0;
	std::list<Siirto> valkealista;
	std::list<Siirto> mustalista;
	//valkoiset
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (asema->lauta[x][y] == NULL) {
				continue;
			}
			if(asema->lauta[x][y]->getNimi() == "vl")
				lailliset->annaLahetinLaillisetSiirrot(valkealista, &Ruutu(x,y),asema, 0);
			if (asema->lauta[x][y]->getNimi() == "vt")
				lailliset->annaTorninLaillisetSiirrot(valkealista, &Ruutu(x, y), asema, 0);
			if (asema->lauta[x][y]->getNimi() == "vd")
				lailliset->annaDaaminLaillisetSiirrot(valkealista, &Ruutu(x, y), asema, 0);
		}
	}
	//mustat
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (asema->lauta[x][y] == NULL) {
				continue;
			}
			if (asema->lauta[x][y]->getNimi() == "ml")
				lailliset->annaLahetinLaillisetSiirrot(mustalista, &Ruutu(x, y), asema, 0);
			if (asema->lauta[x][y]->getNimi() == "mt")
				lailliset->annaTorninLaillisetSiirrot(mustalista, &Ruutu(x, y), asema, 0);
			if (asema->lauta[x][y]->getNimi() == "md")
				lailliset->annaDaaminLaillisetSiirrot(mustalista, &Ruutu(x, y), asema, 0);
		}
	}
	valkeaLaillisiaSiirtoja = valkealista.size();
	mustaLaillisiaSiirtoja = mustalista.size();

	if (vari == 0)
		return valkeaLaillisiaSiirtoja;
	else
		return mustaLaillisiaSiirtoja;
}
	

