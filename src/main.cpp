/*
author: Oliver Franke
email: ofranke@posteo.de
github: ofrankeADD
latest update: 22.11.2021
*/

#include <iostream>
#include <stdio.h>
#include <time.h>
using namespace std;
#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.hpp"
#include "magnChain.hpp"
#include "constants.h"
#include "dataFileHandling.hpp"
#include "metropolis.hpp"

/*
The initial conditions are build as follows:
- create m cubes aligned along the y-axis in the xy-plane
- each cube consists of n^3 equidistantly placed dipoles
- each dipole has an magnetic momentum vector initially pointing in the y-direction
- the total magnetic chain consists of m x n^3 "elemental" dipoles

The Metropolis Monte Carlo Algorithm is executed as follows:
- calculate the total energy for all individual dipole-dipole interactions of all cubes in the chain
- for each Monte Carlo step, either rotate or translate a randomly selected cube
- check that the updated cube position is not colliding with any other cubic volume
- calculate the updated total energy budget
- if the new chain configuration is more energetically favourable than the old one, keep it
- otherwise within a certain probability, discard the new chain configuration

The data output is written at every stepping time as a snapshot containing the position vectors and the energy value.
- the Python script is used in order to visualize the movement of the cubic chain, either for one snapshot or for all in form of a movie
- the input parameters for the Python script have to be: n, 'all', m, stepsize, total_runs
*/

int main()
{
    time_t start_t, end_t;
    double sec_t;

    magnChain chain(numCubes_, edgeLength_);
    cout << "edgeLength: " << edgeLength_ << endl;

    double angleCubePos = 0.0, angleMagnMom = 0.0;
    double startenergy;
    
    dataFileHandling dataFileIO;

    metropolisInitialConditions(chain, dataFileIO, startenergy);
    
    cout << "Metropolis Method: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    /*
     * 
     * 
     * 
     * 
     * 
     * 
     * 
     * 
     */
    time(&start_t);
    metropolisMonteCarloAlgorithm(chain, dataFileIO, angleCubePos, angleMagnMom);
    time(&end_t);
    sec_t = (double)(end_t - start_t);
    cout << "Simulation took " << sec_t << " seconds." << endl; 
     
    metropolisOutputResults(chain, dataFileIO, angleCubePos, angleMagnMom, startenergy);

    return 0;
}
