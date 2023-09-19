#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <iostream>
#include <string>
#include <vector>

#include "objekty.hpp"

//#define dprintfv(...) printf(__VA_ARGS__)
#define dprintfv(...)

int POCET_LET = 10;
int BATERIE =   30; //kapacita bateriií = instalovaný výkon soláru * BATERIE
int PANELY =    30; //(kolikrát - 1) ze zvětší instalovyný výkon soláru o základní hodnotu
bool RECYKLACE = true;

using namespace std;

double celkova_kapacita_baterii;

static void similuj();
static struct vysledky vysledek(int d);
void print_vysledek(struct vysledky v);
static void spawner_bat(double max);
static void spawner_neobz();
static void spawner_obz();

struct vysledky {
	int index;
	int index2;
	int pocet_let;
	double obz_solar;
	double neobz_voda;
	double neobz_jadro;
	double neobz_para;
	double celkova_spotreba;
	double vyrobeno_celkem;
	double bateire_celkove;
	double podil_voda;
	double podil_jadro;
	double podil_para;
	double podil_solar;
	double install_jadro;
	double install_voda;
	double install_para;
	double install_solar;
	double install_celkem;
	double celkova_kapacita_baterii;
	double pocet_baterii;
	double pocet_panelu;
	double pocet_panelu_na_baterii;
	double emise_para;
	double emise_baterie;
	double emise_panel;
	double emise_celkem;
};

vector<struct vysledky> studie;

int main (int argc, char **argv){
	std::cout.sync_with_stdio(true);                                             
	std::cerr.sync_with_stdio(true);  

	
	if(argc != 1 && argc != 5){
		printf("program bud neprijma zadne parametry (jsou nastaveny na vychozi hodnoty)\n");
		printf("a nebo lze zadat presne 4 parametry v presnem poradi\n");
		printf("takto:\n");
		printf("\t./emise POCET_LET PANELY BATERIE RECYKLACE\n");
		printf("nebo takto:\n");
		printf("\tmake run ARGS=\"POCET_LET PANELY BATERIE RECYKLACE\"\n");
		printf("\nPOCET_LET - integer kolik let pobezi simulace\n");
		printf("PANELY - integer maximalni nasobek soucasneho instalovaneho vykonu solarnich panelu v cr\n");
		printf("BATERIE - integer urcuje maximalni kapacitu baterii jako nasobek instalovaneho vykonu solarnich panelu v aktualni simulaci\n");
		printf("RECYKLACE - '0' neob '1' zda se mají pouzit hodnoty emisi pro recyklovane panely a baterie nebo ne\n");
		exit(0);
	}

	if(argc == 5){
		char *endptr;
		POCET_LET = strtol(argv[1], &endptr, 10);
		PANELY = strtol(argv[2], &endptr, 10);
		BATERIE = strtol(argv[3], &endptr, 10);
		RECYKLACE = !!strtol(argv[4], &endptr, 10);
	}

	instalovany_podil.jadro = instalovyny_vykon * podil_vykonu.jadro;
	instalovany_podil.para = instalovyny_vykon * podil_vykonu.para;
	instalovany_podil.voda = instalovyny_vykon * podil_vykonu.voda;
	instalovany_podil.solar = instalovyny_vykon * podil_vykonu.solar;
	
	double vychozi_vykon_solar = instalovany_podil.solar;

	for(int i = 0; i < PANELY; ++i){
		//zvyseni vykonu solaru
		double nasobic = !!i;

		instalovany_podil.solar += vychozi_vykon_solar * nasobic;	
		instalovyny_vykon += vychozi_vykon_solar * nasobic;

		podil_vykonu.jadro = instalovany_podil.jadro / instalovyny_vykon;
		podil_vykonu.voda = instalovany_podil.voda / instalovyny_vykon;
		podil_vykonu.para = instalovany_podil.para / instalovyny_vykon;
		podil_vykonu.solar = instalovany_podil.solar / instalovyny_vykon;
		/////////

	for(int j = 0; j < BATERIE; ++j){
		baterie.clear();
		celkova_kapacita_baterii = instalovany_podil.solar * j;
		spawner_bat(celkova_kapacita_baterii);
		spawner_neobz();
		spawner_obz();

		similuj();

		struct vysledky tmp_vysledek = vysledek(i);

		dprintfv("celkova kapacita baterii %lf GWh\n", celkova_kapacita_baterii / 1000000);
		dprintfv("pocet baterii %lf\n", celkova_kapacita_baterii / 13.5);
		dprintfv("pocet panelu %lf, pocet panelu na baterii %lf\n", instalovany_podil.solar / 0.3, (instalovany_podil.solar / 0.3) / (celkova_kapacita_baterii / 13.5));
		tmp_vysledek.celkova_kapacita_baterii = celkova_kapacita_baterii;
		tmp_vysledek.pocet_baterii = celkova_kapacita_baterii / 13.5;
		tmp_vysledek.pocet_panelu = instalovany_podil.solar / 0.3;
		tmp_vysledek.pocet_panelu_na_baterii = (instalovany_podil.solar / 0.3) / (celkova_kapacita_baterii / 13.5);

		///////////// EMISE //////////////////
		if(RECYKLACE == false){
			tmp_vysledek.emise_panel = tmp_vysledek.pocet_panelu * 23.4 + tmp_vysledek.obz_solar * 0.024;
			tmp_vysledek.emise_baterie = tmp_vysledek.pocet_baterii * 1012.5;
			tmp_vysledek.emise_para = tmp_vysledek.neobz_para * 1.06;
		} else {
			tmp_vysledek.emise_panel = tmp_vysledek.pocet_panelu * 7 + tmp_vysledek.obz_solar * 0.024;
			tmp_vysledek.emise_baterie = tmp_vysledek.pocet_baterii * 364.5;
			tmp_vysledek.emise_para = tmp_vysledek.neobz_para * 1.06;
		}
		//////////////////////////////////////
		
		tmp_vysledek.emise_celkem = tmp_vysledek.emise_panel + tmp_vysledek.emise_baterie + tmp_vysledek.emise_para;
		tmp_vysledek.index2 = j;
		tmp_vysledek.pocet_let = rok();

		studie.push_back(tmp_vysledek);
		print_vysledek(tmp_vysledek);
		doba_reset();
		celkova_spotreba = 0;
		baterie_celkove_energie = 0;
	}
	}

	double minimalni_emise = INFINITY;
	int mininimal_index = 0;
	for(int i = 0; i < studie.size(); ++i){
		if(studie[i].emise_celkem < minimalni_emise){
			minimalni_emise = studie[i].emise_celkem;
			mininimal_index = i;
		}
	}
	printf("\n\n/////////////////////////////////////////////////////////////////\n");
	printf("/////////////////////// Nejlepsi vysledek ///////////////////////");
	print_vysledek(studie[mininimal_index]);

	printf("\nVstup simulace POCET_LET %d, PANELY %d, BATERIE %d, RECYKLACE %d\n", POCET_LET, PANELY, BATERIE, RECYKLACE);
	return 0;
}



