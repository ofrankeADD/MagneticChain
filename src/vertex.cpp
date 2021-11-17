#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

typedef double coord;

class vec
{
private:

    //coord x, y, z;
    
public:
    coord x, y, z;

    vec()
    {
        x = y = z = coord(0);
    }

    vec(coord initX, coord initY, coord initZ)
    {
        x = initX; y = initY; z = initZ;
    }

    ~vec(){}

    //vec+vec
    vec operator +(const vec& v)
    {
        return vec(x + v.x, y + v.y, z + v.z);
    }

    //vec-vec
    vec operator -(const vec& v)
    {
        return vec(x - v.x, y - v.y, z - v.z);
    }

    //vec*scalar
    vec operator *(const coord& scalar)
    {
        return vec(x*scalar, y*scalar, z*scalar);
    }
    
    //vec*vec scalarproduct
    coord operator *(const vec& v)
    {
        return coord(x*v.x + y*v.y + z*v.z);
    }
    
    //vec%vec vectorproduct
    vec operator %(const vec& v)
    {
        return vec(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    
    //vec==vec
    bool operator ==(const vec& v)
    {
        if(x == v.x && y == v.y && z == v.z)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    //vec!=vec
    bool operator !=(const vec& v)
    {
        if(x != v.x || y != v.y || z != v.z)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    //vec+=vec
    /*vec operator +=(const vec& v)
    {
        return vec(x,y,z)+v;
    }*/
    
    //vecLength
    coord vecLen()
    {
        return coord(sqrt(x*x + y*y + z*z));
    }
    
    //vecNormalized
    vec vecNorm()
    {
        return vec(x/vecLen(), y/vecLen(), z/vecLen());
    }

    friend ostream& operator<<(ostream &stream, vec v);
};

inline ostream& operator <<(ostream& stream, vec v)
{
    return stream << "(" << v.x << "," << v.y << "," << v.z << ")";
}

#endif
