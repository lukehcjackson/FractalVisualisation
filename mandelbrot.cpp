#include <SFML/Graphics.hpp>
#include "helper.h"
#include "omp.h"

//MAX_ITERATIONS controls the level of detail of the whole image
//50 is my working value
//1000 looks amazing but takes a long time to render a frame - requires mulithreading
int MAX_ITERATIONS = 5500;

//define colour palette array:
//RAINBOW MODE
/*
sf::Color colorPalette[18] = {
    sf::Color(0,0,0),
    sf::Color(212,0,0),
    sf::Color(255,138,7),
    sf::Color(255,193,7),
    sf::Color(255,235,59),
    sf::Color(205,220,57),
    sf::Color(139,192,74),
    sf::Color(76,175,80),
    sf::Color(0,150,136),
    sf::Color(0,188,212),
    sf::Color(3,169,244),
    sf::Color(33,150,243),
    sf::Color(63,81,181),
    sf::Color(103,58,183),
    sf::Color(156,39,176),
    sf::Color(233,30,99),
    sf::Color(244,67,54),
    sf::Color(255,255,255)
};
*/

//PINK-BLUE PASTEL MODE
/*
sf::Color colorPalette[12] = {
    sf::Color(0,0,0),
    sf::Color(255,203,242),
    sf::Color(243,196,251),
    sf::Color(236,188,253),
    sf::Color(229,179,254),
    sf::Color(226,175,255),
    sf::Color(222,170,255),
    sf::Color(216,187,255),
    sf::Color(208,209,255),
    sf::Color(200,231,255),
    sf::Color(192,253,255),
    sf::Color(255,255,255)
};
*/

//MAGENTA-BLUE MODE
sf::Color colorPalette[12] = {
    sf::Color(0,0,0),
    sf::Color(247,37,133),
    sf::Color(181,23,158),
    sf::Color(114,9,183),
    sf::Color(86,11,173),
    sf::Color(72,12,168),
    sf::Color(58,12,163),
    sf::Color(63,55,201),
    sf::Color(67,97,238),
    sf::Color(72,149,239),
    sf::Color(76,201,240),
    sf::Color(255,255,255)
};

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

    //need an x,y coordinate
    //i%WIDTH gives 4 copies, (i*4)%WIDTH gives 16 copies
    long double x = (i)% (WIDTH*4);
    long double y = (i * 1) / WIDTH;

    //map graph coordinates back into pixels to render
    x = plot_map(xOutStart, xOutEnd, 0, WIDTH, x);
    y = plot_map(yOutStart, yOutEnd, 0, HEIGHT, y);
    
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
    
    int colorStepSize = MAX_ITERATIONS / (sizeof(colorPalette) / sizeof(colorPalette[0]));
    sf::Color pixelColor;

    for (int i = 0; i < MAX_ITERATIONS; i += colorStepSize) {
        if (n < i) {
            pixelColor = colorPalette[i / colorStepSize];
            break;
        }
    }

    //possible change: exclude black and white from the loop above this
    //then manually set black if n = max, white if n = max - 1
    if (n == MAX_ITERATIONS) {
        pixelColor = colorPalette[0];
    }

    //GREYSCALE MODE
    //pixelColor = sf::Color(n * 255 / MAX_ITERATIONS, n * 255 / MAX_ITERATIONS, n * 255 / MAX_ITERATIONS, 255);

    
    pixels[i] = pixelColor.r;
    pixels[i + 1] = pixelColor.g;
    pixels[i + 2] = pixelColor.b;
    pixels[i + 3] = 255;

    
    
   }

    return pixels;
}