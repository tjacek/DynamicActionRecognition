#pragma once

#include "stdafx.h"
#include "DepthMap.h"

typedef cv::Vec<double, 3> Point3D;

class PointCloud{
  public:
    vector<Point3D> points;
	Point3D min;
    Point3D max;
	Point3D cloudDim;
	Point3D center;

	PointCloud();
	void addDepthMap(CDepthMap * depthMap);
	void addAction(vector<CDepthMap *> action);
	Point3D getCloudDim();
	void normalize();
	Point3D getCenter();
	Point3D getCenteroid();
	vector<Point3D> getExtremePoints();
	vector<Point3D> sample(int n);
	double r();
	void save(string name);
	void show();
    pair<Point3D, Point3D> computeExtremes();
};

class DynamicPointCloud: public PointCloud{
  map<int,vector<int>> timeFrames;
  public:
    void addTimeFrame(CDepthMap * depthMap);
	void addTimeAction(Action * action);
    vector<Point3D> getPoints(int t);
	vector<Point3D> getFrame(int x,int y);
	~DynamicPointCloud();
};

extern Point3D getPoint(double depthX,double depthY,double depthZ,double resolutionX,double resolutionY);
extern double L2(Point3D point);