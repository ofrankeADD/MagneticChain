#include "metropolis.hpp"
using namespace std;

void metropolisMonteCarloAlgorithm(magnChain& chain, dataFileHandling& dataFileIO, double& angleCubePos, double& angleMagnMom)
{
    double deltaEnergy, prob, yEnergy, xEnergy;
    int a2 = 0, b2 = 0, c2 = 0; 
    int avgCube = (numCubes_*0.5);

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

	//-------------------------Metropolis Monte Carlo Algorithm for magnetic chain----------------------
    cout << "Monte Carlo steps for magnetic chain <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    
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
    
    int count2 = a2+b2+c2;
    cout << "a2=" << a2 << "(" << 100*a2/(double)count2 << "%) b2=" << b2 << "(" << 100*b2/(double)count2 << "%) c2=" << c2 << "(" << 100*c2/(double)count2 << "%)" << endl;
    
    return;
}

void metropolisInitialConditions(magnChain& chain, dataFileHandling& dataFileIO, double& startenergy)
{
	double y = edgeLength_*0.1;
    
    vec m_x(1,0,0);
    vec m_y(0,1,0);
    vec m_z(0,0,1);

	cout << "set cubes lined on y-axis with fixed distance: <<<<<<<<<<<<<<<<<<" << endl;
    chain.initCubesYaxis(nullVec_,nullVec_,0.0);
    vec startPosVec = vec(0,y,0);
    cout << "startPosVec=" << startPosVec << endl;
    chain.initCubesYaxis(startPosVec, m_y, r_+ edgeLength_);
    
    startenergy = chain.chainInteract();
    cout << "Energy=" << startenergy << endl;
    dataFileIO.writeSnapshot(chain, "first", startenergy, 0.0, 0.0, r_+ edgeLength_);
}

void metropolisOutputResults(magnChain& chain, dataFileHandling& dataFileIO, double& angleCubePos, double& angleMagnMom, double& startenergy)
{
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
    
    double endenergy = chain.chainInteract();
    cout << "startenergy = " << startenergy << ", endenergy = " << endenergy << endl;
    dataFileIO.writeSnapshot(chain, "last", endenergy, angleCubePos/(double)steps_, angleMagnMom/(double)steps_, avgCenterDist);
    
    return;
}

