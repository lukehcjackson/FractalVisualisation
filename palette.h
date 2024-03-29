sf::Color calculatePixelColor_iterative (int iterations, int maxIterations);
sf::Color calculatePixelColor_iterativeGreyscale (int iterations, int maxIterations);

sf::Color calculatePixelColor_senikoTwoColours(int iterations, sf::Color c1, sf::Color c2);
sf::Color calculatePixelColor_senikoThreeColours(int iterations, int maxIterations, sf::Color c1, sf::Color c2, sf::Color c3);

void calculateIterationCounts(int iterations, int x, int y);
void printIterationCounts();
void HistogramColouring();
sf::Color getHistogramColour(int x, int y);
void resetHistogramColouring();