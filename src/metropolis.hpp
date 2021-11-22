#ifndef METROPOLIS_H
#define METROPOLIS_H 

#include <iostream>
#include <stdio.h>
#include <random>
#include <math.h>

#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.hpp"
#include "magnChain.hpp"
#include "constants.h"
#include "dataFileHandling.hpp"

void metropolisMonteCarloAlgorithm(magnChain& chain, dataFileHandling& dataFileIO, double& angleCubePos, double& angleMagnMom);

void metropolisInitialConditions(magnChain& chain, dataFileHandling& dataFileIO, double& startenergy);

void metropolisOutputResults(magnChain& chain, dataFileHandling& dataFileIO, double& angleCubePos, double& angleMagnMom, double& startenergy);

#endif