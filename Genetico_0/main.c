#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <GLFW/glfw3.h>

#define PI 3.14159265358979323846
#define MAX_FOOD 2000
#define MAX_CELLS 2000

#define frand(a) (((float)rand()/(float)(RAND_MAX)) * a)
#define toDegrees(x) (x * (180 / PI))

typedef struct {
	float x, y;
}v2;

typedef struct {
	float r, g, b;
}color;

typedef struct {
	v2 pos;
	v2 size;
	color col;
} quad;

typedef struct {
	unsigned __int8 str,
		intl,
		stm,
		agl,

		HP;

	v2 target;
	quad body;
	float radius;
	float speed;
}cell;

typedef union {
	int integer;
	float decimal;
}bitwiseCast;

v2 createtarget()
{
	return (v2) { frand(4.6) - 2.3, frand(2.8) - 1.4 };
}

void changeTarget(cell* cel)
{
	while (true)
	{
		v2 novaDistancia = createtarget();
		float distX = cel->target.x - novaDistancia.x;
		float distY = cel->target.y - novaDistancia.y;
		double distT = ((double)(powf(distX, 2) + powf(distY, 2)));
		double dist = sqrt(distT);

		if (round(dist) >= 1)
		{
			cel->target = novaDistancia;
			break;
		}
	}
}

cell *createCell() {
	cell *p = (cell *)malloc(sizeof(cell));
	p->str = (rand() % 10) + 1;
	p->intl = (rand() % 10) + 1;
	p->stm = (rand() % 10) + 1;
	p->agl = (rand() % 10) + 1;

	p->HP = p->stm * 10;
	p->radius = p->intl / 100.f;
	p->speed = p->agl / 10.f;

	p->body.pos = createtarget();//(v2) { 0, 0 };
	p->body.size = (v2) { 0.01f, 0.01f };
	p->body.col = (color) { 1, 0, 1 };

	p->target = createtarget();

	return p;
}

int getMin(int val1, int val2) {
	return val1 > val2 ? val2 : val1;
}

int getMax(int val1, int val2) {
	return val1 > val2 ? val1 : val2;
}

int intClamp(int min, int val, int max) {
	return val < min ? min : val > max ? max : val;
}

bool contains(int* arr, int val)
{
	if (arr == NULL)
		return false;

	for (int i = 0; i < sizeof(arr); i++)
		if (arr[i] == val)
			return true;

	return false;
}


void geraMutacao(cell *pai, cell *filho, int numChanges, int variability) {

	//int idxAtrNumber = 0;
	//int atrNumber[4] = { -1, -1, -1, -1 };
	//numChanges > 4 ? 4 : numChanges < 1 ? 1 : numChanges;
	//
	//for (int i = 0; i < numChanges; i++) {
	//	int switchVal = rand() % 4;
	//	if (contains(atrNumber, switchVal))
	//	{
	//		i--;
	//		continue;
	//	}
	//
	//	int amount = (rand() % (variability * 2)) - variability;
	//	if (amount == 0)
	//	{
	//		i--;
	//		continue;
	//	}
	//
	//	atrNumber[idxAtrNumber++] = switchVal;
	//
	//	switch (switchVal) {
	//	case 0:
	//		filho->str = pai->str + amount > 10 ?
	//			10 : pai->str + amount < 1 ?
	//			1 : pai->str + amount;
	//
	//		break;
	//
	//	case 1:
	//		filho->intl = pai->intl + amount > 10 ?
	//			10 : pai->intl + amount < 1 ?
	//			1 : pai->intl + amount;
	//
	//		break;
	//
	//	case 2:
	//		filho->stm = pai->stm + amount > 10 ?
	//			10 : pai->stm + amount < 1 ?
	//			1 : pai->stm + amount;
	//
	//		filho->HP = filho->stm * 10;
	//
	//		break;
	//
	//	case 3:
	//		filho->agl = pai->agl + amount > 10 ?
	//			10 : pai->agl + amount < 1 ?
	//			1 : pai->agl + amount;
	//
	//		break;
	//	}
	//}

	int amount = (rand() % (variability * 2)) - variability;

	filho->str = intClamp(1, pai->str + amount, 10);

	amount = (rand() % (variability * 2)) - variability;

	filho->intl = intClamp(1, pai->intl + amount, 10);

	amount = (rand() % (variability * 2)) - variability;

	filho->stm = intClamp(1, pai->stm + amount, 10);

	filho->HP = filho->stm * 10;

	amount = (rand() % (variability * 2)) - variability;
	
	filho->agl = intClamp(1, pai->agl + amount, 10);

	filho->radius = filho->intl / 100.f;
	filho->speed = filho->agl / 10.f;
	filho->body.pos = pai->body.pos;
}

