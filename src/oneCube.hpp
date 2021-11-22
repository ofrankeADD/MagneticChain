#ifndef ONECUBE_H
#define ONECUBE_H

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>

#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.hpp"
#include "constants.h"

class oneCube
{
private:
    
public:

    double dipInteract(cube oneC);
    
    cube rotateMagnMom(cube temp, int m, vec axis, double phi);
    
};

#endif
