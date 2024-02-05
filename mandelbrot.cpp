#include <SFML/Graphics.hpp>
#include "mandelbrot.h"
sf::Uint8* mandelbrot() {

    /*
    In this method, generate the mandelbrot set for each pixel on the screen (using the dimensions given as an argument)
    Return an array of Uint8 of size WIDTH * HEIGHT * 4, where each group of 4 bytes represents the RGBA values of a pixel
    */

    return new sf::Uint8[1600 * 900 * 4];
}