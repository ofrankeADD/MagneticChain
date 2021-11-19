#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>
#include "vertex.cpp"

const double PI_ = 3.141592653589793238462643383279;
const double my0_ = 4.0*PI_*pow(10, -7);
const double kB_ = 1.38064852*pow(10, -23);
const double T_ = 300.0;
const int numCubes_ = 15;//minimum of 3
const int numElem_ = 6;//has to be even
const int totalSize_ = pow(numElem_, 3);
const double edgeLength_ = 43.60*pow(10, -9);
//const double dipLength_ = edgeLength_/(double)numElem_;
const double magnPerVol_ = 0.48*pow(10, 6);
//const double magnPerDip_ = (PI_/6.0)*magnPerVol_*pow(edgeLength_/(double)numElem_, 3);
const vec nullVec_(0,0,0);

const double constrFac_ = 50.0;
const double r_ = 0.2*edgeLength_;
const double kappa_ = 2.0*kB_*T_;
const int runs_ = 10000;//10000000;
const int steps_ = runs_/250;

#endif
