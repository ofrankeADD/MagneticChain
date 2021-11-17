#ifndef DATAFILEHANDLING_H
#define DATAFILEHANDLING_H

#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;
#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.cpp"
#include "constants.h"
#include "magnChain.cpp"

class dataFileHandling
{
private:
    
public:
    dataFileHandling()
    {
        ofstream chainCoords_file("./res/chainCoords.txt", ios::out | ios::trunc);
        chainCoords_file.close();
        //ofstream chainCoords_file2("chainCoords.txt", ios::out | ios::app);
        //chainCoords_file2 << numCubes_ << endl;
        //chainCoords_file2 << "moves" << endl;
        //chainCoords_file2.close();
        ofstream chainEnergy_file("./res/chainEnergy.txt", ios::out | ios::trunc);
        chainEnergy_file.close();
        ofstream dipVecs_first_file("./res/dipVecsCoords_first.txt", ios::out | ios::trunc);
        dipVecs_first_file.close();
        ofstream dipVecs_last_file("./res/dipVecsCoords_last.txt", ios::out | ios::trunc);
        dipVecs_last_file.close();
        ofstream dipVecs_last_py_file("./res/py/dipVecsCoords_last_py.txt", ios::out | ios::trunc);
        dipVecs_last_py_file.close();
        ofstream dipMoms_first_file("./res/dipMomsCoords_first.txt", ios::out | ios::trunc);
        dipMoms_first_file.close();
        ofstream dipMoms_last_file("./res/dipMomsCoords_last.txt", ios::out | ios::trunc);
        dipMoms_last_file.close();
        ofstream globPosCoords_file("./res/globPosCoords.txt", ios::out | ios::trunc);
        globPosCoords_file.close();
        /*ofstream oneCube_randMagnMom_first_file("./res/oneCube_randMagnMom_first.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_first_file.close();
        ofstream oneCube_randMagnMom_last_file("./res/oneCube_randMagnMom_last.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_last_file.close();
        ofstream oneCube_randMagnMom_energy_file("./res/oneCube_randMagnMom_energy.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_energy_file.close();
        ofstream oneCube_randMagnMom_coords_file("./res/oneCube_randMagnMom_coords.txt", ios::out | ios::trunc);
        oneCube_randMagnMom_coords_file.close();*/
        
        cout << "dataFiles truncated!" << endl;
    }
    
    ~dataFileHandling(){}

    void writeChainCoords(const magnChain& ch)
    {
        ofstream chainCoords_file("./res/chainCoords.txt", ios::out | ios::app);
        chainCoords_file << numCubes_ << endl;
        chainCoords_file << "moves" << endl;
        //chainCoords_file << "#i" << '\t' << "x" << '\t' << "y" << '\t' << "z" << endl;
        
        for(int i = 0; i < numCubes_; i++)
        {
            chainCoords_file << ch.cubeArray[i].globalPos.x << '\t' << ch.cubeArray[i].globalPos.y << '\t' << ch.cubeArray[i].globalPos.z << endl;
        }
        chainCoords_file << endl;
        chainCoords_file << endl;
        chainCoords_file.close();
    }
    
    void writeChainEnergy(const int& run, const double& energy)
    {
        ofstream chainEnergy_file("./res/chainEnergy.txt", ios::out | ios::app);
        //chainCoords_file << numCubes_ << endl;
        //chainCoords_file << "moves" << endl;
        chainEnergy_file << run << '\t' << energy << endl;

        chainEnergy_file.close();
    }
    
