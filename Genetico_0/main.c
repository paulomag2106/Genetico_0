#include "functions.h"

int main() {
	float theta = 0;

	size = 256;
	float deltaTime, currentFrame = 0, lastFrame = 0;

	quad *foodArray = malloc(sizeof(quad) * MAX_FOOD);
	initiateFood(foodArray);

	srand((unsigned int)time(NULL));

	// create map
	terrain map;
	generateMap(&map, 30, 30);

	cell *initialCells = (cell *)malloc(sizeof(cell) * MAX_CELLS);
	for (int i = 0; i < MAX_CELLS; i++)
		initialCells[i] = createCell();
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
		cls();
		printf("%.2f FPS\n",1.f/deltaTime);
		printf("%lf\n", yScroll);

		float ratio;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glfwSetScrollCallback(window, scroll_callback);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// glOrtho(-width/2, width/2, -height/2, height/2, -1, 1);
		glFrustum(-width/2, width/2, -height/2, height/2, 5, 10000);

		glTranslatef(0, 0, -11+yScroll/20);
		// glRotatef(-0.2, 1, 0, 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// draw map
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		int numTris = 30 * 30 * 2;
		for(int i = 0;i < numTris;i++) {

			color triColor = map.nodes[i].type == WATER ? (color){0, 0, 0.5} :
											 map.nodes[i].type == SAND ? (color){0.6, 0.6, 0} :
											 map.nodes[i].type == EARTH ? (color){0.2, 0.2, 0} :
											 map.nodes[i].type == FERTILE ? (color){0, 0.5, 0} :
											 map.nodes[i].type == SWAMP ? (color){0, 0.2, 0.2} :
											 map.nodes[i].type == ROCK ? (color){0.4, 0.4, 0.4} : (color){0, 0, 0};

			glBegin(GL_TRIANGLE_STRIP);

			glColor3f(triColor.r, triColor.g, triColor.b);
			glVertex3f(map.mesh[i][0].x, map.mesh[i][0].y, map.mesh[i][0].z);
			glVertex3f(map.mesh[i][1].x, map.mesh[i][1].y, map.mesh[i][1].z);
			glVertex3f(map.mesh[i][2].x, map.mesh[i][2].y, map.mesh[i][2].z);
			glEnd();

			// glPointSize(5);
			// glBegin(GL_POINTS);
			// glColor3f(1, 0, 0);
			// glVertex3f(map.nodes[i].pos.x, map.nodes[i].pos.y, map.nodes[i].pos.z);
			// glEnd();
		}

		// draw food
		for (int i = 0; i < MAX_FOOD; i++) {
			drawQuad(&foodArray[i]);
		}

		for (int i = 0; i < MAX_CELLS; i++)
		{
			goToTarget(&initialCells[i], deltaTime / 2);
			drawQuad(&initialCells[i].body);
			drawCircle(&initialCells[i]);
		}

		// for (int celNum = 0; celNum < MAX_CELLS; celNum++) {
		// 	for (int foodNum = 0; foodNum < MAX_FOOD; foodNum++) {
		// 		if (circleDetection(&initialCells[celNum], foodArray[foodNum])) {
		// 			initialCells[celNum].body.size = (v2) {20, 20};
		// 			initialCells[celNum].body.col = (color) { 0, 0, 1 };
		// 			drawQuad(&initialCells[celNum].body);
		// 		}
		// 		else {
		// 			initialCells[celNum].body.size = (v2) { 10, 10};
		// 			initialCells[celNum].body.col = (color) { 1, 0, 1 };
		// 			drawQuad(&initialCells[celNum].body);
		// 		}
		// 	}
		// }

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	free(initialCells);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
