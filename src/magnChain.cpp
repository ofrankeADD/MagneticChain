#include "magnChain.hpp"
using namespace std;


void magnChain::printChain()
{
    for(int i = 0; i < chainSize; i++)
    {
        cout << cubeArray[i] << endl;
    }
}

void magnChain::initCubesYaxis(vec cubeVec, vec mom, double d)
{
    for(int i = 0; i < chainSize; i++)
    {
        cube c(cubeVec+vec(0,i*d,0), mom, edgeLen);
        //just change y-axis yet
        if(checkVolExclusion(c, i))
        {
            cubeArray[i] = c;
            
            if(mom != nullVec_)
            {
                cout << i << ": " << cubeArray[i].globalPos << endl;
            }
        }
        else
        {
            cout << "couldn't init cube " << i << " at " << c.globalPos << endl;
        }
    }
}

double magnChain::dipInteract(cube& c1, cube& c2)
{
    double result = 0.0;
    vec m1, m2, dist;
    int i, j;
    //int thread_num = 0;
    //omp_set_num_threads(5);//num_threads(4);
    
    #pragma omp parallel for private(j, m1, m2, dist) reduction(+: result)
    for(i = 0; i < totalSize_; i++)
    {
        for(j = 0; j < totalSize_; j++)
        {
        	m1 = c1.dipArray[i].dipMagnMom;
            m2 = c2.dipArray[j].dipMagnMom;
            dist = (c2.globalPos + c2.dipArray[j].dipVec)-(c1.globalPos + c1.dipArray[i].dipVec);
            
            result += ((-1.0*my0_)/(4.0*PI_))*(((m1*dist)*(m2*dist)*3.0)/(pow(dist.vecLen(), 5))-((m1*m2)/pow(dist.vecLen(), 3)));
            
            //printf("thread %i, i %i, j %i\n", omp_get_thread_num(), i, j);
        }
    }
    
    return result;
}

double magnChain::chainInteract()
{
    double resInteract = 0.0;
    int i, j;
    
    //#pragma omp parallel for private(j) reduction(+: resInteract)
    for(i = 0; i < chainSize; i++)
    {
        for(j = chainSize-1; j > i; j--)
        {
            resInteract += dipInteract(cubeArray[i],cubeArray[j]);
            //cout << "resInteract=" << resInteract << endl;
        }
    }

    resInteract *= magnPerDip*magnPerDip;
    return resInteract;
}

bool magnChain::checkVolExclusion(cube& c, const int& Cube)
{
    magnChain chainTemp(numCubes_-1, edgeLen);
    
    for(int i = 0; i < Cube; i++)
    {
        chainTemp.cubeArray[i] = this->cubeArray[i];
    }
    for(int i = Cube; i < numCubes_-1; i++)
    {
        chainTemp.cubeArray[i] = this->cubeArray[i+1];
    }
    
    bool volExclusion = false;
    double minCubeDist, minDipDist;
    int countNullVec = 0;
    
    for(int k = 0; k < chainSize-1; k++)
    {
        if(chainTemp.cubeArray[k].initMom == nullVec_)
        {
            countNullVec++;
        }
    }
    if(countNullVec == chainSize-1)
    {
        cout << "countNullVec == chainSize" << endl;
        return true;
    }
    
    for(int i = 0; i < chainSize-1; i++)
    {
        minCubeDist = vec(chainTemp.cubeArray[i].globalPos - c.globalPos).vecLen();
        //cout << "minCubeDist(" << i << "," << Cube << ")=" << minCubeDist << endl;
        if(minCubeDist >= sqrt(3.0)*(r_+edgeLen))
        {
            //cout << "minCubeDist >= " << sqrt(3)*edgeLen << endl;
            volExclusion = true;
        }
        else if(minCubeDist < (edgeLen+edgeLen/100.0))
        {
            //cout << "minCubeDist < " << edgeLen+edgeLen/100.0 << endl;
            volExclusion = false;
            return false;
        }
        else
        {
            //cout << "minCubeDist in between" << endl;
            for(int j = 0; j < totalSize_; j++)
            {
                for(int k = 0; k < totalSize_; k++)
                {
                    minDipDist = vec(vec(chainTemp.cubeArray[i].globalPos + chainTemp.cubeArray[i].dipArray[j].dipVec)-vec(c.globalPos + c.dipArray[k].dipVec)).vecLen();
                    if(minDipDist < edgeLen/(double)numElem_)//dipLength_)
                    {
                        //cout << "minDipDist < " << edgeLen/numElem_ << endl;
                        volExclusion = false;
                        return false;
                    }
                    else
                    {
                        volExclusion = true;
                    }
                }
            }
        }
    }
    return volExclusion;
}

