#include <SFML/Graphics.hpp>
#include <iostream>
#include "helper.h"
#include "mandelbrot.h"

extern int WIDTH;
extern int HEIGHT;

int main() {
    //create ContextSettings object to control antialiasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    //create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fractal Visualisation", sf::Style::Default, settings);

    //create the texture we will draw to
    sf::Texture fractalTexture;
    //make it blank to begin with
    if (!fractalTexture.create(WIDTH, HEIGHT)) {
        //error
        return -1;
    }

    //draw the texture 'smooth' - with antialiasing
    fractalTexture.setSmooth(true);

    //create the sprite we will draw the texture to
    sf::Sprite fractalSprite(fractalTexture);

    //create the main pixel array
    sf::Uint8* currentPixels = new sf::Uint8[WIDTH * HEIGHT * 4]; //4 bytes per pixel (RGBA)

   //these variables define the rectangular window we are looking at on the graph of the mandelbrot set
   float xStart = -1;
   float xEnd = -0.6;
   float yStart = 0;
   float yEnd = 0.4;

   //these variables define the rectangle drawn on screen with the mouse
   float inputX1;
   float inputY1;
   float inputX2;
   float inputY2;

   //create a lock for the mouse click function
   static bool clickLock = false;
   //and create a lock for when the zoom is changed
   static bool zoomChanged = false;

   bool firstFrame = true;

    //this is the loop - runs once per frame
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed && !clickLock) {
                //mouse button clicked
                inputX1 = event.mouseButton.x;
                inputY1 = event.mouseButton.y;
                //lock this function so it can't be called again until the mouse button is released
                clickLock = true;
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                //mouse button released
                inputX2 = event.mouseButton.x;
                inputY2 = event.mouseButton.y;

                std::cout << "inputX1: " << inputX1 << " inputY1: " << inputY1 << " inputX2: " << inputX2 << " inputY2: " << inputY2 << std::endl;

                //unlock the mouse click function and the zoom function
                clickLock = false;
                zoomChanged = true;
            }
            
        }

        //render background
        window.clear(sf::Color::Cyan);
        
        if (zoomChanged) {
            //a new rectangle has been drawn on the screen
            //this is the new zoom window
            //convert from pixel coordinates to 'graph' coordinates using helper.map() 
            double newX1 = map(xStart, xEnd, 0, WIDTH, inputX1);
            double newX2 = map(xStart, xEnd, 0, WIDTH, inputX2);
            double newY1 = map(yStart, yEnd, 0, HEIGHT, inputY1);
            double newY2 = map(yStart, yEnd, 0, HEIGHT, inputY2);

            std::cout << "newX1: " << newX1 << " newX2: " << newX2 << " newY1: " << newY1 << " newY2: " << newY2 << std::endl;

            currentPixels = mandelbrot(WIDTH, HEIGHT, newX1, newX2, newY1, newY2, currentPixels);
            zoomChanged = false;
        }

        if (firstFrame) {
            currentPixels = mandelbrot(WIDTH, HEIGHT, xStart, xEnd, yStart, yEnd, currentPixels);
            firstFrame = false;
        }
        
        fractalTexture.update(currentPixels);

        //draw the sprite
        window.draw(fractalSprite);
        //render all calls to draw()
        window.display();

        

        //xOutStart -= 0.01;
        //xOutEnd -= 0.01;
        //yOutStart -= 0.01;
        //yOutEnd -= 0.01;

    }

    return 0;
}

/*

TO COMPILE:

g++ -c main.cpp -I"C:\\msys64\\ucrt64\\include\\SFML"
g++ main.o -o main -L"C:\\msys64\\ucrt64\\lib" -lsfml-graphics -lsfml-window -lsfml-system
./main

WITH MULTIPLE FILES
g++ *.cpp -I"C:\\msys64\\ucrt64\\include\\SFML" -o main -L"C:\\msys64\\ucrt64\\lib" -lsfml-graphics -lsfml-window -lsfml-system
./main   

*/