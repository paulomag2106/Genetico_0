#include "functions.h"


v2 createtarget()
{
	return (v2) { (frand(35*30) - (35*15)), (frand(35*30) - (35*15)) };
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

cell createCell() {
	cell p;

	// target developed attributes
	p.maxStr = (rand() % 10) + 1;
	p.maxIntl = (rand() % 10) + 1;
	p.maxCon = (rand() % 10) + 1;
	p.maxAgl = (rand() % 10) + 1;

	// starting attributes
	p.str =1;
	p.intl = 1;
	p.con = 1;
	p.agl = 1;

	p.hp = p.con * 10;
	p.radius = p.intl * 10;
	p.speed = p.agl * 10;

	p.body.pos = createtarget();
	p.body.size = (v2) { 10, 10 };
	p.body.col = (color) { 1, 0, 1 };

	p.target = createtarget();

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

void geraMutacao(cell *pai, cell *filho, int numChanges, int variability) {

	int amount = (rand() % (variability * 2)) - variability;

	filho->str = intClamp(1, pai->str + amount, 10);

	amount = (rand() % (variability * 2)) - variability;

	filho->intl = intClamp(1, pai->intl + amount, 10);

	amount = (rand() % (variability * 2)) - variability;

	filho->con = intClamp(1, pai->con + amount, 10);

	filho->hp = filho->con * 10;

	amount = (rand() % (variability * 2)) - variability;

	filho->agl = intClamp(1, pai->agl + amount, 10);

	filho->radius = filho->intl * 10;
	filho->speed = filho->agl * 10;
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

	for (float i = 0; i <= 2 * M_PI; i += M_PI / 6) {
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
	float yDist = (cel->target.y - cel->body.pos.y);
	float xDist = (cel->target.x - cel->body.pos.x);

	if(xDist < 1 && yDist < 1){
		changeTarget(cel);
	} else {
		float theta = atan2(yDist, xDist);

			x = cosf(theta);
			y = sinf(theta);

	  moveQuad(&(cel->body), deltaTime*x*cel->speed, deltaTime*y*cel->speed);
	}
}

void initiateFood(quad *foodArray) {

	for (int i = 0; i < MAX_FOOD; i++) {

		//srand((unsigned int)time(NULL));
		foodArray[i].col = (color) { 0, 1, 0 };
		foodArray[i].size = (v2) { 10, 10 };
		foodArray[i].pos = (v2) { (frand(35*30) - (35*15)), (frand(35*30) - (35*15))};
		//printf("%f, %f\n", foodArray[i].pos.x, foodArray[i].pos.y);
	}
}

bool circleDetection(cell* cel, quad food) {

	double difX = cel->body.pos.x - food.pos.x;
	double difY = cel->body.pos.y - food.pos.y;

	double dist = sqrt(pow(difX, 2) + pow(difY, 2));

	return dist <= cel->radius;
}

void generateMap(terrain *map, int matrixX, int matrixY) {

	int numTris = 2*matrixX * matrixY;

	map->mesh = malloc(sizeof(v3 *) * numTris);
	// printf("%d\n", numTris);
	for(int i = 0; i < numTris;i++)
		map->mesh[i] = malloc(sizeof(v3) * 3);
	map->nodes = malloc(sizeof(node) * numTris);
	size = 100;
	v3 offset = {-size*(matrixX)/2, -size*(matrixY/2), 0};
	// v3 offset = {0, 0, 0};

	for(int i = 0; i < matrixY;i++) {
		for(int j = 0; j < matrixX;j++) {
			int index = i*matrixX + j;
			map->mesh[index][0] = (v3){offset.x+(j*size), offset.y+(i*size), 0};
			map->mesh[index][1] = (v3){offset.x+(j*size), offset.y+((i+1)*size), 0};
			map->mesh[index][2] = (v3){offset.x+((j+1)*size), offset.y+(i*size), 0};

			index = 900 + i*matrixX + j;
			map->mesh[index][0] = (v3){offset.x+((j)*size), offset.y+((i+1)*size), 0};
			map->mesh[index][1] = (v3){offset.x+((j+1)*size), offset.y+((i+1)*size), 0};
			map->mesh[index][2] = (v3){offset.x+((j+1)*size), offset.y+(i*size), 0};
		}
	}

	for(int i = 0;i < numTris;i++) {

		map->nodes[i].pos = (v3){(map->mesh[i][0].x+map->mesh[i][1].x+map->mesh[i][2].x)/3,
														 (map->mesh[i][0].y+map->mesh[i][1].y+map->mesh[i][2].y)/3,
													 	 (map->mesh[i][0].z+map->mesh[i][1].z+map->mesh[i][2].z)/3};
		map->nodes[i].type = (enum e_terrain_type)(rand() % 6);
	}

}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	y += yOffset;
	// printf("%lf, %lf\n", xOffset, yOffset);
}