static void similuj(){


	while(rok() < POCET_LET){
		dprintf("\nh %d, d %d, t %d, r %d, z %d\n", hodina(), den(), tyden(), rok(), zima());
		double spotreba = spotreba_akt();
		celkova_spotreba += spotreba;
		dprintf("spotreba %lf MWh\n", spotreba / 1000);
		spotreba = neobz_dodej(spotreba);
		dprintf("spotreba po neobz %lf MWh\n", spotreba / 1000);
		double dodano = obz_dodej(-1);
		dprintf("dodano z obz %lf MWh\n", dodano / 1000);
		spotreba -= dodano;
		if(spotreba < 0){
			spotreba *= -1;
			dprintf("bude dobito %lf MWh\n", spotreba / 1000);
			bat_nabij(spotreba);
			spotreba = 0;
		} else {
			dprintf("potreba dodat %lf MWh\n", spotreba / 1000);
			double kapacita = bat_kap();
			double potreba_z_bat = (spotreba < kapacita) ? (spotreba) : (kapacita);
			dprintf("kapacita %lf MWh, potreba %lf MWh\n", kapacita / 1000, potreba_z_bat / 1000);
			double bat_dodaly = bat_vybij(potreba_z_bat);
			dprintf("bat dodaly %lf MWh\n", bat_dodaly / 1000);
			spotreba -= bat_dodaly;
			if(spotreba > 0){
				dprintf("potreba dodat z neobz %lf MWh\n", spotreba / 1000);
				neobz_dodej_uhli(spotreba);
			}
		}
		hodina_pridej(1);
	}
}

void print_vysledek(struct vysledky v){
	printf("\n\n/// VYSLEDEK %d %d ///\n", v.index, v.index2);
	printf("pocet let %d\n", v.pocet_let);
	printf("obz vyrobilo      \t%lf GWh / rok\n", v.obz_solar / 1000000 / v.pocet_let);
	printf("neobz %s vyrobilo \t%lf GWh / rok\n", "voda", v.neobz_voda / 1000000 / v.pocet_let);
	printf("neobz %s vyrobilo \t%lf GWh / rok\n", "jadro", v.neobz_jadro / 1000000 / v.pocet_let);
	printf("neobz %s vyrobilo \t%lf GWh / rok\n", "para", v.neobz_para / 1000000 / v.pocet_let);
	printf("celkova spotreba %lf GWh / rok\n", v.celkova_spotreba / 1000000 / v.pocet_let);
	printf("bateriemi proslo  \t%lf Gwh / rok\n", v.bateire_celkove / 1000000 / v.pocet_let);
	printf("instal vykon jadro podil %lf abs %lf GW\n", v.podil_jadro, v.install_jadro / 1000000);
	printf("instal vykon voda  podil %lf abs %lf GW\n", v.podil_voda,v.install_voda / 1000000);
	printf("instal vykon para  podil %lf abs %lf GW\n", v.podil_para, v.install_para / 1000000);
	printf("instal vykon solar podil %lf abs %lf GW\n", v.podil_solar, v.install_solar / 1000000);
	printf("instal vykon celkem %lf GW\n", v.install_celkem / 1000000);
		
	printf("celkova kapacita baterii %lf GWh\n", v.celkova_kapacita_baterii / 1000000);
	printf("pocet baterii %lf\n", v.pocet_baterii);
	printf("pocet panelu %lf, pocet panelu na baterii %lf\n", v.pocet_panelu, v.pocet_panelu_na_baterii);

	printf("emise para  %lf tun CO2e / rok\n", v.emise_para / 1000 / v.pocet_let);
	printf("emise pa+bt %lf tun CO2e / rok\n", (v.emise_panel + v.emise_baterie) / 1000 / v.pocet_let);
	printf("emise panel %lf tun CO2e / rok\n", v.emise_panel / 1000 / v.pocet_let);
	printf("emise bat   %lf tun CO2e / rok\n", v.emise_baterie / 1000 / v.pocet_let);
	printf("emise celk  %lf tun CO2e / rok\n", v.emise_celkem / 1000 / v.pocet_let);
}

