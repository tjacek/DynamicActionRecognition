#include "stdafx.h"
#include "PointCloud.h"

class Histogram3D{
  public:
    double *** bins;
    int rBins;
    int thetaBins;
	int betaBins;
	Point3D maxValues;
  
    Histogram3D(double r);
    void addToHistogram(double ksi,double theta,double psi);
    void normalize();
    void show();
    vector<double>* toVector();
};

extern Histogram3D *  getShapeContext3D(Instant instant);
extern void addPoints(Point3D centre,vector<Point3D> points,Histogram3D * histogram);
extern PointCloud * getPointCloud(CDepthMap * depthMap);

extern int getIndex(double value,double size,double max);