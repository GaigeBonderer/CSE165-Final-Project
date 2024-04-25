#include <GL/freeglut.h>
#include <iostream>

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

float PlayerX = 0.25f; // Establishes intitial value for PlayerX
float PlayerY = 0.25f; // Establishes intitial value for PlayerY

void keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':

		//PlayerY += 0.05f; Removed so Player cant move up
		break;
	case 'a':
		PlayerX -= 0.05f; // Moves player left
		break;
	case 's':
		//PlayerY -= 0.05f; Removed so Player cant move down
		break;
	case 'd':
		PlayerX += 0.05f; // Moves player right
		break;
		// Exit on escape key press
	case '\x1B':
		exit(EXIT_SUCCESS);
		break;
	}

	glutPostRedisplay();
}

void key_released(unsigned char key, int x, int y)
{
}

void key_special_pressed(int key, int x, int y)
{
}

void key_special_released(int key, int x, int y)
{
}

void mouse_func(int button, int state, int x, int y)
{
}

void passive_motion_func(int x, int y)
{
}

void active_motion_func(int x, int y)
{
}

//=================================================================================================
// RENDERING  (creating and displaying the triangle)
//=================================================================================================

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// glBegin(GL_LINES);
	// 	glColor3f(1.0f, 0.0f, 0.0f);
	// 	glVertex2f(-0.5f, 0.0f);
	// 	glColor3f(0.0f, 1.0f, 0.0f);
	// 	glVertex2f(0.5f, 0.0f);
	// glEnd();

	// glColor3f(0.0f, 0.0f, 1.0f);
	// 	glBegin(GL_LINES);
	// 	glVertex2f(0.0f, -0.5f);
	// 	glVertex2f(0.0f, 0.5f);
	// glEnd();

	glColor3f(1.0f, 1.0f, 1.0f); //sets the color to white
	glBegin(GL_TRIANGLES); //begin drawing the triangle
	glVertex2f(PlayerX, PlayerY); //defines 1st vertex
	glVertex2f(PlayerX + 0.1f, PlayerY); //defines 2nd vertex
	glVertex2f(PlayerX + 0.05f, PlayerY + 0.1f); //defines 3rd vertex (temp)
	glEnd(); //ends drawing

	glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n\n";

	// Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::cout << "Finished initializing...\n\n";

	PlayerX = -0.075f; // Edits Players initial starting position (horizontal)
	PlayerY = -0.9f; // Edits Players initial starting position (Vertical)
}

//=================================================================================================
// MAIN
//=================================================================================================

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("Basic OpenGL Example");

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(key_released);
	glutSpecialFunc(key_special_pressed);
	glutSpecialUpFunc(key_special_released);
	glutMouseFunc(mouse_func);
	glutMotionFunc(active_motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
