#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <memory>

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
    Bullet(float initX, float initY, float initSpeed = 0.005f)
        : x(initX), y(initY), speed(initSpeed), width(0.01f), height(0.1f) {}

    void move() {
        y += speed; // Move bullet upward
    }

    void draw() const {
        glColor3f(1.0f, 0.0f, 0.0f); // Set color to red
        glBegin(GL_QUADS); // Draw rectangle
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    float getY() const {
        return y;
    }

    std::pair<float, float> getPosition() const {
        return { x, y };
    }

};

//=================================================================================================
// Enemy Bullet Class
//=================================================================================================

class EnemyBullet {
private:
    float x, y; // Position of the bullet
    float speed; // Speed of the bullet
    float width, height; // Dimensions of the bullet

public:
    EnemyBullet(float initX, float initY, float initSpeed = -0.001f)
        : x(initX), y(initY), speed(initSpeed), width(0.01f), height(0.1f) {}

    void move() {
        y += speed; // Move bullet downward
    }

    void draw() const {
        glColor3f(0.0f, 1.0f, 0.0f); // Set color to green
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    float getY() const {
        return y;
    }

    std::pair<float, float> getPosition() const {
        return { x, y };
    }
};


//=================================================================================================
// Abstract Enemy Base Class
//=================================================================================================

class Enemy {
public:
    virtual void draw() const = 0; // Pure virtual function for drawing enemies
    virtual ~Enemy() {} // Virtual destructor to ensure proper destruction of derived classes
    virtual std::pair<float, float> getPosition() const = 0;
    
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
        //std::cout << "Drawn called in BasicEnemy" << std::endl;
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y + size); // Write the triangle vertices in reverse to flip it
        glVertex2f(x + size, y + size);
        glVertex2f(x + size / 2, y);
        glEnd();
    }

    // Method to update the position of the enemy
    void updatePosition(float deltaX) {
        x += deltaX; // Update x-coordinate
    }

    // Implementation of getPosition for BasicEnemy
    std::pair<float, float> getPosition() const override {
        return { x, y };
    }
};

//=================================================================================================
// Derived FrontLine Enemy Class
//=================================================================================================

class FrontLine : public Enemy {
private:
    float x, y; // Enemy position
    float size; // Size of the enemy

public:
    FrontLine(float initX, float initY, float initSize = 0.1f)
        : x(initX), y(initY), size(initSize) {}

    void draw() const override {
        //std::cout << "Drawn called in BasicEnemy" << std::endl;
        glColor3f(0.0f, 0.0f, 1.0f); // Red
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y + size); // Write the triangle vertices in reverse to flip it
        glVertex2f(x + size, y + size);
        glVertex2f(x + size / 2, y);
        glEnd();
    }

    // Method to update the position of the enemy
    void updatePosition(float deltaX) {
        x += deltaX; // Update x-coordinate
    }

    // Implementation of getPosition for BasicEnemy
    std::pair<float, float> getPosition() const override {
        return { x, y };
    }
};

//=================================================================================================
// Derived Boss Enemy Class
//=================================================================================================

// to be done

//=================================================================================================
// Enemy Manager Class (for upcasting)
//=================================================================================================

class EnemyManager {
private:
    std::vector < std::shared_ptr<Enemy>> enemies; // Vector to store shared pointers to Enemy objects
    std::list<EnemyBullet> enemyBullets; // List to store enemy bullets globally
public:
    // Method to add a new enemy to the manager
    template<typename T>
    void addEnemy(float x, float y, float size) {
        enemies.push_back(std::make_shared<T>(x, y, size));
    }

    // Method to draw all enemies
    void drawEnemies() const {
        for (const auto& enemy : enemies) {
            enemy->draw(); // Draw each enemy using polymorphism
            //std::cout << "Drawn called in EnemyManger" << std::endl;
        }
    }

    // Method to update enemy positions
    void updateEnemyPositions(float deltaX) {
        for (const auto& enemy : enemies) {
            auto basicEnemy = std::dynamic_pointer_cast<BasicEnemy>(enemy);
            if (basicEnemy) {
                basicEnemy->updatePosition(deltaX);
            }
        }
    }

    std::vector<std::shared_ptr<Enemy>>& getEnemies() {
        return enemies;
    }
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const {
        return enemies;
    }


    void eraseEnemy(std::shared_ptr<Enemy> enemy) {
        enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    }

    void shootFromEnemies() {
        for (const auto& enemy : enemies) {
            auto pos = enemy->getPosition();
            enemyBullets.push_back(EnemyBullet(pos.first, pos.second - 0.1)); // Create bullet below the enemy
        }
    }

