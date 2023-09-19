#ifndef __OBJEKTY_H__
#define __OBJEKTY_H__

#include <stdint.h>
#include <vector>
#include <string>

//#define dprintf(...) printf(__VA_ARGS__)
#define dprintf(...)

/////////////////////////////// ČAS /////////////////////////////////////////////

bool zima();
int rok();
int tyden(); //týden 0 je 22 prosinec
int tyden_gauss();
int den();
int hodina();
void hodina_pridej(int h);
void doba_reset();

/////////////////////////////// SPOTŘEBA ////////////////////////////////////////

extern double celkova_spotreba;
extern double spotreba_cr[24][53];

double spotreba_akt();

/////////////////////////////// ZDROJE //////////////////////////////////////////

enum typ_zdroje {
	JADRO,
	PARA,
	VODA,
	SOLAR
};

extern std::string typy_zdroju[4];

struct zdroj {
	enum typ_zdroje typ;
	//int priorita;
	double vykon;
	bool obnovitelny;
	double vyrobeno;
};

struct podil_vykonu {
	double jadro;
	double para;
	double voda;
	double solar;
};

extern double instalovyny_vykon;
extern struct podil_vykonu podil_vykonu;
extern struct podil_vykonu instalovany_podil;

extern std::vector<struct zdroj> neobz;
extern std::vector<struct zdroj> obz;

double obz_kap();
double obz_dodej(double e); //vrací kolik energie se reálně vyrobilo
double neobz_kap();
double neobz_dodej(double e);
double neobz_dodej_uhli(double e);

/////////////////////////////// BATERIE /////////////////////////////////////////

enum typ_baterie {
	ZADNA,
	LI_ION,
	LIFEPO
};


struct bat {
	enum typ_baterie typ;
	//int priorita;
	double kapacita;
	double max_kapacita;
	double horni_limit;
	double spodni_limit;
	double zimni_limit;
};

extern std::vector<struct bat> baterie;
extern double baterie_celkove_energie;

double bat_kap();
double bat_vybij(double e); //vrací kolik energie se vyčerpalo
double bat_nabij(double e);	//vrací kolik energie se nevešlo

/////////////////////////////////////////////////////////////////////////////////






#endif
