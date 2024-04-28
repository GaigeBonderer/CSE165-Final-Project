#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <list>

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

//=================================================================================================
// Bullet Class (for user to shoot)
//=================================================================================================
class Bullet {
private:
    float x, y; // Position of the bullet
    float speed; // Speed of the bullet
    float width, height; // Dimensions of the bullet

public:
    Bullet(float initX, float initY, float initSpeed = 0.1f)
        : x(initX), y(initY), speed(initSpeed), width(0.01f), height(0.1f) {}

    void move() {
        y += speed; // Move bullet upward
    }

    void draw() const {
        glColor3f(1.0f, 0.0f, 0.0f); // Set color to red
        glBegin(GL_QUADS); // Draw rectangle (thin red)
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    float getY() const {
        return y;
    }
};


//=================================================================================================
// Abstract Enemy Base Class
//=================================================================================================

class Enemy {
public:
    virtual void draw() const = 0; // Pure virtual function for drawing enemies
    virtual ~Enemy() {} // Virtual destructor to ensure proper destruction of derived classes
};

//=================================================================================================
// Derived Basic Enemy Class
//=================================================================================================

class BasicEnemy : public Enemy {
private:
    float x, y; // Enemy position
    float size; // Size of the enemy

public:
    BasicEnemy(float initX, float initY, float initSize = 0.1f)
        : x(initX), y(initY), size(initSize) {}

    void draw() const override {
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y);
        glVertex2f(x + size, y);
        glVertex2f(x + size / 2, y + size);
        glEnd();
    }
};

//=================================================================================================
// Derived Boss Enemy Class
//=================================================================================================

class AdvancedEnemy : public Enemy {
private:
    float x, y; // Enemy position
    float size; // Size of the enemy

public:
    AdvancedEnemy(float initX, float initY, float initSize = 0.15f)
        : x(initX), y(initY), size(initSize) {}

    void draw() const override {
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y);
        glVertex2f(x + size, y);
        glVertex2f(x + size / 2, y + size);
        glEnd();
    }
};

//=================================================================================================
// Player Class
//=================================================================================================

class Player {
private:
    float x, y; // Player position
    float size; // Size of the triangle representing the player
    float speed; // Speed of movement
    std::list<Bullet> bullets; // List of bullets

public:
    // Constructor to initialize the player with default values
    Player(float initX = 0.0f, float initY = 0.0f, float initSize = 0.1f, float initSpeed = 0.05f)
        : x(initX), y(initY), size(initSize), speed(initSpeed) {}

    // Method to move the player based on keyboard input
    void move(char direction) {
        switch (direction) {
            //case 'w':
                //y += speed; // Move up // Not need since limited to horizontal motion
                //break;
        case 'a':
            x -= speed; // Move left
            break;
            //case 's':
                //y -= speed; // Move down // Not need since limited to horizontal motion
                //break;
        case 'd':
            x += speed; // Move right
            break;
        }
    }
    // Method to shoot a bullet
    void shoot() {
        bullets.push_back(Bullet(x + size / 2, y + size)); // Bullet from top of triangle
    }

    // Method to update bullets
    void updateBullets() {
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            it->move(); // Move the bullet upward
            if (it->getY() > 1.0f) { // Remove bullet when it goes out of bounds
                it = bullets.erase(it);
            }
            else {
                ++it;
            }
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
        // Draw bullets
        for (const auto& bullet : bullets) {
            bullet.draw();
        }
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
// Dot Class
//=================================================================================================

class Dot {
private:
    float x, y; // Coordinates of the dot
    float size; // Size of the dot

public:
    // Constructor to initialize the dot with position and size
    Dot(float initX, float initY, float initSize = 5.0f)
        : x(initX), y(initY), size(initSize) {}

    // Method to draw the dot
    void draw() const {
        glPointSize(size); // Set point size
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glBegin(GL_POINTS); // Start drawing points
        glVertex2f(x, y); // Draw the dot at specified coordinates
        glEnd(); // End drawing
    }

    // Getters for dot coordinates
    std::pair<float, float> getPosition() const {
        return { x, y };
    }

    // Setters for dot coordinates
    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }
};

//=================================================================================================
// DotManager Class
//=================================================================================================

class DotManager {
private:
    std::vector<Dot> dots; // Collection of dots

public:
    // Method to add a new dot to the collection
    void addDot(float x, float y, float size = 5.0f) {
        dots.emplace_back(x, y, size);
    }

    // Method to draw all dots
    void drawDots() const {
        for (const auto& dot : dots) {
            dot.draw(); // Draw each dot
        }
    }



};

//=================================================================================================
// GLOBAL VARIABLES
//=================================================================================================

//calls the Player
Player player(0.0f, -0.9f, 0.1f, 0.05f); // Player object with initial position and size
DotManager dotManager; // Dot manager to manage a collection of dots

//std::vector<BasicEnemy> enemies; // <- errors with this

BasicEnemy benem(0.0f, -0.9f, 0.1f);

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
        if (key == 'w') { // Shoot a bullet on 'w' key press
            player.shoot();
        }
        else {
            player.move(key); // Move player based on input
        }
    }

    glutPostRedisplay(); // Redraw after key press
}


//=================================================================================================
// RENDERING
//=================================================================================================

void display_func(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws the white dots
    dotManager.drawDots(); // Draw all dots in the dot manager

    //initialize the dots
    dotManager.addDot(-0.7f, 0.7f);
    dotManager.addDot(0.5f, -0.6f);
    dotManager.addDot(0.3f, 0.9f);
    dotManager.addDot(-0.4f, -0.3f);
    dotManager.addDot(0.1f, 0.1f);
    dotManager.addDot(-0.3f, 0.4f);
    dotManager.addDot(0.6f, 0.0f);
    dotManager.addDot(-0.9f, -0.5f);
    dotManager.addDot(0.9f, 0.8f);

    benem.draw();

    player.updateBullets(); // Move bullets

    // Draws the player
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