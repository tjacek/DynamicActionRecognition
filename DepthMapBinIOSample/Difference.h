#pragma once
#include "stdafx.h"
#include "DepthMap.h"

class ActionBackground{
  public:
    int rows;
    int cols;
	int frames;
	double *** data;
  public:
    ActionBackground(int frames,int rows,int cols);
};

class Kernel{
  public:
	virtual double eval(int x,int y,int z)=0;
};

class ExpKernel:public Kernel{
  public:
    double sigmaX;
	double sigmaY;
	double sigmaZ;

	ExpKernel( double sigmaX,double sigmaY,double sigmaZ);
    double eval(int x,int y,int t);
};

extern double weightedSum(int x0,int y0,Action action,Kernel * kernel);