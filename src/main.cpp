#include <iostream>
#include <stdio.h>
#include <random>
#include <math.h>
#include <time.h>
using namespace std;
#include "vertex.cpp"
#include "dipole.cpp"
#include "cube.cpp"
#include "magnChain.cpp"
#include "constants.h"
#include "dataFileHandling.cpp"
#include "oneCube.cpp"

int main()
{
    //clock_t start_c;
    time_t start_t, end_t;
    double sec_t;

    magnChain chain(numCubes_, edgeLength_);
    cout << "edgeLength: " << edgeLength_ << endl;
    double y = edgeLength_*0.1;
    
    vec m_x(1,0,0);
    vec m_y(0,1,0);
    vec m_z(0,0,1);

    double deltaEnergy, prob, yEnergy, xEnergy, energy, startenergy;
    int a2 = 0, b2 = 0, c2 = 0; 
    int avgCube = (numCubes_*0.5);
    double angleCubePos = 0.0, angleMagnMom = 0.0;
    
    dataFileHandling dataFileIO;
    
    //Mersenne Twister:
    random_device rng;
    uniform_real_distribution<double> randZ(-1.0, 1.0);
    uniform_real_distribution<double> randTheta(0.0, PI_*2.0);
    uniform_real_distribution<double> randProb(0.0, 1.0);
    //uniform_real_distribution<double> randDist(0.0, edgeLength_*numCubes_);//((edgeLength/(double)numCubes_), edgeLength);
    uniform_real_distribution<double> randDist(0.0, r_);
    uniform_int_distribution<int> randCube(0, numCubes_-1);
    uniform_int_distribution<int> randElem(0, totalSize_-1);
    uniform_real_distribution<double> randPhi(0.0, PI_*0.05);//9°
    mt19937 engine(rng());    
    
    //printf("PI %f, 180 %f, 2PI %f, 360 %f, PI/2 %f, 90 %f\n", cos(PI_), cos(180), cos(2*PI_), cos(360), cos(PI_/2.0), cos(90));
    //cout << cos(2*PI_ - PI_*0.05) << endl;
    //cout << acos(0.8) << endl;
    //for(int i = 0; i < 10; i++){double rotPhi = 2*PI_ - randPhi(engine);printf("rotPhi %f\n", rotPhi);}

    cout << "set cubes lined on y-axis with fixed distance: <<<<<<<<<<<<<<<<<<" << endl;
    chain.initCubesYaxis(nullVec_,nullVec_,0.0);
    //vec randPosVec(randVec(engine), randVec(engine), randVec(engine));
    vec startPosVec = vec(0,y,0);
    cout << "startPosVec=" << startPosVec << endl;
    chain.initCubesYaxis(startPosVec, m_y, r_+ edgeLength_);
    
    //dataFileIO.writeChainCoords(chain);
    startenergy = chain.chainInteract();
    cout << "Energy=" << startenergy << endl;
    dataFileIO.writeSnapshot(chain, "first", startenergy, 0.0, 0.0, r_+ edgeLength_);
    
    cout << "Metropolis Method: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    //Metropolis Method:
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
     //----------------------------------Monte Carlo steps for magnetic chain----------------------------
     cout << "Monte Carlo steps for magnetic chain <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    //start_c = clock();
    time(&start_t);

    for(int i = 0; i < runs_; i++)
    {
        if(i%steps_ == 0)
        {
            cout << "run: " << i << endl;
            dataFileIO.writeChainCoords(chain);
            dataFileIO.writeChainEnergy(i, chain.chainInteract());
            chain.timeAverage(avgCube, angleCubePos, angleMagnMom);
            dataFileIO.writeDipVecsAll(chain, i);
        }
        
        
        int cube_idx = randCube(engine);
        cube cx = chain.cubeArray[cube_idx];
        //cout << "cx: " << cx << endl;
        xEnergy = chain.chainInteract();

        double xRand = sqrt(1-pow(randZ(engine),2))*cos(randTheta(engine));
        double yRand = sqrt(1-pow(randZ(engine),2))*sin(randTheta(engine));
        double zRand = randTheta(engine);
        double transProb = randProb(engine);
        cube cy;
        if(transProb <= 0.5)
        {   
            vec rotVec = vec(xRand, yRand, zRand);
            double rotPhi = randPhi(engine);
            double rotProb = randProb(engine);
            if(rotProb <= 0.5)
            {
                rotPhi = rotPhi;
            }
            else
            {
                rotPhi = 2.0*PI_ - rotPhi;
            }
            //cout << "rotVec=" << rotVec << ", r_=" << r_ << ", rotPhi=" << rotPhi << ", qProb=" << qProb << endl;
            cy = chain.rotateSelf(cube_idx, rotVec, rotPhi);
        }
        else
        {
            vec transVec = vec(xRand, yRand, zRand);
            double transDist = randDist(engine);
            cy = chain.translateCube(cube_idx, transVec*transDist);
        }

        //cout << "cy: " << cy << endl;
        chain.cubeArray[cube_idx] = cy;
        yEnergy = chain.chainInteract();
        
        deltaEnergy = yEnergy - xEnergy;
        //cout << "deltaEnergy=" << deltaEnergy << " , " << deltaEnergy/(kB_*T_) << endl;
        prob = min(1.0, exp(-1.0*deltaEnergy/(kB_*T_)));
        double qProb = randProb(engine);
        //cout << "prob=" << prob << endl;
        if(deltaEnergy <= 0)
        {
            chain.cubeArray[cube_idx] = cy;
            a2++;
        }
        else
        {
            if(qProb < prob)
            {
                chain.cubeArray[cube_idx] = cy;
                b2++;
            }
            else
            {
                chain.cubeArray[cube_idx] = cx;
                c2++;
            }
        }
    }
    //sec_c = ((double)(clock() - start_c))/CLOCKS_PER_SEC;
    time(&end_t);
    sec_t = (double)(end_t - start_t);
    cout << "Simulation took " << sec_t << " seconds." << endl; 
    
    int count2 = a2+b2+c2;
    cout << "a2=" << a2 << "(" << 100*a2/(double)count2 << "%) b2=" << b2 << "(" << 100*b2/(double)count2 << "%) c2=" << c2 << "(" << 100*c2/(double)count2 << "%)" << endl;
    
    cout << "time average of angle between cube.globalPos: " << angleCubePos/(double)steps_ << "°" << endl;
    cout << "time average of angle between cube.dipMagnMom: " << angleMagnMom/(double)steps_ << "°" << endl;
    
    vec temp = chain.cubeArray[0].globalPos;
    double centerDist = 0.0;
    for(int i = 1; i < numCubes_; i++)
    {
        vec v = chain.cubeArray[i].globalPos;
        centerDist += (temp - v).vecLen();
        temp = v;
    }
    double avgCenterDist = centerDist/double(numCubes_ - 1);
    cout << "avg center-center-dist of the chain for last snapshot: (r_+edgeLength_)*" << 1+((avgCenterDist-(r_+edgeLength_))/(r_+edgeLength_)) << endl;
    
    energy = chain.chainInteract();
    cout << "startenergy = " << startenergy << ", endenergy = " << energy << endl;
    dataFileIO.writeSnapshot(chain, "last", energy, angleCubePos/(double)steps_, angleMagnMom/(double)steps_, avgCenterDist);
    
    return 0;
}

