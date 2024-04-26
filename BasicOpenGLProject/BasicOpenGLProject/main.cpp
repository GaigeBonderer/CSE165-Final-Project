#include <GL/freeglut.h>
#include <iostream>
#include <vector>


//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------


//=================================================================================================
// Player Class
//=================================================================================================

class Player {
private:
    float x, y; // Player position
    float size; // Size of the triangle representing the player
    float speed; // Speed of movement

public:
    // Constructor to initialize the player with default values
    Player(float initX = 0.0f, float initY = 0.0f, float initSize = 0.1f, float initSpeed = 0.05f)
        : x(initX), y(initY), size(initSize), speed(initSpeed) {}

    // Method to move the player based on keyboard input
    void move(char direction) {
        switch (direction) {
        case 'w':
            y += speed; // Move up
            break;
        case 'a':
            x -= speed; // Move left
            break;
        case 's':
            y -= speed; // Move down
            break;
        case 'd':
            x += speed; // Move right
            break;
        }
    }

    // Method to draw the player as a triangle
    void draw() const {
        glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
        glBegin(GL_TRIANGLES); // Start drawing the triangle
        glVertex2f(x, y); // First vertex
        glVertex2f(x + size, y); // Second vertex
        glVertex2f(x + size / 2, y + size); // Third vertex
        glEnd(); // End drawing
    }

    // Getter for the player's position
    std::pair<float, float> getPosition() const {
        return { x, y };
    }

    // Setter for the player's position
    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }

    // Setter for the player's speed
    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    // Getter for the player's speed
    float getSpeed() const {
        return speed;
    }
};

//=================================================================================================
// GLOBAL VARIABLES
//=================================================================================================

//calls the Player
Player player(0.0f, -0.9f, 0.1f, 0.05f); // Player object with initial position and size

//=================================================================================================
// CALLBACKS
//=================================================================================================

void idle_func() {
    glutPostRedisplay(); // Ensures continuous display updates
}

void reshape_func(int width, int height) {
    glViewport(0, 0, width, height);
    glutPostRedisplay(); // Redraw after reshaping
}

void keyboard_func(unsigned char key, int x, int y) {
    if (key == '\x1B') {
        exit(EXIT_SUCCESS); // Exit on ESC key press
    }
    else {
        player.move(key); // Move player based on input
    }

    glutPostRedisplay(); // Redraw after key press
}

//=================================================================================================
// RENDERING
//=================================================================================================

void display_func(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw some white dots
    std::vector<std::pair<float, float>> dotCoords = {
        { -0.7f, 0.7f }, { 0.5f, -0.6f }, { 0.4f, 0.6f }, { -0.4f, -0.3f }, // using (x,y) coord
        { 0.1f, 0.4f }, { -0.3f, 0.6f }, { 0.6f, -0.4f }, { -0.5f, -0.5f }
    };

    glPointSize(5.0f); //point size, diameter in pizels
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the dots

    glBegin(GL_POINTS);
    for (const auto& coord : dotCoords) {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();

    // Draw the player
    player.draw(); // Draw the player triangle

    glutSwapBuffers(); // Swap buffers to display the result
}

//=================================================================================================
// INIT
//=================================================================================================

void init(void) {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Background color
}

//=================================================================================================
// MAIN
//=================================================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutCreateWindow("OpenGL Example with Player Class");

    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    glutReshapeFunc(reshape_func);
    glutKeyboardFunc(keyboard_func);

    init();

    glutMainLoop(); // Main event loop

    return EXIT_SUCCESS;
}
