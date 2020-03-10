// Copyright 2018 Banu Miruna Elena 311CA <miru.b99@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "./functions.h"

glacier_t **alloc_glacier(int R) {
// aloca memoria matricei-ghetar
    glacier_t **g;
    g = (glacier_t**)malloc((2 * R + 1) * sizeof(glacier_t*));
    for (int i = 0; i < 2 * R + 1; ++i) {
        g[i] = (glacier_t*) malloc((2 * R + 1) * sizeof(glacier_t));
    }
    return g;
}

void free_glacier(glacier_t **g, int R) {
    for (int i = 0; i < 2 * R + 1; ++i) {
        free(g[i]);
    }
    free(g);
}

int on_glacier(int x, int y, int R) {
// verifica daca spiridusul se afla pe ghetar
    if (x >= (2 * R + 1) || y >= (2 * R + 1) || x < 0 || y < 0) {
        return 0;
    }
    if (((x - R) * (x - R) + (y - R) * (y - R)) <= R * R) {
        return 1;
    }
    return 0;
}

void fight(int a, int b, glacier_t **g, elf_t *e, FILE *u) {
// realizeaza lupta dintre elfi
    int i = 0; // indica al cui rand e sa loveasca
    if (e[b].stamina >= e[a].stamina) {
        i = 1;
    }
// i % 2 = 0 - loveste primul spiridus
// i % 2 = 1 - loveste al doilea spiridus (cel care intra in casuta)
    while (e[a].hp > 0 && e[b].hp > 0) {
        if (i % 2 == 0) { // verifica al cui e randul
            e[b].hp -= e[a].dmg;
        } else {
            e[a].hp -= e[b].dmg;
        }
        ++i; // schimba randul
    }
    if (e[a].hp <= 0) {
// daca spiridusul "a" pierde
        e[b].stamina += e[a].stamina; // stamina castigatorului creste
        // cu cea a pierzatorului
        e[a].stamina = 0;
        g[e[b].x][e[b].y].o = b; // castigatorul ramane in casuta
        e[b].eliminated++; // creste numarul de spiridusi
        // eliminati de "b"
        fprintf(u, "%s sent %s back home.\n", e[b].n, e[a].n);
        e[a].hp = 0;
    }
    if (e[b].hp <= 0) {
// daca spiridusul "b" pierde (analog "a")
        e[a].stamina += e[b].stamina;
        e[b].stamina = 0;
        g[e[a].x][e[a].y].o = a;
        e[a].eliminated++;
        fprintf(u, "%s sent %s back home.\n", e[a].n, e[b].n);
        e[b].hp = 0;
    }
}

void change_gloves(int id, glacier_t **g, elf_t *e, int x, int y) {
// verifica daca manusile din noua casuta sunt mai puternice
// si le interschimba
    int aux;
    if (g[x][y].B > e[id].dmg) {
        aux = g[x][y].B;
        g[x][y].B = e[id].dmg;
        e[id].dmg = aux;
    }
}

void move_direction(glacier_t **g, elf_t *e, int mvx, int mvy, int id, int R,
    FILE *u) {
    int dif;
    dif = abs(g[e[id].x][e[id].y].A - g[e[id].x + mvx][e[id].y + mvy].A);
            // calculeaza diferenta de inaltime
            if (e[id].stamina >= dif) {
            // verifica daca are stamina sa faca miscarea
                e[id].stamina -= dif; // scade stamina
                g[e[id].x][e[id].y].o = -1;
                // casuta in care se afla devine libera
                if (on_glacier(e[id].x + mvx, e[id].y + mvy, R)) {
                // daca se afla pe ghetar
                    e[id].x += mvx;
                    e[id].y += mvy;
                    // schimba indicele
                    change_gloves(id, g, e, e[id].x, e[id].y);
                    // schimba manusile
                    if (g[e[id].x][e[id].y].o != -1) {
                        // daca nu e libera casuta are loc batalia
                        fight(g[e[id].x][e[id].y].o, id, g, e, u);
                    } else {
                        g[e[id].x][e[id].y].o = id;
                        // daca e libera se muta acolo
                    }
                } else {
                    fprintf(u, "%s fell off the glacier.\n", e[id].n);
                    // daca nu se afla pe ghetar
                    e[id].hp = 0;
                }
            }
}

void move(glacier_t **g, elf_t *e, char m, int id, int R, FILE *u) {
    if (e[id].hp > 0) {
    // daca elful e inca pe ghetar
        switch (m) {
        // verifica ce miscare face
            case 'U' :
            // mutare in sus
            move_direction(g, e, -1, 0, id, R, u);
            break;
            case 'D' :
            // mutare in jos
            move_direction(g, e, 1, 0, id, R, u);
            break;
            case 'L' :
            // mutare la stanga
            move_direction(g, e, 0, -1, id, R, u);
            break;
            case 'R' :
            // mutare la dreapta
            move_direction(g, e, 0, 1, id, R, u);
            break;
            default: break;
        }
    }
}

