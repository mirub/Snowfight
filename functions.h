// Copyright 2018 Banu Miruna Elena 311CA <miru.b99@gmail.com>

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define NMAX 100

typedef struct {
// defineste caracteristicile ghetarului
    int A, B, o; // o = id ul spiridusului care ocupa celula
    // o = 0 daca celula e libera
} glacier_t;

typedef struct {
// defineste caracteristicile unui spiridus
    char n[NMAX];
    int x, y, hp, stamina, id, dmg, eliminated;
} elf_t;

glacier_t **alloc_glacier(int R);
void free_glacier(glacier_t **g, int R);
int on_glacier(int x, int y, int R);
void fight(int a, int b, glacier_t **g, elf_t *e, FILE *u);
void change_gloves(int id, glacier_t **g, elf_t *e, int x, int y);
void move(glacier_t **g, elf_t *e, char m, int id, int R, FILE *u);
void snowstorm(int k, elf_t *e, int id, FILE *u, glacier_t **g);
void print_scoreboard(elf_t *e, int P, FILE *u);
glacier_t **meltdown(glacier_t **g, elf_t *e, int R, int P, int stam, FILE *u);
void win(glacier_t **g, int R, int v[2]);
void move_direction(glacier_t **g, elf_t *e, int mvx, int mvy, int id, int R,
    FILE *u);

#endif // _FUNCTIONS_H_