/*double MCstep = randProb(engine);

//change cubeVec randomly
if(MCstep <= 0.5)
{
    int Cube = randCube(engine);
    cube cx = chain.cubeArray[Cube];
    vec xVec = chain.cubeArray[Cube].globalPos;
    
    double Econstr = chain.translateCube(Cube, nullVec_);
    xEnergy = chain.chainInteract() + Econstr;
    
    double xRand = randVec(engine);
    double yRand = randVec(engine);
    double zRand = randVec(engine);
    vec qVec = vec(xRand, yRand, zRand);
    double qProb = randProb(engine);
    //cout << "qVec=" << qVec << ", r_=" << r_ << ", qProb=" << qProb << endl;
    
    vec transVec = qVec*r_;
    Econstr = chain.translateCube(Cube, transVec);
    //cout << "transVec=" << transVec << ", Econstr=" << Econstr << endl;
    
    if(Econstr != 0.0)
    {
        chain.cubeArray[Cube].globalPos = xVec + transVec;
        yEnergy = chain.chainInteract() + Econstr;
        
        deltaEnergy = yEnergy - xEnergy;
        //cout << "deltaEnergy=" << deltaEnergy << " , " << deltaEnergy/(kB_*T_) << endl;
        prob = min(1.0, exp(-1.0*deltaEnergy/(kB_*T_)));
        //cout << "prob=" << prob << endl;
        if(deltaEnergy <= 0)
        {
            chain.cubeArray[Cube].globalPos = xVec + transVec;
            a1++;
        }
        else
        {
            if(qProb < prob)
            {
                chain.cubeArray[Cube].globalPos = xVec + transVec;
                b1++;
            }
            else
            {
                chain.cubeArray[Cube].globalPos = xVec;
                c1++;
            }
        }
    }
    else
    {
        //cout << "Econstr == 0.0" << endl;
    }
}
//rotateSelf cube with random axis
else if(MCstep > 0.5)
{
	*/