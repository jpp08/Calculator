#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>

#ifndef __EXPR_H_
#define __EXPR_H_

#define B1 1 // +
#define B2 2 // -
#define B3 3 // *
#define B4 4 // /
#define B5 5 // min
#define B6 6 // max
#define U1 7 // sqrt
#define U2 8 // exp 
#define U3 9 // ln
#define U4 10 // abs
#define U5 11 // floor
#define U6 12 // ceil
#define A1 13 // autres
#define NBCLE 12

typedef struct { char *cle; int val; } t_symcle;

typedef struct abin {
	/* signi => 0 = opérande, 1 = opérateur */
	int descri;
	double val;
	struct abin* fd;
	struct abin* fg;
} *expr;

/*Nom des fonctions */
int strtocle(char* cle);
void free_expr(expr e);
void free_mem(char** str);
expr creation(char* str);
double calcul_expr(expr e,int* err);



#endif