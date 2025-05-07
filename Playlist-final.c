#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNGIME_MAXIMA_NUME 100


typedef struct {
    char nume[LUNGIME_MAXIMA_NUME];
} Melodie;

typedef struct {
    char nume[LUNGIME_MAXIMA_NUME];
    int rating;
    Melodie *melodie;
    int melodieContorizare;
} Playlist;

void afiseazaMeniu() {
    printf("\nMeniu:\n");
    printf("\033[0;31m1. \033[0;33mCreeaza playlist\n");
    printf("\033[0;31m2. \033[0;33mSterge playlist\n");
    printf("\033[0;31m3. \033[0;33mAdauga o melodie intr-un playlist\n");
    printf("\033[0;31m4. \033[0;33mSterge o melodie dintr-un playlist\n");
    printf("\033[0;31m5. \033[0;33mVizualizeaza playlist\n");
    printf("\033[0;31m6. \033[0;33mIesi\n");
    printf("\033[0;37mIntrodu optiunea dorita: ");
}

void sorteazaPlaylisturi(Playlist *playlisturi, int playlisturiContorizare) {
    for (int i = 0; i < playlisturiContorizare - 1; i++) {
        for (int j = 0; j < playlisturiContorizare - i - 1; j++) {
            if (playlisturi[j].rating < playlisturi[j + 1].rating) {
                Playlist temp = playlisturi[j];
                playlisturi[j] = playlisturi[j + 1];
                playlisturi[j + 1] = temp;
            }
        }
    }
}

int contorizarePlaylist(const char *filename, Playlist **playlisturi) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Nu s-a putut deschide fisierul.\n");
        return 0;
    }

    int contorizarePlaylisturi;
    fscanf(file, "%d", &contorizarePlaylisturi);

    *playlisturi = (Playlist*) malloc(contorizarePlaylisturi * sizeof(Playlist));
    if (*playlisturi == NULL) {
        printf("Nu s-a putut aloca memorie\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < contorizarePlaylisturi; i++) {
        fscanf(file, " %[^\n]", (*playlisturi)[i].nume);
        fscanf(file, " %d", &(*playlisturi)[i].rating);
        fscanf(file, "%d", &(*playlisturi)[i].melodieContorizare);

        (*playlisturi)[i].melodie =(Melodie*) malloc((*playlisturi)[i].melodieContorizare * sizeof(Melodie));
        for (int j = 0; j < (*playlisturi)[i].melodieContorizare; j++) {
            fscanf(file, " %[^\n]", (*playlisturi)[i].melodie[j].nume);
        }
    }

    fclose(file);
    return contorizarePlaylisturi;
}


void salveazaIntrare(const char *filename, Playlist *playlisturi, int contorizarePlaylisturi) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Nu s-a putut deschide fisierul.\n");
        return;
    }

    fprintf(file, "%d\n", contorizarePlaylisturi);
    for (int i = 0; i < contorizarePlaylisturi; i++) {
        fprintf(file, "%s\n", playlisturi[i].nume);
        fprintf(file, "%d\n", playlisturi[i].rating);
        fprintf(file, "%d\n", playlisturi[i].melodieContorizare);
        for (int j = 0; j < playlisturi[i].melodieContorizare; j++) {
            fprintf(file, "%s\n", playlisturi[i].melodie[j].nume);
        }
    }

    fclose(file);
}

void adaugaPlaylist(Playlist **playlisturi, int *playlisturiContorizare) {
    *playlisturi = (Playlist*) realloc(*playlisturi, (*playlisturiContorizare + 1) * sizeof(Playlist));
    if (*playlisturi == NULL) {
        printf("Nu s-a putut aloca memorie!\n");
        return;
    }

    printf("Introdu numele playlistului: ");
    scanf(" %[^\n]", (*playlisturi)[*playlisturiContorizare].nume);

    printf("Introdu rating-ul playlistului: ");
    scanf(" %d", &(*playlisturi)[*playlisturiContorizare].rating);
    
    int stop = 0;
    while(stop == 0) {
    if((*playlisturi)[*playlisturiContorizare].rating < 5 || (*playlisturi)[*playlisturiContorizare].rating > 100) {
        printf("\nValoarea introdusa este gresita. Rating-ul este o valoare intre \033[0;31m5 si 100 \033[0;37m. ");
        printf("\nIntrodu rating-ul playlistului: ");
    scanf(" %d", &(*playlisturi)[*playlisturiContorizare].rating);
    } else{stop=1;}
        
    }

    (*playlisturi)[*playlisturiContorizare].melodie = NULL;
    (*playlisturi)[*playlisturiContorizare].melodieContorizare = 0;

    (*playlisturiContorizare)++;

    sorteazaPlaylisturi(*playlisturi, *playlisturiContorizare);

    printf("Playlist creeat cu succes!\n");
}

