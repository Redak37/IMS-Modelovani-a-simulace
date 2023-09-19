#include "objekty.hpp"

/////////////////////////////////////////////////////////////////////////////////

std::vector<struct zdroj> neobz;
std::vector<struct zdroj> obz;

double instalovyny_vykon = 21989000;
struct podil_vykonu podil_vykonu = {
	0.20 * 100 / 94, 
	0.59 * 100 / 94,
	0.05 * 100 / 94,
	0.10 * 100 / 94
};
struct podil_vykonu instalovany_podil;

std::string typy_zdroju[4] = {
	"jadro",
	"para",
	"voda",
	"solar"
};

static double efektivita(){
	double ret = 1;

	double h = hodina() / 10.0;

	//hodina
	ret *= -1.5625 * h * h + 4.0625 * h - 1.640425;
	//tyden
	double modifikator = -0.75 * (26 - tyden_gauss()) / 27;
	ret += modifikator;
	if (ret < 0)
		ret = 0;
	//dprintf("efektivita %lf\n", ret);
	return ret;
}



double obz_kap(){
	double ret = 0;
	//for(struct zdroj a : obz){
	//	ret += a.vykon;
	//}
	for(unsigned long i = 0; i < obz.size(); ++i){
		ret += obz[i].vykon;
	}

	ret *= efektivita();
	
	return ret;
}

double obz_dodej(double e){
	int pocet = obz.size();
	double e_tmp = 0;
	e_tmp = obz_kap();
	if(e_tmp < e){
		e = e_tmp;
		fprintf(stderr, "Chyba: obnovitelne zdroje nestaci\n");
	}
	if(e == -1){
		e = e_tmp;
	}
	for(int i = 0; i < pocet; ++i){
		obz[i].vyrobeno += e/pocet;
	}
	return e;
}

double neobz_kap(){
	double ret = 0;
	for(struct zdroj a : obz){
		ret += a.vykon;
	}
	return ret;
}

//vrací kolik energie ne nevyrobilo
double neobz_dodej(double e){
	int pocet = neobz.size() - 1;
	for(int i = 0; i < pocet; ++i){
		double davka = neobz[i].vykon;
		e -= davka;
		neobz[i].vyrobeno += davka;

		if(e < 0){
			neobz[i].vyrobeno += e;
			e = 0;
		}
	}
	return e;
}

double neobz_dodej_uhli(double e){
	int pocet = neobz.size();
	int i = 2;
		double davka = neobz[i].vykon;
		e -= davka;
		neobz[i].vyrobeno += davka;

		if(e < 0){
			neobz[i].vyrobeno += e;
			e = 0;
		}
	return e;
}
