#include <SFML/Graphics.hpp>
#include "helper.h"
#include "palette.h"
#include "omp.h"

const long double c_re = -0.20335400390625002;
const long double c_im = -0.677032470703125;
//R > 0 such that R**2 - R >= sqrt(c_re **2 + c_im **2)
const double escapeRadius = 2; 
const int MAX_ITERATIONS = 500;

extern sf::Color colorPalette[12];


sf::Uint8* julia(int WIDTH, int HEIGHT, long double xOutStart, long double xOutEnd, long double yOutStart, long double yOutEnd, sf::Uint8* pixels) {
    
        omp_set_num_threads(10);
        #pragma omp parallel for
        for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
            long double x = i % (WIDTH * 4);
            long double y = (i * 1.0) / WIDTH;
    
            x = plot_map(xOutStart, xOutEnd, 0, WIDTH, x);
            y = plot_map(yOutStart, yOutEnd, 0, HEIGHT, y);
    
            int n = 0;
            while (n < MAX_ITERATIONS) {
                long double re = x * x - y * y;
                long double im = 2 * x * y;

                x = re + c_re;
                y = im + c_im;

                if (x * x + y * y > escapeRadius * escapeRadius) {
                    break;
                }

                n++;
            }
    
            int colorStepSize = MAX_ITERATIONS / (sizeof(colorPalette) / sizeof(colorPalette[0]));
            sf::Color pixelColor = calculatePixelColor_iterative(n, MAX_ITERATIONS, colorStepSize);
            //GREYSCALE MODE
            //sf::Color pixelColor = calculatePixelColor_iterativeGreyscale(n, MAX_ITERATIONS);

            pixels[i] = pixelColor.r;
            pixels[i + 1] = pixelColor.g;
            pixels[i + 2] = pixelColor.b;
            pixels[i + 3] = pixelColor.a;
        }
        return pixels;
}