cube magnChain::rotateSelf(const int &c, vec &axis, double &phi)
{
    //with rotation matrix:
    cube temp = cubeArray[c];
    vec cNorm = axis.vecNorm();
    vec dipVecs, dipMoms, rotDips, rotMoms;

    double oneMinusCos = 1 - cos(phi);
    double sinus = sin(phi);
    double rotMat_x1 = (cNorm.x*cNorm.x)*oneMinusCos + cos(phi);
    double rotMat_x2 = (cNorm.y*cNorm.x)*oneMinusCos + cNorm.z*sinus;
    double rotMat_x3 = (cNorm.z*cNorm.x)*oneMinusCos + cNorm.y*sinus;
    double rotMat_y1 = (cNorm.x*cNorm.y)*oneMinusCos - cNorm.z*sinus;
    double rotMat_y2 = (cNorm.y*cNorm.y)*oneMinusCos + cos(phi);
    double rotMat_y3 = (cNorm.z*cNorm.y)*oneMinusCos + cNorm.x*sinus;
    double rotMat_z1 = (cNorm.x*cNorm.z)*oneMinusCos + cNorm.y*sinus;
    double rotMat_z2 = (cNorm.y*cNorm.z)*oneMinusCos - cNorm.x*sinus;
    double rotMat_z3 = (cNorm.z*cNorm.z)*oneMinusCos + cos(phi);

    for(int i = 0; i < totalSize_; i++)
    {
        dipVecs = temp.dipArray[i].dipVec;
        dipMoms = temp.dipArray[i].dipMagnMom;
        
        rotDips.x = dipVecs.x*rotMat_x1 + dipVecs.y*rotMat_y1 + dipVecs.z*rotMat_z1;
    	rotDips.y = dipVecs.x*rotMat_x2 + dipVecs.y*rotMat_y2 + dipVecs.z*rotMat_z2;
    	rotDips.z = dipVecs.x*rotMat_x3 + dipVecs.y*rotMat_y3 + dipVecs.z*rotMat_z3;

    	rotMoms.x = dipMoms.x*rotMat_x1 + dipMoms.y*rotMat_y1 + dipMoms.z*rotMat_z1;
    	rotMoms.y = dipMoms.x*rotMat_x2 + dipMoms.y*rotMat_y2 + dipMoms.z*rotMat_z2;
    	rotMoms.z = dipMoms.x*rotMat_x3 + dipMoms.y*rotMat_y3 + dipMoms.z*rotMat_z3;

        temp.dipArray[i].dipVec = rotDips;
        temp.dipArray[i].dipMagnMom = rotMoms;
    }

    if(checkVolExclusion(temp, c))
    {
        return temp;
    }
    else
    {
        //cout << "couldn't rotateSelf, volExclusion" << endl;
        return cubeArray[c];
    }
}

