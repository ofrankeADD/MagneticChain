#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "vertex.cpp"
#include "dipole.cpp"
#include "constants.h"

class cube
{
private:
    vec dipPos;
    double dipLength;
    double ctheta;//in range [0,PI]
    double cphi;//in range [-PI,PI]
    
public:
    vec globalPos, initMom;
    vector<dipole> dipArray;
    
    cube(){}
    
    cube(vec centerPos, vec mom, double edgeLen) : globalPos(centerPos), initMom(mom), dipArray(totalSize_)
    {
        dipLength = edgeLen/(double)numElem_;
        initDipVecs();
        //sphereCoords();
    }
    
    ~cube(){}
    
    void initDipVecs();
    
    //issues due to precision of rounding
    void sphereCoords();
    
    friend ostream& operator<<(ostream &stream, cube c);
};

inline ostream& operator <<(ostream& stream, cube c)
{
    return stream << "{" << c.globalPos << "," << c.initMom << "}";
}

#endif
