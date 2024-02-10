#include <SFML/Graphics.hpp>
#include "mandelbrot.h"

int WIDTH = 1600;
int HEIGHT = 900;

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
    //we can swap between these two pixel arrays and use them like a buffer, rather than creating a new pixel array each frame
    sf::Uint8* newPixels = new sf::Uint8[WIDTH * HEIGHT * 4];

    long long frameCount = 0;


    //this is the loop - runs once per frame
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        //render background
        window.clear(sf::Color::Cyan);

        //TEST!!! update pixel array
        /*
        for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
            newPixels[i] = frameCount % 255; //red
            newPixels[i + 1] = (frameCount + i + 500) % 255; //green
            newPixels[i + 2] = (i * 100 + 150) % 255; //blue
            newPixels[i + 3] = 255; //alpha
        }

        currentPixels = newPixels;
        */

        currentPixels = mandelbrot(WIDTH, HEIGHT, currentPixels);
        //TODO : do i even need newPixels ??

        fractalTexture.update(currentPixels);

        //draw the sprite
        window.draw(fractalSprite);
        //render all calls to draw()
        window.display();

        frameCount++;

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