cube magnChain::rotateSphere(int c, double theta, double phi)
{
    cube temp = cubeArray[c];
    
    double r = temp.globalPos.vecLen();
    temp.globalPos.x = r*sin(theta)*cos(phi);
    temp.globalPos.y = r*sin(theta)*sin(phi);
    temp.globalPos.z = r*cos(theta);
    temp.sphereCoords();
    
    for(int i = 0; i < totalSize_; i++)
    {
        double rdip = temp.dipArray[i].dipVec.vecLen();
        temp.dipArray[i].dipVec.x = rdip*sin(theta)*cos(phi);
        temp.dipArray[i].dipVec.y = rdip*sin(theta)*sin(phi);
        temp.dipArray[i].dipVec.z = rdip*cos(theta);
        
        double rmom = temp.dipArray[i].dipMagnMom.vecLen();
        temp.dipArray[i].dipMagnMom.x = rmom*sin(theta)*cos(phi);
        temp.dipArray[i].dipMagnMom.y = rmom*sin(theta)*sin(phi);
        temp.dipArray[i].dipMagnMom.z = rmom*cos(theta);
    }
    
    if(checkVolExclusion(temp, c))
    {
        return temp;
    }
    else
    {
        cout << "couldn't rotateSphere, volExclusion" << endl;
        return cubeArray[c];
    }
}

cube magnChain::translateCube(const int &c, const vec &trans)
{
    cube temp = cubeArray[c];
    vec globalTrans = cubeArray[c].globalPos + trans;
    int first, second;
    
    findClosestNeighbours(c, first, second);
    //cout << "c:" << c << ", first:" << first << ", second:" << second << endl;
    
    vec distLeft = globalTrans - cubeArray[first].globalPos;
    vec distRight = cubeArray[second].globalPos - globalTrans;
    
    double deltaLeft = distLeft.vecLen();// - r_;
    double deltaRight = distRight.vecLen();// - r_;
    
    double kappaLeft = kappa_/pow(constrFac_*distLeft.vecLen(), 2);
    double kappaRight = kappa_/pow(constrFac_*distRight.vecLen(), 2);
    this->Econstr = kappaLeft*(deltaLeft*deltaLeft) + kappaRight*(deltaRight*deltaRight);
    
    temp.globalPos = globalTrans;
    
    if((deltaLeft >= 2.0*(r_+edgeLen)) || (deltaRight >= 2.0*(r_+edgeLen)))
    {
        return cubeArray[c];
    }
    
    if(checkVolExclusion(temp, c))
    {
        return temp;
    }
    else
    {
        //cout << "couldn't translate cube, volExclusion" << endl;
        return cubeArray[c];
    }
}

void magnChain::findClosestNeighbours(const int& c, int &firstNearest, int &secondNearest)
{
    if((c == chainSize-1) || (c == 0))
    {
        double tempDist, nearestDist = 100.0;
        for(int i = 0; i < numCubes_; i++)
        {
            if(i != c)
            {
                tempDist = (cubeArray[c].globalPos - cubeArray[i].globalPos).vecLen();
                if(nearestDist > tempDist)
                {
                    nearestDist = tempDist;
                    firstNearest = i;
                    secondNearest = i;
                }
            }
        }
    }
    else
    {
        double tempDist, firstDist = 100.0, secondDist = 100.0;
        for(int i = 0; i < numCubes_; i++)
        {
        	if(i != c)
        	{
            	tempDist = (cubeArray[c].globalPos - cubeArray[i].globalPos).vecLen();

	            if(firstDist > tempDist)
	            {
	            	secondNearest = firstNearest;
	                firstNearest = i;

	                secondDist = firstDist;
	                firstDist = tempDist;
	            }
	            else if(secondDist > tempDist)
	            {
	            	secondNearest = i;
	            	secondDist = tempDist;
	            }
	        }
        }
    }
}

void magnChain::timeAverage(const int& c, double &angleCubePos, double &angleMagnMom)
{
    int left = 0, right = 0;
    findClosestNeighbours(c, left, right);

    vec cubePos = cubeArray[c].globalPos;
    vec vecLeft = cubePos - cubeArray[left].globalPos;
    vec vecRight = cubeArray[right].globalPos - cubePos;
    angleCubePos += acos((vecLeft*vecRight)/(vecLeft.vecLen()*vecRight.vecLen()))*(180.0/PI_);
    
    vec cubeMom = cubeArray[c].dipArray[0].dipMagnMom;
    vec momLeft = cubeArray[left].dipArray[0].dipMagnMom;
    angleMagnMom += acos((momLeft*cubeMom)/(momLeft.vecLen()*cubeMom.vecLen()))*(180.0/PI_);
}