static struct vysledky vysledek(int d){
	dprintfv("\n\n/// VYSLEDEK %d ///\n", d);
	struct vysledky ret = {};
	ret.index = d;
	double vyrobeno_celkem = 0;
	double obz_vyrobilo = 0;
	for(auto a : obz)
		obz_vyrobilo += a.vyrobeno;
	dprintfv("obz vyrobilo      \t %lf GWh\n", obz_vyrobilo / 1000000);
	ret.obz_solar = obz_vyrobilo;
	vyrobeno_celkem = obz_vyrobilo;
	for(auto a : neobz){
		double neobz_vyrobilo = a.vyrobeno;
		vyrobeno_celkem += neobz_vyrobilo;
		dprintfv("neobz %s vyrobilo \t%lf GWh\n", typy_zdroju[a.typ].c_str(), neobz_vyrobilo / 1000000);
	}
	ret.neobz_voda = neobz[0].vyrobeno;
	ret.neobz_jadro = neobz[1].vyrobeno;
	ret.neobz_para = neobz[2].vyrobeno;
	double spotrebovano_celkem = 0;
	dprintfv("celkova spotreba %lf GWh\n", celkova_spotreba / 1000000);
	ret.celkova_spotreba = celkova_spotreba;
	dprintfv("vyrobeno celkem  %lf Gwh\n", vyrobeno_celkem / 1000000);
	ret.vyrobeno_celkem = vyrobeno_celkem;
	
	dprintfv("baterie celkove        %lf Gwh\n", baterie_celkove_energie / 1000000);
	dprintfv("instal vykon jadro %lf %lf GW\n", podil_vykonu.jadro, instalovany_podil.jadro / 1000000);
	dprintfv("instal vykon voda  %lf %lf GW\n", podil_vykonu.voda,instalovany_podil.voda / 1000000);
	dprintfv("instal vykon para  %lf %lf GW\n", podil_vykonu.para,instalovany_podil.para / 1000000);
	dprintfv("instal vykon solar %lf %lf GW\n", podil_vykonu.solar,instalovany_podil.solar / 1000000);
	dprintfv("instal vykon celkem %lf GW\n", instalovyny_vykon / 1000000);

	ret.bateire_celkove = baterie_celkove_energie;

	ret.podil_jadro = podil_vykonu.jadro;
	ret.podil_voda = podil_vykonu.voda;
	ret.podil_para = podil_vykonu.para;
	ret.podil_solar = podil_vykonu.solar;

	ret.install_jadro = instalovany_podil.jadro;
	ret.install_voda = instalovany_podil.voda;
	ret.install_para = instalovany_podil.para;
	ret.install_solar = instalovany_podil.solar;
	ret.install_celkem = instalovyny_vykon;

	return ret;
}


static void spawner_bat(double max){
	dprintf("generuji baterii o kapacite %lf MWh\n", max / 1000);
	struct bat tmp = {};
	tmp.kapacita = max * 0.9;
	tmp.max_kapacita = max;
	tmp.horni_limit = 0.8;
	tmp.spodni_limit = 0.2;
	tmp.zimni_limit = 0.4;

	baterie.push_back(tmp);
}

static void spawner_neobz(){
	neobz.clear();
	struct zdroj voda = {
		VODA,
		instalovany_podil.voda,
		true,
		0
	};
	neobz.push_back(voda);

	struct zdroj jadro = {
		JADRO,
		instalovany_podil.jadro,
		false,
		0
	};
	neobz.push_back(jadro);
	
	struct zdroj para = {
		PARA,
		instalovany_podil.para,
		false,
		0
	};
	neobz.push_back(para);
}

static void spawner_obz(){
	obz.clear();
	struct zdroj solar = {
		SOLAR,
		instalovany_podil.solar,
		true,
		0
	};
	obz.push_back(solar);
}


