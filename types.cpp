
#include "types.h"

#include <math.h>
#include <stdio.h>

void print(values_t& v)
{
    for(unsigned int i=0; i<v.size(); i++)
        printf("%.5f ", v[i]);
    printf("\n");
}


double abs(const values_t& a)
{
    double sum = 0.0;
    for(unsigned int i=0; i<a.size(); i++){
        sum += a[i]*a[i];
    }
    return sqrt(sum);
}

double sum(const values_t& a)
{
    double sum = 0.0;
    for(unsigned int i=0; i<a.size(); i++){
        sum += a[i];
    }
    return sum;
}

//NUMBER/VECTOR ARITHMETIC

values_t operator+(const double a, const values_t& b)
{
    values_t result(b.size());
    for(unsigned int i=0; i<b.size(); i++){
        result[i] = a+b[i];
    }
    return result;
}

values_t operator-(const double a, const values_t& b)
{
    values_t result(b.size());
    for(unsigned int i=0; i<b.size(); i++){
        result[i] = a-b[i];
    }
    return result;
}

values_t operator*(const double a, const values_t& b)
{
    values_t result(b.size());
    for(unsigned int i=0; i<b.size(); i++){
        result[i] = a*b[i];
    }
    return result;
}

values_t operator/(const double a, const values_t& b)
{
    values_t result(b.size());
    for(unsigned int i=0; i<b.size(); i++){
        result[i] = a/b[i];
    }
    return result;
}

values_t operator+(const values_t& b, const double a)
{
    return operator+(a,b);
}
values_t operator-(const values_t& b, const double a)
{
    return operator-(a,b);
}
values_t operator*(const values_t& b, const double a)
{
    return operator*(a,b);
}
values_t operator/(const values_t& b, const double a)
{
    return operator/(a,b);
}

//NORMAL ARITHMETIC

values_t operator+(const values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    values_t result(a.size());
    for(unsigned int i=0; i<a.size(); i++){
        result[i] = a[i]+b[i];
    }
    return result;
}

values_t operator-(const values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    values_t result(a.size());
    for(unsigned int i=0; i<a.size(); i++){
        result[i] = a[i]-b[i];
    }
    return result;
}

values_t operator*(const values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    values_t result(a.size());
    for(unsigned int i=0; i<a.size(); i++){
        result[i] = a[i]*b[i];
    }
    return result;
}

values_t operator/(const values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    values_t result(a.size());
    for(unsigned int i=0; i<a.size(); i++){
        result[i] = a[i]/b[i];
    }
    return result;
}



// ASSIGNMENT*ARITHMETIC
values_t& operator+=(values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    for(unsigned int i=0; i<a.size(); i++){
        a[i] += b[i];
    }
    return a;
}

values_t& operator-=(values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    for(unsigned int i=0; i<a.size(); i++){
        a[i] -= b[i];
    }
    return a;
}

values_t& operator*=(values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    for(unsigned int i=0; i<a.size(); i++){
        a[i] *= b[i];
    }
    return a;
}

values_t& operator/=(values_t& a, const values_t& b)
{
#ifndef OPTIMIZED
    if(a.size() != b.size())
        throw "Vector addition error: wrong sizes!";
#endif
    for(unsigned int i=0; i<a.size(); i++){
        a[i] /= b[i];
    }
    return a;
}
