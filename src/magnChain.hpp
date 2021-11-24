#ifndef MAGNCHAIN_H
#define MAGNCHAIN_H

#include <iostream>
#include <stdio.h>
#include <random>
#include <math.h>
#include <vector>
#include <omp.h>

#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.hpp"
#include "constants.h"

class magnChain
{
private:
    double edgeLen, magnPerDip, Econstr;
    
public:
    vector<cube> cubeArray;
    int chainSize;

    magnChain(){}

    magnChain(int numCubes, double cubeLen) : cubeArray(numCubes)
    {
        chainSize = numCubes;
        edgeLen = cubeLen;
        magnPerDip = magnPerVol_*pow(edgeLen/(double)numElem_/2.0, 3);
    }
    
    ~magnChain(){}

    void printChain();

    void initCubesYaxis(vec cubeVec, vec mom, double d);
    
    /*
    - all dipole-dipole interactions between two cubes
    */
    double dipInteract(cube& c1, cube& c2);
    
    /*
    - calculate the magnetic energy via the dipole-dipole interactions for the whole chain
    */
    double chainInteract();
    
    /*
    - only allow a rotation or translation of a randomly picked cube, if the new altered position 
    of this cube is not colliding with any other cube of the chain
    */
    bool checkVolExclusion(cube& c, const int& Cube);
    
    cube rotateSelf(const int& c, vec& axis, const double& phi);
    
    cube rotateSphere(const int& c, const double& theta, const double& phi);
    
    cube translateCube(const int& c, const vec& trans);
    
    void findClosestNeighbours(const int& c, int& firstNearest, int& secondNearest);
    
    void timeAverage(const int& c, double& angleCubePos, double& angleMagnMom);
};

#endif