    void snapshot(magnChain ch, string snapshot, double energy, double avgAngleCubePos, double avgAngleMagnMom, double avgCenterDist)
    {
        string txtFile = "./res/dipVecsCoords_" + snapshot + ".txt";
        string txtFile_py = "./res/py/dipVecsCoords_" + snapshot + "_py.txt";
        ofstream dipVecsCoords_file(txtFile, ios::out | ios::app);
        ofstream dipVecsCoords_py_file(txtFile_py, ios::out | ios::app);
        dipVecsCoords_file << "#energy" << '\t' << "avgAngleCubePos" << '\t' << "avgAngleMagnMom" << '\t' << "avgCenterDist" << endl;
        dipVecsCoords_file << "#" << energy << '\t' << avgAngleCubePos << '\t' << avgAngleMagnMom << '\t' << avgCenterDist << endl;
        dipVecsCoords_file << endl;
        dipVecsCoords_file << endl;
        dipVecsCoords_file << "#x" << '\t' << "y" << '\t' << "z" << endl;
        
        for(int i = 0; i < numCubes_; i++)
        {
            for(int j = 0; j < totalSize_; j++)
            {
                vec v = ch.cubeArray[i].globalPos + ch.cubeArray[i].dipArray[j].dipVec;
                dipVecsCoords_file << v.x << '\t' << v.y << '\t' << v.z << endl;
                dipVecsCoords_py_file << j << '\t' << v.x << '\t' << v.y << '\t' << v.z << endl;
            }
            dipVecsCoords_file << endl;
            dipVecsCoords_file << endl;
            dipVecsCoords_py_file << endl;
            dipVecsCoords_py_file << endl;
        }
        dipVecsCoords_file.close();
        dipVecsCoords_py_file.close();
        
        ofstream globPosCoords_file("./res/globPosCoords.txt", ios::out | ios::app);
        globPosCoords_file << "x" << '\t' << "y" << '\t' << "z" << endl;
        
        for(int i = 0; i < numCubes_; i++)
        {
            vec v = ch.cubeArray[i].globalPos;
            globPosCoords_file << v.x << '\t' << v.y << '\t' << v.z << endl;
        }
        globPosCoords_file << endl;
        globPosCoords_file << endl;
        globPosCoords_file.close();

        string txtFile2 = "./res/dipMomsCoords_" + snapshot + ".txt";
        ofstream dipMomsCoords_file(txtFile2, ios::out | ios::app);
        dipMomsCoords_file << "#x" << '\t' << "y" << '\t' << "z" << endl;
        
        for(int i = 0; i < numCubes_; i++)
        {
            for(int j = 0; j < totalSize_; j++)
            {
                vec v = ch.cubeArray[i].dipArray[j].dipMagnMom;
                dipMomsCoords_file << v.x << '\t' << v.y << '\t' << v.z << endl;
            }
            dipMomsCoords_file << endl;
            dipMomsCoords_file << endl;
        }
        dipMomsCoords_file.close();
    }

    void writeOneCubeSnapshot(cube oneC, string snapshot)
    {
        string txtFile = "./res/oneCube_randMagnMom_" + snapshot + ".txt";
        ofstream oneCube_randMagnMom_file(txtFile, ios::out | ios::app);
        oneCube_randMagnMom_file << "#rx" << '\t' << "ry" << '\t' << "rz" << '\t' << "mx" << '\t' << "my" << '\t' << "mz" << endl;
        
        for(int i = 0; i < totalSize_; i++)
        {
            vec v = oneC.dipArray[i].dipVec;
            vec m = oneC.dipArray[i].dipMagnMom;
            oneCube_randMagnMom_file << v.x << '\t' << v.y << '\t' << v.z << '\t' << m.x << '\t' << m.y << '\t' << m.z <<endl;
        }
        
        oneCube_randMagnMom_file << endl;
        oneCube_randMagnMom_file << endl;
        oneCube_randMagnMom_file.close();
    }
    
    void writeOneCubeEnergy(int mcStep, double energy)
    {
        ofstream oneCube_randMagnMom_energy_file("./res/oneCube_randMagnMom_energy.txt", ios::out | ios::app);
        oneCube_randMagnMom_energy_file << mcStep << '\t' << energy << endl;
        oneCube_randMagnMom_energy_file.close();
    }

    void writeOneCubeCoords(cube oneC)
    {
        ofstream oneCube_randMagnMom_coords_file("./res/oneCube_randMagnMom_coords.txt", ios::out | ios::app);
        
        for(int i = 0; i < totalSize_; i++)
        {
            vec m = oneC.dipArray[i].dipMagnMom;
            oneCube_randMagnMom_coords_file << m.x << '\t' << m.y << '\t' << m.z <<endl;
        }
        
        oneCube_randMagnMom_coords_file << endl;
        oneCube_randMagnMom_coords_file << endl;
        oneCube_randMagnMom_coords_file.close();
    }
};

#endif
