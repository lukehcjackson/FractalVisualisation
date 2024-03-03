#include <SFML/Graphics.hpp>
#include <cmath>
#include "helper.h"
#include "palette.h"
#include "omp.h"

#include <iostream>

long double c_re = -0.77146;
long double c_im = -0.10119;
// R > 0 such that R**2 - R >= sqrt(c_re **2 + c_im **2)
const double escapeRadius = 2;
const int MAX_ITERATIONS = 50;

extern sf::Color colorPalette[40];


sf::Uint8* julia(int WIDTH, int HEIGHT, long double xOutStart, long double xOutEnd, long double yOutStart, long double yOutEnd, sf::Uint8 *pixels, long double a)
{
    /*
    OPTIONAL: ANIMATING THE JULIA SET
    c = 0.7885 * e^ia where a ranges from 0 to 2PI
    alterate form: 0.7885 * cos(a) + 0.7885 * sin(a)i
    just comment out the two lines below to use a fixed value of c
    */
   
    c_re = 0.7885 * cos(a);
    c_im = 0.7885 * sin(a);
    
    omp_set_num_threads(10);
    #pragma omp parallel for
    for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4)
    {

        int xCoord = (i % (WIDTH * 4)) / 4;
        int yCoord = ((i * 1.0) / WIDTH) / 4;

        long double x = map(xOutStart, xOutEnd, 0, WIDTH, xCoord);
        long double y = map(yOutStart, yOutEnd, 0, HEIGHT, yCoord);


        int n = 0;
        while (n < MAX_ITERATIONS)
        {
            long double re = x * x - y * y;
            long double im = 2 * x * y;

            x = re + c_re;
            y = im + c_im;

            if (x * x + y * y > escapeRadius * escapeRadius)
            {
                break;
            }

            n++;
        }

        //DO PIXEL-BY-PIXEL COLOURING HERE

        //sf::Color pixelColor = calculatePixelColor_iterative(n, MAX_ITERATIONS);
        //sf::Color pixelColor = calculatePixelColor_iterativeGreyscale(n, MAX_ITERATIONS);
        //sf::Color pixelColor = calculatePixelColor_senikoTwoColours(n, sf::Color::Black, sf::Color::White);
        //sf::Color pixelColor = calculatePixelColor_senikoThreeColours(n, MAX_ITERATIONS, sf::Color::Black, sf::Color::White, sf::Color::Red);

        /*
        pixels[i] = pixelColor.r;
        pixels[i + 1] = pixelColor.g;
        pixels[i + 2] = pixelColor.b;
        pixels[i + 3] = pixelColor.a;
        */

        calculateIterationCounts(n, xCoord, yCoord);

    }

    //printIterationCounts();
    HistogramColouring();
    #pragma omp parallel for
    for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4)
    {
        int xCoord = (i % (WIDTH * 4)) / 4;
        int yCoord = ((i * 1.0) / WIDTH) / 4;

        sf::Color pixelColor = getHistogramColour(xCoord, yCoord);
        pixels[i] = pixelColor.r;
        pixels[i + 1] = pixelColor.g;
        pixels[i + 2] = pixelColor.b;
        pixels[i + 3] = pixelColor.a;
    }
    resetHistogramColouring();
    

    return pixels;
}