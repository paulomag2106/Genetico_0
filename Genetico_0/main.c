#include "functions.h"

int main() {

	float xVal = 0, yVal = 0;
	float deltaTime, currentFrame = 0, lastFrame = 0;

	quad *foodArray = malloc(sizeof(quad) * MAX_FOOD);
	initiateFood(foodArray);

	srand((unsigned int)time(NULL));
	bitwiseCast mutation;
	mutation.decimal = 0.75f;

	cell** celulas = (cell**)malloc(sizeof(cell) * MAX_CELLS);
	for (int i = 0; i < MAX_CELLS; i++)
		celulas[i] = createCell();
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

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-width/2, width/2, -height/2, height/2, 0, 1);

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
		}

		for (int celNum = 0; celNum < MAX_CELLS; celNum++) {
			for (int foodNum = 0; foodNum < MAX_FOOD; foodNum++) {
				if (circleDetection(celulas[celNum], foodArray[foodNum])) {
					celulas[celNum]->body.size = (v2) {20, 20};
					celulas[celNum]->body.col = (color) { 0, 0, 1 };
					drawQuad(&(celulas[celNum])->body);
				}
				else {
					celulas[celNum]->body.size = (v2) { 10, 10};
					celulas[celNum]->body.col = (color) { 1, 0, 1 };
					drawQuad(&(celulas[celNum])->body);
				}
			}
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
