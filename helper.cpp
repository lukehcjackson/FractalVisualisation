int WIDTH = 1600;
int HEIGHT = 900;
/*
    NOTES FOR FIXING SCALING:
    leaving out the *0.25 in the slope calculations gives you 4 mandelbrots along the top
    doing ySlope * 0.25 stretches them out so the sets fill the height of the screen
    doing xSlope * 0.25 stretches them out so they WOULD fit the screen, but it cuts off and tiles 4 times
    this is fixed by doing i % (WIDTH * 4) -> want to get my head around this more, is there a nicer way to write this / do this correction?
    ALSO WANT TO ROUND THE MAPPING FUNCTION SO IT'S MORE ACCURATE -> SEE STACKOVERFLOW POST
    
    */

/*
long double plot_map(long double outStart, long double outEnd, long double inStart, long double inEnd, long double point) {
    long double slope = 1.0 * (outEnd - outStart) / (inEnd - inStart) * 0.25;   
    return outStart + slope * point;
}
*/

long double map(long double outStart, long double outEnd, long double inStart, long double inEnd, long double point) {
    long double slope = 1.0 * (outEnd - outStart) / (inEnd - inStart);   
    return outStart + slope * (point - inStart);
}
