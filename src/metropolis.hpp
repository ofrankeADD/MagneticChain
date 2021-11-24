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

/*
The Metropolis Monte Carlo Algorithm is executed as follows:
- calculate the total magnetic energy for all individual dipole-dipole interactions of all cubes in the chain
- this process is parallelized using fopenmp
- for each Monte Carlo step, either rotate or translate a randomly selected cube
- check that the updated cube position is not colliding with any other cubic volume
- calculate the updated total energy budget
- if the new chain configuration is more energetically favourable than the old one, keep it
- otherwise within a certain probability, discard the new chain configuration
*/
void metropolisMonteCarloAlgorithm(magnChain& chain, dataFileHandling& dataFileIO, double& angleCubePos, double& angleMagnMom);

/*
The initial conditions are build as follows:
- create m cubes aligned along the y-axis in the xy-plane
- each cube consists of n^3 equidistantly placed dipoles
- each dipole has an magnetic momentum vector initially pointing in the y-direction
- the total magnetic chain consists of m x n^3 "elemental" dipoles
*/
void metropolisInitialConditions(magnChain& chain, dataFileHandling& dataFileIO, double& startenergy);

/*
The data output is written at every stepping time as a snapshot containing the position vectors and the energy value.
- the Python script is used in order to visualize the movement of the cubic chain, either for one snapshot or for all in form of a movie
- the input parameters for the Python script have to be: n, 'all', m, stepsize, total_runs
*/
void metropolisOutputResults(magnChain& chain, dataFileHandling& dataFileIO, double& angleCubePos, double& angleMagnMom, double& startenergy);

#endif