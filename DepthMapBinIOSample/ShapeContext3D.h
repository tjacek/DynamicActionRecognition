#pragma once

#include "stdafx.h"
#include "PointCloud.h"
#include "Dataset.h"
#include "Difference.h"

class Histogram3D{
  public:
    double *** bins;
    int rBins;
    int thetaBins;
	int betaBins;
	Point3D maxValues;
  
    Histogram3D(double r);
	Histogram3D(int rBins,int thetaBins,int betaBins,double r);
    void addToHistogram(double ksi,double theta,double psi);
    void normalize();
    void show();
    vector<double>* toVector();
  private:
    void init(double r);
};

extern Histogram3D *  getShapeContext3D( DatasetParametrs params,Instant instant);
extern void addPoints(Point3D centre,vector<Point3D> points,Histogram3D * histogram);
extern PointCloud * getPointCloud(CDepthMap * depthMap);

extern int getIndex(double value,double size,double max);
extern Histogram3D * getSimpeShapeContext( DatasetParametrs params, Action action);

void centerHistogram(Histogram3D * histogram,DynamicPointCloud * cloud);
void extremeHistogram(Histogram3D * histogram,DynamicPointCloud * cloud);
void sampleHistogram(Histogram3D * histogram,DynamicPointCloud * cloud);