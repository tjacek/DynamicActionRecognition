#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "PointCloud.h"

class ActionArray{
  public:
    int rows;
    int cols;
	int frames;
	double *** data;
    void convol(double * kernel,ActionArray * action);
    ActionArray(Action * action);
	~ActionArray();
};

class ActionSummary{
  public:
    CDepthMap mean;
    CDepthMap variance;
    
    ActionSummary(ActionArray * action);
	~ActionSummary();
};

typedef CDepthMap * (*Projection)(CDepthMap * dimage,int z_min); 

PointCloud * actionVar(Action * action,Projection projection);
ActionArray *actionDifference(Action * action,Projection projection);
ActionArray * transformAction(Action * action,Projection projection);
double applyKernel(int x_0,int y_0,int t_0,int k,double * kernel,ActionArray * orginal);
double applyKernel2(int x_0,int y_0,int t_0,int k,double * kernel,ActionArray * orginal);

CDepthMap * projectionZX(CDepthMap * mat,int zmin);
CDepthMap * projectionZY(CDepthMap * mat,int zmin);
CDepthMap * projectionXY(CDepthMap * mat,int zmin);

void  showImage(cv::Mat * m,const char * name);
void zero(CDepthMap * dmap);
int getZMin(Action * action);