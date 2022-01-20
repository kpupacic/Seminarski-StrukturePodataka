#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX 100
#define MAX_STEPS 10

struct _Sastojci;
typedef struct _Sastojci* PositionS;
typedef struct _Sastojci{
    char sastojak[MAX];
    char kolicina[MAX];
    PositionS next;
} Sastojci;

struct _Recept;
typedef struct _Recept* PositionR;
typedef struct _Recept{
    char naziv[MAX];
    char kategorija[MAX];
    char opis[MAX_LINE];
    int vrijeme;
    PositionS sastojci;
    char upute[MAX_STEPS*MAX_LINE];
    PositionR next;
} Recept;

int GlavniIzbornik(PositionR head);                                     // glavni izbornik, sve ponudene radnje
int IspisiSavSadrzaj(char* nazivDatoteke);                              // ispisuje cijeli sadrzaj kuharice
int IspisiSveKategorije(char* nazivDatoteke);                           // ispisuje dostupne kategorije
int IspisiNaziveIzKategorije(char* nazivDatoteke);                      // ispisuje nazive jedne kategorije
int PronadiReceptePoVremenu(char *nazivDatoteke, int vrijeme);          // trazi postoje li recepti za trazeno vrijeme pripreme
int PronadiReceptPoNazivu(PositionR head, PositionR fav);               // recept se trazi dok se ne upise postojeci naziv ili dok se ne odabere povratak na glavni izbornik
int IspisiReceptePoVremenu(char* nazivDatoteke, int priprema);          // ispisuje nazive recepata trazenog vremena pripreme
PositionR PronadiNazivUListi(PositionR head, char naziv[]);             // trazi postoji li trazeni recept u vezanoj listi
int UcitajRecept(PositionR head, char naziv[]);                         // ucitava podatke o receptu iz datoteke u strukturu
int UpisiSastojke(PositionS sastojci, char sastojak[], char kolicina[]);// ucitava sastojke u strukturu
int SortiraniUnos(PositionR head, char naziv[], char kategorija[], char opis[], int vrijeme, PositionS sastojci, char upute[]); // sortirano unosi recept u vezanu listu
PositionR StvoriRecept(char naziv[], char kategorija[], char opis[], int vrijeme, PositionS sastojci, char upute[]);            // stvara novi recept
int SpremiUFavorite(PositionR fav, PositionR recept);                   // sprema otvoreni recept u vezanu listu najdrazih recepata
int IspisRecepta(PositionR recept);                                     // ispisuje jedan recept
int IspisSastojaka(PositionS head);                                     // ispisuje sastojke jednog recepta
int TraziDatoteku(char nazivDatoteke[]);                                // provjerava postoji li datoteka koju pokusavamo otvoriti
int UpisiFavoriteUDatoteku(PositionR fav, char nazivDatoteke[]);        // upisuje recepte koje je korisnik oznacio kao najdraze u datoteku