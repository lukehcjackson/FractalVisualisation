#include <SFML/Graphics.hpp>
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


    //this is the loop - runs once per frame
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        //render background
        window.clear(sf::Color::Cyan);

        currentPixels = mandelbrot(WIDTH, HEIGHT, currentPixels);
        fractalTexture.update(currentPixels);

        //draw the sprite
        window.draw(fractalSprite);
        //render all calls to draw()
        window.display();

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