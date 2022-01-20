#define _CRT_SECURE_NO_WARNINGS
#include "funkcije.h"

int main(){

    Recept headRecept = {.naziv = {0}, .kategorija = {0}, .opis = {0}, .vrijeme = 0, .sastojci = NULL, .upute = {0}, .next = NULL};
    PositionR headR = &headRecept;

    GlavniIzbornik(headR);

    return 0;
}