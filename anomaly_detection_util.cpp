///*
// * anomaly_detection_util.cpp
// *
// * Author: Shira Goren 207814989 with Roi Avraham 318778081
// */
//
//
#include "anomaly_detection_util.h"
#include <math.h>
#include <cstdlib>
#include <iostream>

float avg(float* x, int size){
    float sum=0;
    for(int i=0;i<size;i++) {
        sum+=x[i];
    }
    return sum/size;
}

// returns the variance of X and Y
float var(float* x, int size){
    float average =avg(x,size);
    float sum=0;
    for(int i=0;i<size;i++){
        sum+=x[i]*x[i];
    }
    float variance = sum/size - average*average;
    return variance;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avgXy=0;
    for(int i=0;i<size;i++){
        avgXy+=x[i]*y[i];
    }
    avgXy/=size;

    float covariance = avgXy - avg(x,size)*avg(y,size);
    return covariance;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    float covariance = cov(x, y, size);
    float sigmaX = float (sqrt(double(var(x, size))));
    float sigmaY = float (sqrt(double(var(y, size))));
    float p = covariance / (sigmaX * sigmaY);
    return p;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float* x = new float[size];
    float* y = new float[size];
    float a = 0.0;
    float b = 0.0;

    /*
     * loop to create array with all x values
     * and array with all y values
     * */
    for(int i = 0;i< size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size) / var(x, size);
    float aveX = avg(x,size);
    float aveY = avg(y, size);
    b = aveY - (a * aveX);
    Line l = Line(a, b);
    return l;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line l=linear_reg(points,size);
    return dev(p,l);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float fX = l.f(p.x);
    float deviation = fX - p.y;
    if (deviation < 0) {
        deviation = (-1) * deviation;
     }
     return deviation;
}





