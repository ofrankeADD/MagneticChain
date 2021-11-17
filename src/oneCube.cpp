#ifndef ONECUBE_H
#define ONECUBE_H

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
using namespace std;
#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.cpp"
#include "constants.h"

class oneCube
{
private:
    
public:

    double dipInteract(cube oneC)
    {
        double resOneCube = 0.0;
        vec m1, m2, dist;
        
        for(int i = 0; i < totalSize_; i++)
        {
            m1 = oneC.dipArray[i].dipMagnMom;
            
            for(int j = 0; j < totalSize_; j++)
            {
                if(j > i)
                {
                    m2 = oneC.dipArray[j].dipMagnMom;
                    dist = oneC.dipArray[j].dipVec - oneC.dipArray[i].dipVec;
                    resOneCube += ((-1.0*my0_)/(4.0*PI_))*(((m1*dist)*(m2*dist)*3.0)/(pow(dist.vecLen(), 5))-((m1*m2)/pow(dist.vecLen(), 3)));
                }
            }
        }
        resOneCube *= pow(magnPerVol_*pow(edgeLength_/(double)numElem_/2.0, 3), 2);
        //cout << "dipInteract for one cube: " << resOneCube << " , " << resOneCube/(kB_*T_) << endl;
        
        return resOneCube;
    }
    
    cube rotateMagnMom(cube temp, int m, vec axis, double phi)
    {
        vec cNorm = axis.vecNorm();
        vec dipMom = temp.dipArray[m].dipMagnMom;
        vec rotMom = cNorm*(cNorm*dipMom) + ((cNorm % dipMom)%cNorm)*cos(phi) + (cNorm % dipMom)*sin(phi);
        temp.dipArray[m].dipMagnMom = rotMom;
        
        return temp;
    }
    
};
#endif
