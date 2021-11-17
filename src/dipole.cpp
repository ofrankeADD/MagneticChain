#ifndef DIPOLE_H
#define DIPOLE_H

#include <iostream>
#include <stdio.h>
using namespace std;
#include "vertex.cpp"

class dipole
{
private:

    int posInDipArray;
    //double phi, theta;
    
public:
    vec dipMagnMom, dipVec;

    dipole(){}
    
    dipole(int posCube, vec dipMagneticMomentum, vec dipVector) : dipMagnMom(dipMagneticMomentum), dipVec(dipVector)
    {
        posInDipArray = posCube;
    }
    
    ~dipole(){}
    
    friend ostream& operator<<(ostream &stream, dipole dip);
};

inline ostream& operator <<(ostream& stream, dipole dip)
{
    return stream << "{[" << dip.posInDipArray << "]" << dip.dipVec << "}";
}

#endif
