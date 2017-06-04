#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <GLFW/glfw3.h>

#define MAX_FOOD 50
#define MAX_CELLS 20

#define frand(a) (((float)rand()/(float)(RAND_MAX)) * a)
#define toDegrees(x) (x * (180 / M_PI))

// structs

typedef struct {
	float x, y;
}v2;

typedef struct {
	float x, y, z;
} v3;

typedef struct {
	v3 *mesh;

	v3 *nodes;
	v3 *edges;

} terrain;

typedef struct {
	float r, g, b;
}color;

typedef struct {
	v2 pos;
	v2 size;
	color col;
} quad;

typedef struct t_cell {
	unsigned char str, intl, con, agl, HP;

	v2 target;
	quad body;
	float radius;
	float speed;

	// kinship
	struct t_cell *parents;
	struct t_cell *children;

	// linked list pointers
	struct t_cell *prev;
	struct t_cell *next;
}cell;

typedef union {
	int integer;
	float decimal;
}bitwiseCast;

// functions

v2 createtarget();
void changeTarget(cell* cel);
cell *createCell();
int getMin(int val1, int val2);
int getMax(int val1, int val2);
int intClamp(int min, int val, int max);
bool contains(int* arr, int val);
void geraMutacao(cell *pai, cell *filho, int numChanges, int variability);
void drawQuad(quad *body);
void drawCircle(cell *cel);
void moveQuad(quad *body, float x, float y);
void goToTarget(cell* cel, float deltaTime);
void initiateFood(quad *foodArray);
bool circleDetection(cell* cel, quad food);
void generateMap(terrain *map, int matrixX, int matrixY);