void drawQuad(quad *body) {
	// let's define limits for cel quad

	float left = body->pos.x - body->size.x / 2,
		right = body->pos.x + body->size.x / 2,
		top = body->pos.y + body->size.y / 2,
		bot = body->pos.y - body->size.y / 2;

	// draw cel quad at position

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);

	glColor3f(body->col.r, body->col.g, body->col.b);
	glVertex2f(left, bot);
	glVertex2f(right, bot);
	glVertex2f(right, top);
	glVertex2f(left, top);

	glEnd();
}

void drawCircle(cell *cel) {

	float radius = cel->radius;


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_LINE_STRIP);

	glColor3f(1, 1, 1);

	for (float i = 0; i <= 2 * PI; i += PI / 5) {
		glVertex2f(cos(i)*radius + cel->body.pos.x, sin(i)*radius + cel->body.pos.y);
	}

	glEnd();
}

void moveQuad(quad *body, float x, float y) {
	body->pos.x += x;
	body->pos.y += y;
}

void goToTarget(cell* cel, float deltaTime)
{
	float x = 0, y = 0;
	float yDiv = (cel->target.y - cel->body.pos.y);
	float xDiv = (cel->target.x - cel->body.pos.x);
	float m = yDiv;

	if (roundf(xDiv) != 0) {
		m = yDiv / xDiv;
		float teta = toDegrees(atanf(m));
		x = cosf(teta);
		y = sinf(teta);
	}
	else
	{
		float mm = roundf(m);
		if (mm > 0)
			y = 1;
		else if (mm < 0)
			y = -1;
		else
			changeTarget(cel);
	}

	moveQuad(&(cel->body), deltaTime*x*cel->speed, deltaTime*y*cel->speed);
}

void initiateFood(quad *foodArray) {

	for (int i = 0; i < MAX_FOOD; i++) {

		//srand((unsigned int)time(NULL));
		foodArray[i].col = (color) { 0, 1, 0 };
		foodArray[i].size = (v2) { 0.01, 0.01 };
		foodArray[i].pos = (v2) { frand(3.6) - 1.8, frand(1.8) - 0.9 };
		//printf("%f, %f\n", foodArray[i].pos.x, foodArray[i].pos.y);
	}
}

bool circleDetection(cell* cel, quad food) {

	double difX = cel->body.pos.x - food.pos.x;
	double difY = cel->body.pos.y - food.pos.y;

	double dist = sqrt(pow(difX, 2) + pow(difY, 2));

	return dist <= cel->radius;
}

int main() {

	float xVal = 0, yVal = 0;
	float deltaTime, currentFrame = 0, lastFrame = 0;

	quad *foodArray = malloc(sizeof(quad) * MAX_FOOD);
	initiateFood(foodArray);

	srand((unsigned int)time(NULL));
	bitwiseCast mutation;
	mutation.decimal = 0.75f;

	cell** celulas = (cell*)malloc(sizeof(cell) * MAX_CELLS);
	for (int i = 0; i < MAX_CELLS; i++)
		celulas[i] = createCell();
	//cell *cel1 = createCell();
	//cell *cel2 = createCell();
	//
	//cell *cel3 = malloc(sizeof(cell));
	//
	//geraMutacao(cel2, cel1, 4, 2);
	//geraMutacao(cel1, cel2, 4, 2);

	/*FILE *fp = NULL;
	fopen_s(&fp, "berimbal.txt", "w");

	for (int i = 0; i < 10000; i++) {

		geraMutacao(cel3, cel3, 4, 2);

		fprintf(fp, "Generation %05d --> STR %02d, INT %02d, STM %02d, AGL %02d\n", i, cel3->str, cel3->intl, cel3->stm, cel3->agl);
	}

	fclose(fp);
*/
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Genetico 0", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// update delta time
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		printf("%f\n", deltaTime * 1000);

		float ratio;
		int width, heigth;

		glfwGetFramebufferSize(window, &width, &heigth);
		ratio = width / (float)heigth;
		glViewport(0, 0, width, heigth);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1 * ratio, 1 * ratio, -1, 1, 0, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// draw food
		for (int i = 0; i < MAX_FOOD; i++) {
			drawQuad(&foodArray[i]);
		}

		for (int i = 0; i < MAX_CELLS; i++)
		{
			goToTarget(celulas[i], deltaTime / 2);
			drawQuad(&(celulas[i])->body);
			drawCircle(celulas[i]);
			//geraMutacao(celulas[i], celulas[i], 4, 6);
		}

		//for (int celNum = 0; celNum < MAX_CELLS; celNum++) {
		//	for (int foodNum = 0; foodNum < MAX_FOOD; foodNum++) {
		//		if (circleDetection(celulas[celNum], foodArray[foodNum])) {
		//			celulas[celNum]->body.size = (v2) { 0.02, 0.02 };
		//			celulas[celNum]->body.col = (color) { 0, 0, 1 };
		//			drawQuad(&(celulas[celNum])->body);
		//		}
		//		else {
		//			celulas[celNum]->body.size = (v2) { 0.01, 0.01 };
		//			celulas[celNum]->body.col = (color) { 1, 0, 1 };
		//			drawQuad(&(celulas[celNum])->body);
		//		}
		//	}
		//}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}