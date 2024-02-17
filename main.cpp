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

   //these variables define our 'canvas', the whole window we are rendering
   float xCanvasStart = 0;
   float xCanvasEnd = WIDTH;
   float yCanvasStart = 0;
   float yCanvasEnd = HEIGHT;

   //these variables define the rectangular window we are zooming in on, on the graph of the mandelbrot set
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

        if (firstFrame) {
            currentPixels = mandelbrot(WIDTH, HEIGHT, xStart, xEnd, yStart, yEnd, currentPixels);
            std::cout << "xStart: " << xStart << " xEnd: " << xEnd << " yStart: " << yStart << " yEnd: " << yEnd << std::endl;
            firstFrame = false;
        }

       if (zoomChanged && !firstFrame) {

        float newX1 = map(xStart, xEnd, xCanvasStart, xCanvasEnd, inputX1);
        float newY1 = map(yStart, yEnd, yCanvasStart, yCanvasEnd, inputY1);
        float newX2 = map(xStart, xEnd, xCanvasStart, xCanvasEnd, inputX2);
        float newY2 = map(yStart, yEnd, yCanvasStart, yCanvasEnd, inputY2);

        std::cout << "newX1: " << newX1 << " newY1: " << newY1 << " newX2: " << newX2 << " newY2: " << newY2 << std::endl;

        currentPixels = mandelbrot(WIDTH, HEIGHT, newX1, newX2, newY1, newY2, currentPixels);
        xStart = newX1;
        xEnd = newX2;
        yStart = newY1;
        yEnd = newY2;

   
        zoomChanged = false;
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