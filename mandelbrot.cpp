#include <SFML/Graphics.hpp>
#include "mandelbrot.h"
sf::Uint8* mandelbrot(int WIDTH, int HEIGHT, sf::Uint8* pixels) {

    /*
    In this method, generate the mandelbrot set for each pixel on the screen
    Return the modified pixel array
    */
   
   //TODO: NEED TO SCALE THE SIZE OF THE CANVAS APPROPRIATELY!!!
   //the canvas coordinates are from 0-1600, 0-900 but we want to be looking at a scale more like -2 to 2, -2 to 2
   float scaleFactor = 800;


   int outStart = -2;
   int outEnd = 2;

   double xSlope = 1.0 * (outEnd - outStart) / WIDTH;
   double ySlope = 1.0 * (outEnd - outStart) / HEIGHT;

   //try arduino suggestsion on stackoverflow

   int MAX_ITERATIONS = 50;

   for (int i = 0; i < WIDTH * HEIGHT * 4 ; i += 4) {
    //each i is a pixel on the screen

    //need an x,y coordinate
    int x = i % WIDTH;
    int y = i / WIDTH;

    //want to 'map' x,y from 0->width, 0->height to -2->2, -2->2
    
    double newX = -2 + xSlope * x;
    double newY = -2 + ySlope * y;
    x = newX;
    y = newY;
    

    int n = 0;
    
    while (n < MAX_ITERATIONS) {
        //Z = Z^2 + C
        //C is our initial point a,b
        //Z = x + yi, Z^2 = (x^2 - y^2) + 2xyi
        //so we can represent the real and imaginary parts of the result without needing to use any complex number libraries

        //Z^2
        float re = x * x - y * y;
        float im = 2 * x * y;
        //+C
        re += x;
        im += y;

        //check if the magnitude of Z is greater than 2
        //=> sqrt(re^2 + im^2) > 2 so re^2 + im^2 > 4
        if (re * re + im * im > 4) {
            //this point is not in the mandelbrot set
            break;
        }

        n++;
    }

    //we now want to colour the pixels depending on how many iterations they took to diverge
    
    int color;
    if (n == MAX_ITERATIONS) {
        color = 255;
    } else {
        color = 0;
    }

    pixels[i] = color;
    pixels[i + 1] = color;
    pixels[i + 2] = color;
    pixels[i + 3] = 255;
    
    
    //pixels[i] = n * 255 / MAX_ITERATIONS; //red
    //pixels[i + 1] = n * 255 / MAX_ITERATIONS; //green
    //pixels[i + 2] = n * 255 / MAX_ITERATIONS; //blue
    //pixels[i + 3] = 255; //alpha
    
   }

    return pixels;
}