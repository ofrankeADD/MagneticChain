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
