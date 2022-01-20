#define _CRT_SECURE_NO_WARNINGS
#include "funkcije.h"

int GlavniIzbornik(PositionR head){

    char nazivDatoteke[MAX];
    char nazivKategorije[MAX];
    char tempDatoteka[MAX];
    int odabir = 0;
    int tempOdabir = 0, status = 0;
    char tempZnak;
    Recept headFavoriti = {.naziv = {0}, .kategorija = {0}, .opis = {0}, .vrijeme = 0, .sastojci = NULL, .upute = {0}, .next = NULL};
    PositionR favoriti = &headFavoriti;

    strcpy(nazivDatoteke, "kuharica.txt");

    while(1){

        printf("\n(0) - Izadi iz programa."
               "\n(1) - Trazi recepte po nazivu."
               "\n(2) - Trazi recepte po kategoriji."
               "\n(3) - Trazi recepte po vremenu pripreme."
               "\n(4) - Ispisi favorite u datoteku.");
        printf("\n\nUpisite broj zeljene radnje: ");
        scanf(" %d", &odabir);

        switch(odabir){
            
            case 0:
                return 0;

            case 1:

                IspisiSavSadrzaj(nazivDatoteke);
                PronadiReceptPoNazivu(head, favoriti);

                break;
                
            case 2:

                printf("\nDostupne kategorije:\n");
                IspisiSveKategorije(nazivDatoteke);

                printf("\nUpisite naziv kategorije koju zelite otvoriti: ");
                scanf(" %[^\n]s", nazivKategorije);
                strcat(nazivKategorije, ".txt");

                printf("\nDostupni recepti:\n");
                IspisiNaziveIzKategorije(nazivKategorije);
                PronadiReceptPoNazivu(head, favoriti);

                break;

            case 3:

                tempOdabir = 0;

                while(tempOdabir < 1 || tempOdabir > 3){

                    printf("\nVrijeme pripreme:\n"
                           "(1) - kratko, do 30 min.\n"
                           "(2) - srednje, do 2h\n"
                           "(3) - dugo, vise od 2h\n");

                    printf("\nOdabir: ");
                    scanf("%d", &tempOdabir);
                    if(tempOdabir < 1 || tempOdabir > 3){
                        printf("\nOdabir nije podrzan! Pokusajte ponovo!\n");
                    }
                }

                status = PronadiReceptePoVremenu(nazivDatoteke, tempOdabir);
                if(status!=0){
                    PronadiReceptPoNazivu(head, favoriti);
                }

                break;

            case 4:

                status = 0;

                printf("Jeste li sigurni da zelite ispisati sve oznacene recepte u datoteku? (Y/N): ");
                scanf(" %c", &tempZnak);

                if(toupper(tempZnak) == 'Y'){
                    while(status == 0){
                        printf("\nUpisite naziv datoteke: ");
                        scanf(" %[^\n]s", tempDatoteka);
                        status = TraziDatoteku(tempDatoteka);
                        if(status == 0){
                            printf("\nNaziv zauzet! Pokusajte drugi naziv.");
                        }
                    }
                    UpisiFavoriteUDatoteku(favoriti, tempDatoteka);
                    printf("\nUpisivanje...\nUpisivanje...\nUpisivanje...\n"
                            "Recepti uspjesno upisani u datoteku!\n");
                }
                else if(toupper(tempZnak) == 'N'){
                    printf("\nRadnja prekinuta.\n");
                }
                else{
                    printf("\nOdabir nije podrzan! Pokusajte ponovo!\n");
                }

                break;

            default:
                printf("\nOdabir nije podrzan! Pokusajte ponovo.\n");
                break;
        }
    }

    return EXIT_SUCCESS;
}

