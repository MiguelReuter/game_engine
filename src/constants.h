#ifndef constants_h
#define constants_h

#include <map>
#include <string>

using namespace std;

const int RESOLUTION[2] = {640, 480};
const int NOMINAL_FPS = 60;
const float NOMINAL_DT = 1. / NOMINAL_FPS;

// physics
const int PIXELS_NB_PER_METER = 64;
const float GRAVITY[2] = {0, 10}; // m.s^-2

enum DIRECTIONS {
    LEFT = 0,
    TOP = 1,
    RIGHT = 2,
    BOTTOM = 3};

#endif /* constants_h */
