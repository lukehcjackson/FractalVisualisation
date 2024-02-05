#include <SFML/Graphics.hpp>

int WIDTH = 1600;
int HEIGHT = 900;


int main() {
    //create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fractal Visualisation");

    //create the texture we will draw to
    sf::Texture fractalTexture;
    //make it blank to begin with
    if (!fractalTexture.create(WIDTH, HEIGHT)) {
        //error
        return -1;
    }

    //create the sprite we will draw the texture to
    sf::Sprite fractalSprite(fractalTexture);

    //create the main pixel array
    sf::Uint8* currentPixels = new sf::Uint8[WIDTH * HEIGHT * 4]; //4 bytes per pixel (RGBA)
    //we can swap between these two pixel arrays and use them like a buffer, rather than creating a new pixel array each frame
    sf::Uint8* newPixels = new sf::Uint8[WIDTH * HEIGHT * 4];

    long long frameCount = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        //render background
        window.clear(sf::Color::Cyan);

        //TEST!!! update pixel array
        for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
            newPixels[i] = frameCount % 255; //red
            newPixels[i + 1] = 0; //green
            newPixels[i + 2] = 0; //blue
            newPixels[i + 3] = 255; //alpha
        }

        currentPixels = newPixels;

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

*/