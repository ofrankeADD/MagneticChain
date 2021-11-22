#include "cube.hpp"
using namespace std;
    
void cube::initDipVecs()
{
    dipPos = (vec(0,0,0)+vec(0,0,-1*dipLength)+vec(0,-1*dipLength,0)+vec(dipLength,0,0))*coord(0.5);
    dipPos = dipPos*coord(numElem_);//-1
    vec vi, vrow, vcol;
    vi = vrow = vcol = dipPos;
    int pos = 0;
    int posi = 0;
    //cout << dipPos << " " << dipPos.vecLen() << endl;//" " << dipDiag/2.0*(numElem_-1) << endl;
    
    for(int i = 0; i < numElem_; i++)
    {
        vi = vi+(vec(-1.0*dipLength,0,0)*coord(i));
        
        for(int row = 0; row < numElem_; row++)
        {
            vrow = vi+(vec(0,0,dipLength)*coord(row));
            
            for(int col = 0; col < numElem_; col++)
            {
                pos = row*numElem_+col;
                posi = pos+i*numElem_*numElem_;
                
                vcol = vrow+(vec(0,dipLength,0)*coord(col));
                
                dipArray[posi] = dipole(posi,initMom,vcol);
                //cout << dipArray[posi] << endl;//" " << dipArray[posi].dipVec.vecLength() << endl;
                
                vcol = dipPos;
            }
            vrow = dipPos;
        }
        vi = dipPos;
    }
}

//issues due to precision of rounding
void cube::sphereCoords()
{
    if(globalPos == nullVec_)
    {
        cphi = 0.0;
        ctheta = 0.0;
    }
    else if(globalPos.x == 0 && globalPos.y == 0)
    {
        cphi = 0.0;
        ctheta = acos(globalPos.z/globalPos.vecLen());
    }
    else
    {
        ctheta = acos(globalPos.z/globalPos.vecLen());
        
        if(globalPos.x > 0)
        {
            cphi = atan(globalPos.y/globalPos.x);
        }
        else if(globalPos.x == 0)
        {
            if(globalPos.y < 0)
            {
                cphi = -0.5*PI_;
            }
            else if(globalPos.y == 0)
            {
                cphi = 0.0;
            }
            else if(globalPos.y > 0)
            {
                cphi = 0.5*PI_;
            }
        }
        else if(globalPos.x < 0 && globalPos.y >= 0)
        {
            cphi = atan(globalPos.y/globalPos.x) + PI_;
        }
        else if(globalPos.x < 0 && globalPos.y < 0)
        {
            cphi = atan(globalPos.y/globalPos.x) - PI_;
        }
    }
}

