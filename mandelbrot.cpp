#include <SFML/Graphics.hpp>
#include "mandelbrot.h"
sf::Uint8* mandelbrot(int WIDTH, int HEIGHT, sf::Uint8* pixels) {

    /*
    In this method, generate the mandelbrot set for each pixel on the screen
    Return the modified pixel array
    */
   
   //the canvas coordinates are from 0-1600, 0-900 but we want to be looking at a scale more like -2 to 2, -2 to 2
   //https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
   //float scaleFactor = 800;
   float outStart = -2.5;
   float outEnd = 2.5;
   double xSlope = 1.0 * (outEnd - outStart) / WIDTH * 0.25;
   double ySlope = 1.0 * (outEnd - outStart) / HEIGHT * 0.25;


   int MAX_ITERATIONS = 50;
   
   for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
    //each i is a pixel on the screen

    //need an x,y coordinate
    //i%WIDTH gives 4 copies, (i*4)%WIDTH gives 16 copies
    double x = (i)% (WIDTH*4);
    double y = (i * 1) / WIDTH;

    //want to 'map' x,y from 0->width, 0->height to -2->2, -2->2
    //this is not exactly precisely right and should be moved out into another function

    /*
    NOTES FOR FIXING SCALING:
    leaving out the *0.25 in the slope calculations gives you 4 mandelbrots along the top
    doing ySlope * 0.25 stretches them out so the sets fill the height of the screen
    doing xSlope * 0.25 stretches them out so they WOULD fit the screen, but it cuts off and tiles 4 times
    this is fixed by doing i % (WIDTH * 4) -> want to get my head around this more, is there a nicer way to write this / do this correction?
    ALSO WANT TO ROUND THE MAPPING FUNCTION SO IT'S MORE ACCURATE -> SEE STACKOVERFLOW POST
    
    */

    double newX = outStart + xSlope * x;
    double newY = outStart + ySlope * y;
    x = newX;
    y = newY;
    
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
        color = 255;
    } else {
        color = 0;
    }
    
    pixels[i] = n * 255 / MAX_ITERATIONS; //red
    pixels[i + 1] = n * 255 / MAX_ITERATIONS; //green
    pixels[i + 2] = n * 255 / MAX_ITERATIONS; //blue
    pixels[i + 3] = 255; //alpha
    
   }

    return pixels;
}