// Copyright 2018 Banu Miruna Elena 311CA <miru.b99@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "./functions.h"

int main() {
    int P, R, v[2] = {0}, id, stam, k;
    FILE *f = fopen("snowfight.in", "rt");
    FILE *u = fopen("snowfight.out", "wt");
    fscanf(f, "%d%d", &R, &P);
    glacier_t **g;
    elf_t *e;
    e = malloc(P * sizeof(elf_t));
    g = alloc_glacier(R);
    for (int i = 0; i < (2 * R + 1); ++i) {
        for (int j = 0; j < (2 * R + 1) ; ++j) {
            fscanf(f, "%d %d", &g[i][j].A, &g[i][j].B);
            g[i][j].o = -1;
                // matricea e libera initial
        }
    }
    for (int i = 0; i < P; ++i) {
        fscanf(f, "%s%d%d%d%d", e[i].n, &e[i].x, &e[i].y,
            &e[i].hp, &e[i].stamina);
        e[i].id = i;
        e[i].eliminated = 0; // initial nu a eliminat spiridusi
        e[i].dmg = g[e[i].x][e[i].y].B;
        // ia manusile gasite in celula
        g[e[i].x][e[i].y].B = 0;
        // in celula nu mai sunt manusi
        if (on_glacier(e[i].x, e[i].y, R)) {
                g[e[i].x][e[i].y].o = e[i].id;
        // daca aterizeaza pe ghetar, ocupa casuta
        } else {
            fprintf(u, "%s has missed the glacier.\n", e[i].n);
            e[i].hp = 0;
            // iese din joc
        }
    }
    char comm[20], m;
    while (fscanf(f, "%s", comm) == 1) {
        // cat timp se pot citi comenzi
        win(g, R, v);
        // verifica daca a castigat cineva
        if (v[0] != 1) {
            // daca nu a castigat nimeni verifica ce comanda urmeaza
            char c1[] = "MOVE";
            if (strcmp(comm, c1) == 0) {
            // daca s-a dat comanda move
                fscanf(f, "%d", &id);
                fscanf(f, "%c", &m);
                while (!feof(f) && m != '\n') {
                // cat timp se citesc directii de pe acelasi rand
                // sau nu s-a ajuns la finalul fisierului
                    fscanf(f, "%c", &m);
                    if (feof(f)) {
                        break;
                    }
                    move(g, e, m, id, R, u);
                    // realizeaza comanda move
                }
            }
            char c2[] = "SNOWSTORM";
            if (strcmp(comm, c2) == 0) {
                // daca se da comanda snowstorm
                fscanf(f, "%d", &k);
                for (int i = 0; i < P; ++i) {
                   snowstorm(k, e, i, u, g);
                   // realizeaza comanda snowstorm
                }
            }
            char c3[] = "PRINT_SCOREBOARD";
            if (strcmp(comm, c3) == 0) {
            // daca se da comanda print scoreboard
                print_scoreboard(e, P, u);
            }
            char c4[] = "MELTDOWN";
            if (strcmp(comm, c4) == 0) {
                // daca se da comanda meltdown
                fscanf(f, "%d", &stam);
                g = meltdown(g, e, R, P, stam, u);
                // se realizeaza comanda meltdown
                R -= 1;
                // scade raza
            }
        } else {
            // daca s-a castigat
            fprintf(u, "%s has won.\n", e[v[1]].n);
            break;
        }
    }
    free_glacier(g, R);
    free(e);
    fclose(f);
    fclose(u);
    return 0;
}
