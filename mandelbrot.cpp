#include <SFML/Graphics.hpp>
#include "helper.h"
#include "palette.h"
#include "omp.h"

//MAX_ITERATIONS controls the level of detail of the whole image
//50 is my working value
//1000 looks amazing but takes a long time to render a frame - requires mulithreading
//5000 allows a lot of zoom but initial images don't look that cool
//crashes if less than about 20
const int MAX_ITERATIONS = 1000;

extern sf::Color colorPalette[12];


sf::Uint8* mandelbrot(int WIDTH, int HEIGHT, long double xOutStart, long double xOutEnd, long double yOutStart, long double yOutEnd, sf::Uint8* pixels) {

    /*
    In this method, generate the mandelbrot set for each pixel on the screen
    Return the modified pixel array
    */
   

   omp_set_num_threads(10);
   //100% CPU utilisation when num_threads = num logical cores in your cpu
   #pragma omp parallel for
   for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
    //each i is a pixel on the screen

    //find x,y coordinate from pixel array index
    int xCoord = (i % (WIDTH * 4)) / 4;
    int yCoord = ((i * 1.0) / WIDTH) / 4;

    //convert back into graph coordinates
    long double x = map(xOutStart, xOutEnd, 0, WIDTH, xCoord);
    long double y = map(yOutStart, yOutEnd, 0, HEIGHT, yCoord);
    
    long double cx = x;
    long double cy = y;

    int n = 0;
    
    while (n < MAX_ITERATIONS) {
        //Z = Z^2 + C
        //C is our initial point a,b
        //Z = x + yi, Z^2 = (x^2 - y^2) + 2xyi
        //so we can represent the real and imaginary parts of the result without needing to use any complex number libraries

        //Z^2
        long double re = x * x - y * y;
        long double im = 2 * x * y;
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
    sf::Color pixelColor = calculatePixelColor_iterative(n, MAX_ITERATIONS);

    //GREYSCALE MODE
    //sf::Color pixelColor = calculatePixelColor_iterativeGreyscale(n, MAX_ITERATIONS);
    
    pixels[i] = pixelColor.r;
    pixels[i + 1] = pixelColor.g;
    pixels[i + 2] = pixelColor.b;
    pixels[i + 3] = 255;

   }

    return pixels;
}