#include <SFML/Graphics.hpp>
#include "helper.h"

sf::Uint8* mandelbrot(int WIDTH, int HEIGHT, sf::Uint8* pixels) {

    /*
    In this method, generate the mandelbrot set for each pixel on the screen
    Return the modified pixel array
    */
   
   //the canvas coordinates are from 0-1600, 0-900 but we want to be looking at a scale more like -2 to 2, -2 to 2
   //https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
   
   //'zooming' means modifying outStart and outEnd
   //for a fullscreen mandelbrot: x -2 to 1, y -1 to 1
   //this is basically the coordinates of the rectangle we are looking at on a graph of the mandelbrot set
   //a click-drag zoom function would map screen coordinates to new graph coordinates representing the rectangle drawn in graph space
/*
   float xOutStart = -1;
   float xOutEnd = -0.6;
   float yOutStart = 0;
   float yOutEnd = 0.4;
*/
   float xOutStart = -2;
   float xOutEnd = 1;
   float yOutStart = -1;
   float yOutEnd = 1;

   int MAX_ITERATIONS = 50;
   
   for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
    //each i is a pixel on the screen

    //need an x,y coordinate
    //i%WIDTH gives 4 copies, (i*4)%WIDTH gives 16 copies
    double x = (i)% (WIDTH*4);
    double y = (i * 1) / WIDTH;

    x = map(xOutStart, xOutEnd, 0, WIDTH, x);
    y = map(yOutStart, yOutEnd, 0, HEIGHT, y);
    
    double cx = x;
    double cy = y;

    int n = 0;
    
    while (n < MAX_ITERATIONS) {
        //Z = Z^2 + C
        //C is our initial point a,b
        //Z = x + yi, Z^2 = (x^2 - y^2) + 2xyi
        //so we can represent the real and imaginary parts of the result without needing to use any complex number libraries

        //Z^2
        double re = x * x - y * y;
        double im = 2 * x * y;
        //+C
        x = re + cx;
        y = im + cy;

        //check if the magnitude of Z is greater than 2
        //=> sqrt(re^2 + im^2) > 2 so re^2 + im^2 > 4
        if (x * x + y * y > 4) {
            //this point is not in the mandelbrot set
            break;
        }

        n++;
    }
    
   

    //we now want to colour the pixels depending on how many iterations they took to diverge
    
    int color;
    if (n == MAX_ITERATIONS) {
        color = 0;
        //color = n * 255 / MAX_ITERATIONS;
    } else {
        color = n * 255 / MAX_ITERATIONS;
    }
    
    pixels[i] = color; //red
    pixels[i + 1] = color; //green
    pixels[i + 2] = color; //blue
    pixels[i + 3] = 255; //alpha
    
   }

    return pixels;
}