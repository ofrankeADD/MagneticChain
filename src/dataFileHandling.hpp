#ifndef DATAFILEHANDLING_H
#define DATAFILEHANDLING_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.hpp"
#include "constants.h"
#include "magnChain.hpp"

class dataFileHandling
{
private:
    string c = to_string(numCubes_);
    string e = to_string(numElem_);
    string r = to_string(runs_);
    string s = to_string(runs_/steps_);
    
public:
    dataFileHandling()
    {
        ofstream chainCoords_file("../res/chainCoords.txt", ios::out | ios::trunc);
        chainCoords_file.close();
        //ofstream chainCoords_file2("chainCoords.txt", ios::out | ios::app);
        //chainCoords_file2 << numCubes_ << endl;
        //chainCoords_file2 << "moves" << endl;
        //chainCoords_file2.close();
        ofstream chainEnergy_file("../res/chainEnergy.txt", ios::out | ios::trunc);
        chainEnergy_file.close();
        ofstream dipVecs_first_file("../res/dipVecsCoords_first.txt", ios::out | ios::trunc);
        dipVecs_first_file.close();
        ofstream dipVecs_last_file("../res/dipVecsCoords_last.txt", ios::out | ios::trunc);
        dipVecs_last_file.close();
        ofstream dipVecs_last_py_file("../res/dipVecsCoords_all_c"+c+"_e"+e+"_r"+r+"_s"+s+".txt", ios::out | ios::trunc);
        dipVecs_last_py_file.close();
        ofstream dipMoms_first_file("../res/dipMomsCoords_first.txt", ios::out | ios::trunc);
        dipMoms_first_file.close();
        ofstream dipMoms_last_file("../res/dipMomsCoords_last.txt", ios::out | ios::trunc);
        dipMoms_last_file.close();
        ofstream globPosCoords_file("../res/globPosCoords.txt", ios::out | ios::trunc);
        globPosCoords_file.close();
        /*ofstream oneCube_randMagnMom_first_file("../res/oneCube_randMagnMom_first.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_first_file.close();
        ofstream oneCube_randMagnMom_last_file("../res/oneCube_randMagnMom_last.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_last_file.close();
        ofstream oneCube_randMagnMom_energy_file("../res/oneCube_randMagnMom_energy.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_energy_file.close();
        ofstream oneCube_randMagnMom_coords_file("../res/oneCube_randMagnMom_coords.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_coords_file.close();*/
        
        cout << "dataFiles truncated!" << endl;
    }
    
    ~dataFileHandling(){}

    void writeChainCoords(const magnChain& ch);
    
    void writeChainEnergy(const int& run, const double& energy);

    void writeDipVecsAll(magnChain ch, int run);

    void writeSnapshot(magnChain ch, string snapshot, double energy, double avgAngleCubePos, double avgAngleMagnMom, double avgCenterDist);

    void writeOneCubeSnapshot(cube oneC, string snapshot);
    
    void writeOneCubeEnergy(int mcStep, double energy);

    void writeOneCubeCoords(cube oneC);
};

#endif
