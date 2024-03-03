#include <SFML/Graphics.hpp>
#include <iostream>
#include "helper.h"
#include "mandelbrot.h"
#include "julia.h"

extern int WIDTH;
extern int HEIGHT;

int constrainMouseX(int mouseX)
{
    if (mouseX < 0)
        mouseX = 0;
    if (mouseX > WIDTH)
        mouseX = WIDTH;
    return mouseX;
}

int constrainMouseY(int mouseY)
{
    if (mouseY < 0)
        mouseY = 0;
    if (mouseY > HEIGHT)
        mouseY = HEIGHT;
    return mouseY;
}

int main()
{

    ///////////////////////
    // SFML WINDOW SETUP //
    ///////////////////////

    // create ContextSettings object to control antialiasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fractal Visualisation", sf::Style::Default, settings);

    //////////////////////////
    // SFML VARIABLES SETUP //
    //////////////////////////

    // create the texture we will draw to
    sf::Texture fractalTexture;
    // make it blank to begin with
    fractalTexture.create(WIDTH, HEIGHT);
    // draw the texture 'smooth' - with antialiasing
    fractalTexture.setSmooth(true);

    // create the sprite we will draw the texture to
    sf::Sprite fractalSprite(fractalTexture);

    // create the main pixel array
    sf::Uint8 *currentPixels = new sf::Uint8[WIDTH * HEIGHT * 4]; // 4 bytes per pixel (RGBA)

    /////////////////////////////
    // ZOOMING VARIABLES SETUP //
    /////////////////////////////

    // these variables define our 'canvas', the whole window we are rendering
    float xCanvasStart = 0;
    float xCanvasEnd = WIDTH;
    float yCanvasStart = 0;
    float yCanvasEnd = HEIGHT;

    // these variables define the rectangular window we are zooming in on, on the graph of the fractal
    // MANDELBROT SET: make sure the lengths of the rectangle here are in a 16:9 ratio
    //-1, 1, -0.84375, 0.84375
    //-3.55.., 1.77.., -1.5, 1.5
    // -2.370370370370, 1.185185185185, -1, 1
    //( or don't )
    //-1, -0,6, 0, 0,4
    //-2, 1, -1, 1
    //JULIA SET: -1.5, 1.5, -1, 1
    long double xStart = -1.5;
    long double xEnd = 1.5;
    long double yStart = -1;
    long double yEnd = 1;

    // these variables define the rectangle drawn on screen with the mouse
    int inputX1;
    int inputY1;
    int inputX2;
    int inputY2;

    // create a lock for the mouse click function
    static bool clickLock = false;
    // and create a lock for when the zoom is changed
    static bool zoomChanged = false;

    static bool firstFrame = true;

    static bool automaticZooming = false;
    int frameCount = 0;
    int frameLimit = 1;
    int zoomPixels = 10;

    ///////////////////////
    // MAIN DRAWING LOOP //
    ///////////////////////

    // this is the loop - runs once per frame
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && !clickLock)
            {
                // mouse button clicked
                inputX1 = event.mouseButton.x;
                inputY1 = event.mouseButton.y;
                // lock this function so it can't be called again until the mouse button is released
                clickLock = true;
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                // mouse button released
                inputX2 = event.mouseButton.x;
                inputY2 = event.mouseButton.y;

                // std::cout << "inputX1: " << inputX1 << " inputY1: " << inputY1 << " inputX2: " << inputX2 << " inputY2: " << inputY2 << std::endl;

                // unlock the mouse click function and the zoom function
                clickLock = false;
                zoomChanged = true;
            }
        }

        // render background
        window.clear(sf::Color::Cyan);

        if (firstFrame)
        {
            currentPixels = julia(WIDTH, HEIGHT, xStart, xEnd, yStart, yEnd, currentPixels);
            // std::cout << "xStart: " << xStart << " xEnd: " << xEnd << " yStart: " << yStart << " yEnd: " << yEnd << std::endl;
            firstFrame = false;
        }

        if (zoomChanged && !firstFrame)
        {

            // this looks terrible very quickly if we don't force the aspect ratio to stay the same
            // base the new rectangle on x1,y1 and x2,y1 (only the position of the first click and the x-length of the line drawn matters)
            long double xDiff = inputX2 - inputX1;
            long double height = ((1.0 * HEIGHT) / (1.0 * WIDTH)) * xDiff;
            inputY2 = inputY1 + height;

            long double newX1 = map(xStart, xEnd, xCanvasStart, xCanvasEnd, inputX1);
            long double newY1 = map(yStart, yEnd, yCanvasStart, yCanvasEnd, inputY1);
            long double newX2 = map(xStart, xEnd, xCanvasStart, xCanvasEnd, inputX2);
            long double newY2 = map(yStart, yEnd, yCanvasStart, yCanvasEnd, inputY2);

            currentPixels = julia(WIDTH, HEIGHT, newX1, newX2, newY1, newY2, currentPixels);
            xStart = newX1;
            xEnd = newX2;
            yStart = newY1;
            yEnd = newY2;

            zoomChanged = false;
        }

        fractalTexture.update(currentPixels);

        // draw the sprite
        window.draw(fractalSprite);
        // render all calls to draw()
        window.display();

        // automatic slow zooming:
        // input x1,y1 very close to 0
        // input x2 very close to 1600 (y2 is ignored to maintain aspect ratio)
        // consider 'center point' of the rectangle, can change? center around mouse pos?
        // set zoomChanged to true - once per x frames to control speed
        if (automaticZooming)
        {
            if (frameCount++ == frameLimit)
            {

                // get position of the mouse
                int mouseX = sf::Mouse::getPosition(window).x;
                int mouseY = sf::Mouse::getPosition(window).y;

                // mouse position isn't constrained to the window size, so do that!
                mouseX = constrainMouseX(mouseX);
                mouseY = constrainMouseY(mouseY);

                // position the new frame around the mouse
                // consider which quarter of the screen the mouse is in
                if (mouseX < WIDTH / 2)
                {
                    // left half of the screen
                    // take off pixels on the right
                    inputX1 = 0;
                    inputX2 = WIDTH - zoomPixels;
                }
                else
                {
                    // take off pixels on the left
                    inputX1 = zoomPixels;
                    inputX2 = WIDTH;
                }

                if (mouseY < HEIGHT / 2)
                {
                    // top half of the screen
                    // take off pixels on the bottom
                    inputY1 = 0;
                    inputY2 = HEIGHT - zoomPixels;
                }
                else
                {
                    // take off pixels on the top
                    inputY1 = zoomPixels;
                    inputY2 = HEIGHT;
                }

                zoomChanged = true;
                frameCount = 0;
            }
        }
    }

    return 0;
}



/*

TO COMPILE:

g++ -c main.cpp -I"C:\\msys64\\ucrt64\\include\\SFML"
g++ main.o -o main -L"C:\\msys64\\ucrt64\\lib" -lsfml-graphics -lsfml-window -lsfml-system
./main

WITH MULTIPLE FILES
only include -fopenmp if you are using openmp
g++ -fopenmp *.cpp -I"C:\\msys64\\ucrt64\\include\\SFML" -o main -L"C:\\msys64\\ucrt64\\lib" -lsfml-graphics -lsfml-window -lsfml-system
./main

*/