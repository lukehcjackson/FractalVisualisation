#include <SFML/Graphics.hpp>

/*
USAGE:
DEFINE colorPalette[n] as an array of sf::Color
then, in fractal function, be sure to include palette.cpp and use extern sf::Color colorPalette[n]
remember to update n in your fractal function!
*/


//define colour palette array:
//RAINBOW MODE

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
/*
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
*/

sf::Color calculatePixelColor_iterative (int iterations, int maxIterations, int colorStepSize) {

    sf::Color pixelColor;
    for (int i = 0; i < maxIterations; i += colorStepSize) {
        if (iterations < i) {
            pixelColor = colorPalette[i / colorStepSize];
            break;
        }
    }

    return pixelColor;
}

sf::Color calculatePixelColor_iterativeGreyscale (int iterations, int maxIterations) {
    return sf::Color(iterations * 255 / maxIterations, iterations * 255 / maxIterations, iterations * 255 / maxIterations, 255);
}