int IspisiSavSadrzaj(char* nazivDatoteke){

    FILE *dat = NULL;
    char buffer[MAX_LINE] = {0};
    char *tempBuffer = NULL;
    int i = 0;
    char kategorija[MAX];
    char tempKategorija[MAX];

    dat = fopen(nazivDatoteke, "r");

    if(!dat){
        perror("Problem pri otvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    // iz glavne datoteke citamo nazive recepata, kategoriju po kategoriju
    while(!feof(dat)){

        // citanje linije
        fgets(buffer, MAX_LINE, dat);
        tempBuffer = buffer;
        memset(kategorija, 0, sizeof(kategorija));
        sscanf(tempBuffer, " %[^\n]s", &kategorija);

        printf("\n");
        for(i=0; i < strlen(kategorija); i++){
            printf("%c", toupper(kategorija[i]));
        }
        printf("\n");

        strcpy(tempKategorija, kategorija);
        strcat(tempKategorija, ".txt");

        // citanje naziva recepata te kategorije
        IspisiNaziveIzKategorije(tempKategorija);
    }

    fclose(dat);

    return EXIT_SUCCESS;
}

int IspisiSveKategorije(char* nazivDatoteke){

    FILE *dat = NULL;
    char buffer[MAX_LINE] = {0};
    char *tempBuffer = NULL;
    char kategorija[MAX];

    dat = fopen(nazivDatoteke, "r");

    if(!dat){
        perror("Problem pri otvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    while(!feof(dat)){

        // citanje linije
        fgets(buffer, MAX_LINE, dat);
        tempBuffer = buffer;
        memset(kategorija, 0, sizeof(kategorija));
        sscanf(tempBuffer, " %[^\n]s", &kategorija);

        printf("- %s\n", kategorija);
    }

    fclose(dat);

    return EXIT_SUCCESS;
}

int IspisiNaziveIzKategorije(char* nazivDatoteke){

    FILE *dat = NULL;
    char buffer[MAX_LINE] = {0};
    char *tempBuffer = NULL;
    int priprema;
    char *naziv;

    dat = fopen(nazivDatoteke, "r");

    if(!dat){
        perror("Problem pri otvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    while(!feof(dat)){

        // citanje linije
        fgets(buffer, MAX_LINE, dat);
        tempBuffer = buffer;
        sscanf(tempBuffer, " %d %[^\n]s", &priprema, &naziv);
        printf("- %s\n", &naziv);
    }

    fclose(dat);

    return EXIT_SUCCESS;
}

int PronadiReceptePoVremenu(char* nazivDatoteke, int vrijeme){

    FILE *dat = NULL;
    char buffer[MAX_LINE] = {0};
    char *tempBuffer = NULL;
    char kategorija[MAX];
    int br = 0;

    dat = fopen(nazivDatoteke, "r");

    if(!dat){
        perror("Problem pri otvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    // iz glavne datoteke citamo vrijeme pripreme recepata
    while(!feof(dat)){

        // citanje linije
        fgets(buffer, MAX_LINE, dat);
        tempBuffer = buffer;
        
        memset(kategorija, 0, sizeof(kategorija));
        sscanf(tempBuffer, " %[^\n]s", &kategorija);
        strcat(kategorija, ".txt");

        br = br + IspisiReceptePoVremenu(kategorija, vrijeme);
    }
    if(br == 0){
        printf("\nNema recepata s tim vremenom pripreme!\n");
    }

    fclose(dat);

    return br;
}

int PronadiReceptPoNazivu(PositionR head, PositionR fav){

    PositionR recept = NULL;
    char nazivRecepta[MAX];
    char natrag = 'Y';
    char odabir = 'N';

    while(recept == NULL && natrag == 'Y'){

        memset(nazivRecepta, 0, sizeof(nazivRecepta));
        printf("\nUpisite naziv recepta kojeg zelite otvoriti: ");
        scanf(" %[^\n]s", nazivRecepta);
        printf("\n");
        recept = PronadiNazivUListi(head, nazivRecepta);

        // ako recept ne postoji u vezanoj listi, trazi se njegovu datoteku i sprema se u listu
        if(recept==NULL){
            UcitajRecept(head, nazivRecepta);
            recept = PronadiNazivUListi(head, nazivRecepta);
        }

        // ako se recept i dalje ne moze naci, znaci da je naziv krivo upisan, opcija ponovnog trazenja
        if(recept==NULL){
            printf("Krivo upisan naziv!\n"
                    "Zelite li ponovo traziti recept? (Y/N): ");
            scanf(" %c", &natrag);
            natrag = toupper(natrag);
        }
        else{
            IspisRecepta(recept);
            printf("\nZelite li spremiti recept u favorite? (Y/N): ");
            scanf(" %c", &odabir);
            if(toupper(odabir) == 'Y'){
                SpremiUFavorite(fav, recept);
            }
            else if(toupper(odabir) == 'N'){
                continue;
            }
            else{
                printf("\nOdabir nije podrzan! Radnja prekinuta.\n");
            }
        }
    }

    return EXIT_SUCCESS;
}

int IspisiReceptePoVremenu(char *nazivDatoteke, int vrijeme){

    FILE *dat = NULL;
    char buffer[MAX_LINE] = {0};
    char *tempBuffer = NULL;
    char *naziv;
    int priprema = 0, br = 0;

    dat = fopen(nazivDatoteke, "r");

    if(!dat){
        perror("Problem pri otvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    while(!feof(dat)){

        fgets(buffer, MAX_LINE, dat);
        tempBuffer = buffer;
        sscanf(tempBuffer, " %d %[^\n]s", &priprema, &naziv);

        switch(vrijeme){

            case 1:
                if(priprema<=30){
                    printf("- %s\n", &naziv);
                    ++br;
                }
                break;
            case 2:
                if(priprema > 30 && priprema <= 120){
                    printf("- %s\n", &naziv);
                    ++br;
                }
                break;
            case 3:
                if(priprema >= 120){
                    printf("- %s\n", &naziv);
                    ++br;
                }
                break;
        }
        
    }

    fclose(dat);

    return br;
}

PositionR PronadiNazivUListi(PositionR head, char naziv[]){

    PositionR recept = NULL;
    int i = 0;
    char tempNaziv[MAX];
    char tekstToUpper[MAX] = {0};

    // upisani naziv pretvaramo u ispis velikim slovima, zbog string comparea
    for(i=0; i < strlen(naziv); i++){
        tekstToUpper[i] = toupper(naziv[i]);
        tempNaziv[i] = tekstToUpper[i];
    }

    recept = head->next;

    while(recept){
        if(strcmp(recept->naziv, tempNaziv)==0){
            return recept;
        }
        recept = recept->next;
    }

    return recept;
}

int UcitajRecept(PositionR head, char naziv[]){

    char nazivDatoteke[MAX];
    FILE *recept = NULL;
    PositionR novi = NULL;
    char buffer[MAX_LINE] = {0};
    char *tempBuffer = NULL;
    char tempZnak;
    char tempNaziv[MAX];
    char tempKategorija[MAX];
    char tempOpis[MAX_LINE];
    int tempVrijeme;
    PositionS tempSastojci = NULL;
    char tempSastojak[MAX];
    char tempKolicina[MAX];
    char tempUpute[MAX_STEPS*MAX_LINE];

    strcpy(nazivDatoteke, naziv);
    strcat(nazivDatoteke, ".txt");

    recept = fopen(nazivDatoteke, "r");

    if(!recept){
        perror("Problem pri otvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    // citanje naziva recepta
    fgets(buffer, MAX_LINE, recept);
    tempBuffer = buffer;
    sscanf(tempBuffer, " %[^\n]s", &tempNaziv);

    // citanje kategorije recepta
    fgets(buffer, MAX_LINE, recept);
    tempBuffer = buffer;
    sscanf(tempBuffer, " %s", &tempKategorija);

    // citanje "opisa" recepta
    fgets(buffer, MAX_LINE, recept);
    tempBuffer = buffer;
    sscanf(tempBuffer, " %[^\n]s", &tempOpis);

    // preskakanje reda (podnaslov "vrijeme")
    fgets(buffer, MAX_LINE, recept);

    // citanje vremena potrebnog za pripremu
    fgets(buffer, MAX_LINE, recept);
    tempBuffer = buffer;
    sscanf(tempBuffer, " %d", &tempVrijeme);

    // preskakanje reda (podnaslov "sastojci")
    fgets(buffer, MAX_LINE, recept);
    // alociranje memorije za sastojke trenutnog recepta
    tempSastojci = (PositionS)malloc(sizeof(Sastojci));
    // citanje sastojaka i njihove kolicine
    fgets(buffer, MAX_LINE, recept);
    tempBuffer = buffer;
    sscanf(tempBuffer, " %[^\n]s", &tempSastojak);                  // citanje prvog sastojka
    while(strcmp(tempSastojak, "UPUTE")!=0){                        // citamo sve dok ne dodemo do novog podnaslova
        fgets(buffer, MAX_LINE, recept);
        tempBuffer = buffer;
        sscanf(tempBuffer, " %[^\n]s", &tempKolicina);              // citanje kolicine novog sastojka
        UpisiSastojke(tempSastojci, tempSastojak, tempKolicina);    // upisivanje informacija u strukturu trenutnog recepta
        fgets(buffer, MAX_LINE, recept);
        tempBuffer = buffer;
        sscanf(tempBuffer, " %[^\n]s", &tempSastojak);              // citanje naziva iduceg sastojka
    }

    // citanje uputa
    strcpy(tempUpute, "\0");
    fgets(buffer, MAX_LINE, recept);
    tempBuffer = buffer;
    tempZnak = buffer[0];
    while(!feof(recept)){
        strcat(tempUpute, tempBuffer);
        fgets(buffer, MAX_LINE, recept);
        tempBuffer = buffer;
        tempZnak = buffer[0];
    }

    SortiraniUnos(head, tempNaziv, tempKategorija, tempOpis, tempVrijeme, tempSastojci, tempUpute);

    fclose(recept);

    return EXIT_SUCCESS;
}

PositionR StvoriRecept(char naziv[], char kategorija[], char opis[], int vrijeme, PositionS sastojci, char upute[]){

    PositionR new = NULL;

    new = (PositionR)malloc(sizeof(Recept));

    if(!new){
        perror("Problem pri alociranju memorije! ");
        return NULL;
    }

    strcpy(new->naziv, naziv);
    strcpy(new->kategorija, kategorija);
    strcpy(new->opis, opis);
    new->vrijeme = vrijeme;
    new->sastojci = sastojci;
    strcpy(new->upute, upute);
    new->next = NULL;

    return new;
}

int UpisiSastojke(PositionS sastojci, char sastojak[], char kolicina[]){

    PositionS temp = NULL, novi = NULL;

    temp = sastojci->next;

    novi = (PositionS)malloc(sizeof(Sastojci));

    if(!novi){
        perror("Problem pri alociranju memorije! ");
        return EXIT_FAILURE;
    }

    strcpy(novi->sastojak, sastojak);
    strcpy(novi->kolicina, kolicina);
    novi->next = NULL;

    if(sastojci->next == NULL){
        sastojci->next = novi;
    }
    else{
        while(temp->next!=NULL){
            temp = temp->next;
        }
        temp->next = novi;
    }

    return EXIT_SUCCESS;
}

int SortiraniUnos(PositionR head, char naziv[], char kategorija[], char opis[], int vrijeme, PositionS sastojci, char upute[]){

    PositionR prev = NULL, temp = NULL, new = NULL;

    prev = head;
    temp = head->next;
    new = StvoriRecept(naziv, kategorija, opis, vrijeme, sastojci, upute);

    // ako je head prazan, novi recept upisujemo na prvo mjesto, inace sortiramo
    if(head->next == NULL){
        head->next = new;
    }
    else{
        // prvo sortiramo abecedno, po kategoriji
        while(temp!=NULL && (strcmp(temp->kategorija, new->kategorija) < 0)){
            prev = temp;
            temp = temp->next;
        }
        // pa sortiramo abecedno, po nazivu, dok se nalazimo unutar iste kategorije
        while(temp!=NULL && (strcmp(temp->kategorija, new->kategorija) == 0) && (strcmp(temp->naziv, new->naziv) < 0)){
            prev = temp;
            temp = temp->next;
        }        
        new->next = prev->next;
        prev->next = new;
    }

    return EXIT_SUCCESS;
}

int SpremiUFavorite(PositionR fav, PositionR recept){

    PositionR prev = NULL, temp = NULL, new = NULL;

    prev = fav;
    temp = fav->next;
    new = recept;

    if(fav->next == NULL){
        fav->next = new;
    }
    else{
        while(temp!=NULL && (strcmp(temp->kategorija, new->kategorija) < 0)){
            prev = temp;
            temp = temp->next;
        }
        while(temp!=NULL && (strcmp(temp->kategorija, new->kategorija) == 0) && (strcmp(temp->naziv, new->naziv) < 0)){
            prev = temp;
            temp = temp->next;
        }        
        new->next = prev->next;
        prev->next = new;
    }

    return EXIT_SUCCESS;
}

int IspisRecepta(PositionR recept){

    printf("\n~ %s ~\n", recept->naziv);
    printf("\nkategorija: %s\n", recept->kategorija);
    printf("%s\n", recept->opis);
    // sredeni ispis vremena, posto je informacija spremljena u minutama
    if(recept->vrijeme < 60){
        printf("VRIJEME: %d min.\n", recept->vrijeme);
    }
    else{
        if(recept->vrijeme%60 == 0){
            printf("VRIJEME: %dh\n", recept->vrijeme/60);
        }
        else{
            printf("VRIJEME: %dh %d min.\n", recept->vrijeme/60, recept->vrijeme%60);
        }
    }
    printf("\nSASTOJCI:\n");
    IspisSastojaka(recept->sastojci);
    printf("\nUPUTE:\n");
    printf("%s\n", recept->upute);
    printf("---\n");

    return EXIT_SUCCESS;
}

int IspisSastojaka(PositionS head){

    PositionS temp = NULL;

    temp = head->next;

    while(temp){
        printf("- %s, %s\n", temp->sastojak, temp->kolicina);
        temp = temp->next;
    }

    return EXIT_SUCCESS;
}

int TraziDatoteku(char nazivDatoteke[]){

    FILE* dat = NULL;

    strcat(nazivDatoteke, ".txt");

    dat = fopen(nazivDatoteke, "r");

    // ako datoteka ne postoji, mozemo je stvoriti
    if(!dat){
        return 1;
    }
    
    return 0;
}

int UpisiFavoriteUDatoteku(PositionR fav, char nazivDatoteke[]){

    FILE* dat = NULL;
    PositionR recept = NULL;
    PositionS sastojci = NULL;

    recept = fav->next;

    dat = fopen(nazivDatoteke, "w");

    if(!dat){
        perror("Problem pri stvaranju datoteke! ");
        return EXIT_FAILURE;
    }

    while(recept){
        fprintf(dat, "\n~ %s ~\n", recept->naziv);
        fprintf(dat, "\nkategorija: %s\n", recept->kategorija);
        fprintf(dat, "%s\n", recept->opis);
        if(recept->vrijeme < 60){
            fprintf(dat, "VRIJEME: %d min.\n", recept->vrijeme);
        }
        else if(recept->vrijeme%60 == 0){
            fprintf(dat, "VRIJEME: %dh\n", recept->vrijeme/60);
        }
        else{
            fprintf(dat, "VRIJEME: %dh %d min.\n", recept->vrijeme/60, recept->vrijeme%60);
        }
        fprintf(dat, "\nSASTOJCI:\n");
        sastojci = recept->sastojci->next;
        while(sastojci){
            fprintf(dat, "- %s, %s\n", sastojci->sastojak, sastojci->kolicina);
            sastojci = sastojci->next;
        }
        fprintf(dat, "\nUPUTE:\n");
        fprintf(dat, "%s\n", recept->upute);
        fprintf(dat, "---\n");
        recept = recept->next;
    }

    fclose(dat);

    return EXIT_SUCCESS;
}