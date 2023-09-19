#include "objekty.hpp"
/////////////////////////////////////////////////////////////////////////////////
//protože nezohlednujeme rychlost nabijeni baterii, tak je jedno ze se nebudou nabijet rovnomerne

std::vector<struct bat> baterie;
double baterie_celkove_energie;

double bat_kap(){
	double ret = 0;
	for(struct bat a : baterie){
		//dprintf("bat kapacita %lf\n", a.kapacita);
		ret += a.kapacita;
	}
	return ret;
}

double bat_vybij(double e){
	int pocet = baterie.size();
	double ziskana_energie = 0;

	//dprintf("vybij %lf\n", e);
	for(int i = 0; i < pocet && ziskana_energie < e; ++i){
		double potreba = e - ziskana_energie;
		double davka = 0;
		davka = baterie[i].kapacita - baterie[i].max_kapacita * (zima() ? baterie[i].zimni_limit : baterie[i].spodni_limit);
		//dprintf("bat %d, ma %lf, da %lf, potreba %lf, ziskano %lf\n", i, baterie[i].kapacita, davka, potreba, ziskana_energie);
		if(davka < potreba){
			ziskana_energie += davka;
			baterie[i].kapacita = baterie[i].max_kapacita * (zima() ? baterie[i].zimni_limit : baterie[i].spodni_limit);
		} else {
			baterie[i].kapacita -= potreba;
			ziskana_energie += potreba;
			break;
		}

	}
	baterie_celkove_energie += ziskana_energie;	
	return ziskana_energie;
}

double bat_nabij(double e){
	int pocet = baterie.size();
	//dprintf("nabij %lf\n", e);

	for(int i = 0; i < pocet && e; ++i){
		baterie[i].kapacita += e * 0.9;
		e = 0;
		if(baterie[i].kapacita > baterie[i].max_kapacita * baterie[i].horni_limit){
			double nevlezlo_se = 0;
			nevlezlo_se = baterie[i].kapacita - baterie[i].max_kapacita * baterie[i].horni_limit;
			baterie[i].kapacita = baterie[i].max_kapacita * baterie[i].horni_limit;
			e = nevlezlo_se / 0.9;
		}
	}
	//dprintf("nevlezlo se %lf\n", e);
	return e;
}

