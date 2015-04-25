#pragma once
#include "stdafx.h"
#include "DepthMap.h"

class Kernel{
  public:
	virtual double eval(int x,int y,int z)=0;
};

class ActionBackground{
  public:
    int rows;
    int cols;
	int frames;
	double *** data;
    void convol(Kernel * kernel,Action * action);
    ActionBackground(Action * action);
	
};

class ExpKernel:public Kernel{
  public:
    double sigmaX;
	double sigmaY;
	double sigmaZ;

	ExpKernel( double sigmaX,double sigmaY,double sigmaZ);
    double eval(int x,int y,int t);
};

extern double weightedSum(int x0,int y0,int t0,Action * action,Kernel * kernel);
extern void differenceOfGaussian3D(Action * action);