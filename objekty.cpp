#include "objekty.hpp"

/////////////////////////////////////////////////////////////////////////////////

static bool __zima = true;
static int __hodina = 0;
static int __den = 0;
static int __tyden = 0;
static int __rok = 0;


/////////////////////////////// ČAS /////////////////////////////////////////////

bool zima(){
	if(__tyden >= 9 && __tyden <= 39 )
		__zima = false;
	else
		__zima = true;
	return __zima;
}

int tyden(){
	return __tyden;
}
int tyden_gauss(){
	int tmp = __tyden;
	if(tmp > 26){
		tmp = 27 - (tmp - 25);
	}
	return tmp;
}

int den(){
	return __den;
}

int hodina(){
	return __hodina;
}

int rok(){
	return __rok;
}

void hodina_pridej(int h){
	__hodina += h;
	if(__hodina > 23){
		__hodina = 0;
		__den++;
	}
	if(__den > 7){
		__den = 0;
		__tyden++;
	}
	if(__tyden > 52){
		__tyden = 0;
		__rok++;
	}
}

void doba_reset(){
	__hodina = 0;
	__den = 0;
	__tyden = 0;
	__rok = 0;
	__zima = true;
}


