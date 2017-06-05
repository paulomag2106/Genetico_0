#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <GLFW/glfw3.h>

#define MAX_FOOD 100
#define MAX_CELLS 50

#define frand(a) (((float)rand()/(float)(RAND_MAX)) * a)
#define toDegrees(x) (x * (180 / M_PI))

// global variables
int width, height;
int size;
double x, y;

// structs

typedef struct {
	float x, y;
}v2;

typedef struct {
	float x, y, z;
} v3;

enum e_terrain_type {
	WATER,
	SAND,
	EARTH,
	FERTILE,
	SWAMP,
	ROCK
};

typedef struct {
	v3 pos;
	enum e_terrain_type type;
} node;

typedef struct {
	v3 **mesh;

	node *nodes;
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

enum e_tags{
	MALE = 0x1,
	FEMALE = 0x2,
	HERBIVORE = 0x01,
	OMNIVORE = 0x02,
	CARNIVORE = 0x03,
};

typedef struct t_cell {

	// IDENTIFICATION
	// letter(type of feeding, 1 digit)
	// number(id in current generation, 6 digits)
	// generation(number, 4 digits)
	char id[11];

	// current attributes
	unsigned char str, intl, con, agl;
	// attributes in peek development
	unsigned char maxStr, maxIntl, maxCon, maxAgl;

	// qualitative characteristics
	enum e_tags tags;

	// destination
	v2 target;

	// body polygon
	quad body;

	// variable characteristics
	int feeding;
	int hydration;
	float hp;
	float age;
	float radius;
	float speed;

	// kinship
	struct t_cell *parents;
	struct t_cell *children;

	// linked list pointers
	struct t_cell *prev;
	struct t_cell *next;

	// check if it's alive or dead
	bool isAlive;
}cell;

// functions

v2 createtarget();
void changeTarget(cell* cel);
cell createCell();
int getMin(int val1, int val2);
int getMax(int val1, int val2);
int intClamp(int min, int val, int max);
void geraMutacao(cell *pai, cell *filho, int numChanges, int variability);
void drawQuad(quad *body);
void drawCircle(cell *cel);
void moveQuad(quad *body, float x, float y);
void goToTarget(cell* cel, float deltaTime);
void initiateFood(quad *foodArray);
bool circleDetection(cell* cel, quad food);
void generateMap(terrain *map, int matrixX, int matrixY);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