    // Method to update enemy bullets
    void updateEnemyBullets() {
        for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
            it->move();
            if (it->getY() < -1.0) {
                it = enemyBullets.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    // Method to draw enemy bullets
    void drawEnemyBullets() const {
        for (const auto& bullet : enemyBullets) {
            bullet.draw();
        }
    }

    std::list<EnemyBullet>& getMutableEnemyBullets() {
        return enemyBullets;
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

    std::list<Bullet>& getBullets() {
        return bullets;
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
Player player(-0.05f, -0.9f, 0.1f, 0.05f); // Player object with initial position and size
DotManager dotManager; // Dot manager to manage a collection of dots

EnemyManager enemyManager;

int frameCounter = 0; // Used for controlling seed of enemy autnomous movement

bool FullRight = false;

bool Center = true;

bool FullLeft = false;

int AbsPos = 0;

//=================================================================================================
// Function for detecting collisions
//=================================================================================================

void checkCollisions() {
    auto& bullets = player.getBullets();
    auto& enemies = enemyManager.getEnemies();

    // Iterate over bullets
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
        bool bulletErased = false;
        // Iterate over enemies
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end() && !bulletErased;) {
            // Calculate distance between bullet and enemy
            float distX = bulletIt->getPosition().first - (*enemyIt)->getPosition().first;
            float distY = bulletIt->getPosition().second - (*enemyIt)->getPosition().second;
            float distance = std::sqrt(distX * distX + distY * distY);

            // If distance is less than a threshold (considered overlap), remove both bullet and enemy
            if (distance < 0.1f) {
                // Erase bullet
                bulletIt = bullets.erase(bulletIt);
                bulletErased = true;

                // Erase enemy and update iterator
                enemyIt = enemies.erase(enemyIt);
            }
            else {
                ++enemyIt; // Move to the next enemy
            }
        }
        if (!bulletErased) {
            ++bulletIt; // Move to the next bullet only if it wasn't erased
        }
    }
}

void checkPlayerCollision() {
    auto playerPos = player.getPosition();
    auto& enemyBullets = enemyManager.getMutableEnemyBullets();

    for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ++it) {
        float distX = it->getPosition().first - playerPos.first;
        float distY = it->getPosition().second - playerPos.second;

        // Here is where you put the line with the static cast
        float distance = static_cast<float>(std::sqrt(distX * distX + distY * distY));

        if (distance < 0.1f) { // Simple collision detection
            exit(EXIT_SUCCESS); // End the game if the player is hit
        }
    }
}

//=================================================================================================
// CALLBACKS
//=================================================================================================

void idle_func() {
    // Increment the frame counter
    ++frameCounter;

    if (frameCounter % 1000 == 0) {
        enemyManager.shootFromEnemies();
    }

    // Update the positions of bullets shot by enemies
    auto& enemyBullets = enemyManager.getMutableEnemyBullets();
    for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
        it->move();
        if (it->getY() < -1.0f) {
            it = enemyBullets.erase(it); // Remove bullets that go below the screen
        }
        else {
            ++it;
        }
    }

    // Check for collisions between enemy bullets and the player
    checkPlayerCollision();
    checkCollisions();

    constexpr int movementInterval = 1000; // Update enemy positions every 1000 frames
    if (frameCounter >= movementInterval) {
        frameCounter = 0;
        float deltaX = 0.2f; // Amount enemies move

        if (Center) {
            if (FullRight) {
                AbsPos -= 1;
                enemyManager.updateEnemyPositions(-deltaX);
            }
            else if (FullLeft) {
                AbsPos += 1;
                enemyManager.updateEnemyPositions(deltaX);
            }
            else {
                AbsPos += 1;
                enemyManager.updateEnemyPositions(deltaX); // initially moves enemies to the right
            }
        }

        if (AbsPos == 1) {
            FullRight = true;
            FullLeft = false;
        }
        if (AbsPos == -1) {
            FullLeft = true;
            FullRight = false;
        }
    }

    
    glutPostRedisplay(); // Redraw the scene
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

    player.updateBullets(); // Move bullets

    
    player.draw(); // Draws the player


    enemyManager.drawEnemies(); // Draws enemies

    
    enemyManager.drawEnemyBullets(); // Draw enemy bullets

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

    float sz = 0.1f;


    enemyManager.addEnemy<FrontLine>(-0.05f, 0.4f, sz);

    enemyManager.addEnemy<FrontLine>(-0.55f, 0.3f, sz);

    enemyManager.addEnemy<FrontLine>(0.45f, 0.3f, sz);

    enemyManager.addEnemy<BasicEnemy>(-0.05f, 0.7f, sz);

    enemyManager.addEnemy<BasicEnemy>(0.45f, 0.5f, sz);

    enemyManager.addEnemy<BasicEnemy>(-0.55f, 0.5f, sz);



    init();

    glutMainLoop(); // Main event loop

    return EXIT_SUCCESS;
}