void print_scoreboard(elf_t *e, int P, FILE *u) {
    fprintf(u, "SCOREBOARD:\n");
    elf_t aux, *cp;
    // se realizeaza o copie a tabelului
    cp = malloc(P * sizeof(elf_t));
    for (int i = 0; i < P; ++i) {
        cp[i] = e[i];
    }
    for (int i = 0; i < P - 1; ++i) {
        for (int j = i + 1; j < P; ++j) {
            if (cp[i].hp <= 0 && cp[j].hp > 0) {
            // sorteaza dupa hp
                aux = cp[j];
                cp[j] = cp[i];
                cp[i] = aux;
            } else {
                if ((cp[j].hp > 0 && cp[i].hp > 0) ||
                    (cp[i].hp <= 0 && cp[i].hp <= 0)) {
                    if (cp[j].eliminated > cp[i].eliminated) {
                        // sorteaza dupa numarul de spiridusi eliminati
                        aux = cp[j];
                        cp[j] = cp[i];
                        cp[i] = aux;
                    } else {
                        if (cp[j].eliminated == cp[i].eliminated) {
                            if (strcmp(cp[i].n, cp[j].n) > 0) {
                                // sorteaza lexicografic
                                aux = cp[j];
                                cp[j] = cp[i];
                                cp[i] = aux;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < P; ++i) {
        fprintf(u, "%s\t", cp[i].n);
        if (cp[i].hp > 0) {
            fprintf(u, "DRY\t");
        } else {
            fprintf(u, "WET\t");
        }
        fprintf(u, "%d\n", cp[i].eliminated);
    }
    free(cp);
}

void snowstorm(int k, elf_t *e, int id, FILE *u, glacier_t **g) {
    int dmg, rs, ys, xs, kc;
    kc = k; // i se face copie lui k
    xs = k & 255;
    k = k >> 8;
    ys = k & 255;
    k = k >> 8;
    rs = k & 255;
    dmg = k >> 8;
    // se afla numerele prin shiftari repetate la dreapta ale lui k
    if (kc < 0) {
    // daca k e negativ
        dmg = -dmg;
    }
    if (e[id].hp > 0) {
        if (((e[id].x - xs) *
            (e[id].x - xs) + (e[id].y - ys) * (e[id].y - ys))
            <= rs * rs) {
            // verifica daca spiridusul e in raza furtunii
            e[id].hp -= dmg;
        }
        if (e[id].hp <= 0) {
            // daca nu mai are energie
            fprintf(u, "%s was hit by snowstorm.\n", e[id].n);
            e[id].hp = 0;
            g[e[id].x][e[id].y].o = -1;
        }
    }
}

glacier_t **meltdown(glacier_t **g, elf_t *e, int R,
    int P, int stam, FILE *u) {
    glacier_t **h;
    h = alloc_glacier(R - 1);
    // se aloca o noua matrice de dimensiune R - 1
    for (int i = 1; i < (2 * R); ++i) {
        for (int j = 1; j < (2 * R); ++j) {
            h[i - 1][j - 1] = g[i][j];
        }
    }
    // se copiaza in noua matrice valorile lui g
    // exceptand liniile/coloanele exterioare
    for (int i = 0; i < P; ++i) {
        e[i].x = e[i].x - 1;
        e[i].y = e[i].y - 1;
        // se scad indicii casutelor unde sunt spiridusii
        if (on_glacier(e[i].x, e[i].y, R - 1) && e[i].hp > 0) {
            // daca se afla pe ghetar si inca e in joc
            h[e[i].x][e[i].y].o = i;
            // ramane pe ghetar
            e[i].stamina += stam;
            // creste stamina
        } else {
            if (e[i].hp > 0) {
                // daca nu mai e pe ghetar si inca era in joc
                fprintf(u, "%s got wet because of global warming.\n", e[i].n);
                e[i].hp = 0;
                if (e[i].x >= 0 && e[i].y >= 0
                    && e[i].x < 2 * R - 1 && e[i].y < 2 * R - 1) {
                    // daca e in matrice
                    // dar nu in perimetrul ghetarului
                    h[e[i].x][e[i].y].o = -1;
                    // se elibereaza casuta
                }
            }
            e[i].hp = 0;
            // iese din joc
        }
    }
    free_glacier(g, R);
    return h;
}

void win(glacier_t **g, int R, int v[2]) {
// verifica daca a mai ramas doar un spiridus
    v[0] = 0; // numarul initial de spiridusi = 0
    for (int i = 0; i < (2 * R + 1); ++i) {
        for (int j = 0; j < (2 * R + 1); ++j) {
            if (g[i][j].o != -1) {
                v[0]++; // verifica daca mai sunt spiridusi pe ghetar
                v[1] = g[i][j].o; // indicele spiridusului din ghetar
            }
        }
    }
}