void stergePlaylist(Playlist **playlisturi, int *playlisturiContorizare) {
    if (*playlisturiContorizare == 0) {
        printf("Nu exista playlisturi care sa poata fi sterse.\n");
        return;
    }

    char countryName[LUNGIME_MAXIMA_NUME];
    printf("Introdu numele playlistului pe care vrei sa il stergi: ");
    scanf(" %[^\n]", countryName);

    int index = -1;
    for (int i = 0; i < *playlisturiContorizare; i++) {
        if (strcmp((*playlisturi)[i].nume, countryName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Playlistul nu a fost gasit.\n");
        return;
    }

    free((*playlisturi)[index].melodie);

    for (int i = index; i < *playlisturiContorizare - 1; i++) {
        (*playlisturi)[i] = (*playlisturi)[i + 1];
    }

    *playlisturi = (Playlist*) realloc(*playlisturi, (*playlisturiContorizare - 1) * sizeof(Playlist));
    (*playlisturiContorizare)--;

    printf("Playlistul a fost sters cu succes!\n");
}

void adaugaMelodie(Playlist *playlisturi, int playlisturiContorizare) {
    if (playlisturiContorizare == 0) {
        printf("Nu exista playlisturi in care sa poata fi adaugate melodii.\n");
        return;
    }

    printf("\nPlaylisturi:\n");
    for (int i = 0; i < playlisturiContorizare; i++) {
        printf("\n%d. \033[0;35m%s \033[0;37m\n", i+1, playlisturi[i].nume);
    }

    int choice;
    printf("Introdu numarul playlistului in care vrei sa introduci melodii: ");
    scanf("%d", &choice);

    char countryName[LUNGIME_MAXIMA_NUME];

    int countryIndex = choice-1;

    playlisturi[countryIndex].melodie = (Melodie*) realloc(playlisturi[countryIndex].melodie,
                                             (playlisturi[countryIndex].melodieContorizare + 1) * sizeof(Melodie));

    if (playlisturi[countryIndex].melodie == NULL) {
        printf("Nu s-a putut aloca memorie!\n");
        return;
    }

    printf("Introdu numele melodiei: ");
    scanf(" %[^\n]", playlisturi[countryIndex].melodie[playlisturi[countryIndex].melodieContorizare].nume);
    playlisturi[countryIndex].melodieContorizare++;

    printf("Melodia a fost adaugata cu succes!\n");
}

void stergeMelodie(Playlist *playlisturi, int playlisturiContorizare) {
    if (playlisturiContorizare == 0) {
        printf("Nu exista playlisturi din care sa poata fi sterse melodii.\n");
        return;
    }

    char countryName[LUNGIME_MAXIMA_NUME];

    printf("\nPlaylisturi:\n");
    for (int i = 0; i < playlisturiContorizare; i++) {
        printf("\n%d. %s\n", i+1, playlisturi[i].nume);
    }

    int choice;
    printf("Introdu numarul playlistului din care vrei sa stergi melodii: ");
    scanf("%d", &choice);

    int countryIndex = choice-1;


    if (playlisturi[countryIndex].melodieContorizare == 0) {
        printf("Nu exista melodii in acest playlist\n");
        return;
    }

    char cityName[LUNGIME_MAXIMA_NUME];

    for (int j = 0; j < playlisturi[countryIndex].melodieContorizare; j++) {
                printf("  %d. %s\n", j+1, playlisturi[countryIndex].melodie[j].nume);
            }

            int cityChoice;
    printf("Introdu numarul melodiei pe care vrei sa o stergi: ");
    scanf("%d", &cityChoice);

    int cityIndex = cityChoice-1;
    for (int i = cityIndex; i < playlisturi[countryIndex].melodieContorizare - 1; i++) {
        playlisturi[countryIndex].melodie[i] = playlisturi[countryIndex].melodie[i + 1];
    }

    playlisturi[countryIndex].melodie = (Melodie*) realloc(playlisturi[countryIndex].melodie,
                                             (playlisturi[countryIndex].melodieContorizare - 1) * sizeof(Melodie));

    playlisturi[countryIndex].melodieContorizare--;

    printf("Melodia a fost setearsa cu succes!\n");
}

void afiseazaPlaylist(Playlist *playlisturi, int playlisturiContorizare) {
    if (playlisturiContorizare == 0) {
        printf("Nu exista playlisturi.\n");
        return;
    }

    printf("\nPlaylisturi:\n");
    for (int i = 0; i < playlisturiContorizare; i++) {
        printf("\n%d. \033[0;35m%s \033[0;37m- \033[0;34m%d \033[0;37m\n", i+1, playlisturi[i].nume, playlisturi[i].rating);
        }

        int choice;
    printf("Introdu numarul playlistului din care vrei sa vezi melodiile: ");
    scanf("%d", &choice);

    if (playlisturi[choice-1].melodieContorizare == 0) {
            printf("  Nu sunt melodii in acest playlist.\n");
        } else {
            printf("\nMelodiile din playlistul \033[0;34m%s \033[0;37msunt:", playlisturi[choice-1].nume);
            for (int j = 0; j < playlisturi[choice-1].melodieContorizare; j++) {
                printf("\n%d. \033[0;35m%s \033[0;37m\n", j+1, playlisturi[choice-1].melodie[j].nume);
            }
        }
}

int main() {
    Playlist *playlisturi = NULL;
    int playlisturiContorizare = 0;
    int choice;

    const char *filename = "playlist.txt";

    playlisturiContorizare = contorizarePlaylist(filename, &playlisturi);

    while (1) {
        afiseazaMeniu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adaugaPlaylist(&playlisturi, &playlisturiContorizare);
                break;
            case 2:
                stergePlaylist(&playlisturi, &playlisturiContorizare);
                break;
            case 3:
                adaugaMelodie(playlisturi, playlisturiContorizare);
                break;
            case 4:
                stergeMelodie(playlisturi, playlisturiContorizare);
                break;
            case 5:
                afiseazaPlaylist(playlisturi, playlisturiContorizare);
                break;
            case 6:
                // Free dynamically allocated memory before exit
                salveazaIntrare(filename, playlisturi, playlisturiContorizare);
                for (int i = 0; i < playlisturiContorizare; i++) {
                    free(playlisturi[i].melodie);
                }
                free(playlisturi);
                printf("Termina...\n");
                return 0;
            default:
                printf("Valoarea introdusa este incorecta, incearca din nou.\n");
        }
    }

    return 0;
}
