#include "StdAfx.h"
#include "PointCloud.h"

PointCloud::PointCloud(){}

void PointCloud::addDepthMap(CDepthMap * depthMap){
  int height=depthMap->GetNRows();
  int width=depthMap->GetNCols();
  for(int i=0;i<height;i++){
	for(int j=0;j<width;j++){
	  double z= depthMap->GetItem(i,j);
	  if(z!= emptyPoint ){
		Point3D point=getPoint(i,j,z,height,width);
	    points.push_back(point);
	  }
    } 
  }
}

Point3D PointCloud::getCloudDim(){
  pair<Point3D, Point3D> extremes=computeExtremes();
  min=extremes.first;
  max=extremes.second;
  cloudDim=max-min;
  return cloudDim;
}

vector<Point3D> PointCloud::getExtremePoints(){
  vector<Point3D> points;
  //points.push_back(center);
  Point3D gemetryCenter=1000* (cloudDim/( r()));
 //cout << gemetryCenter << " " << cloudDim <<"\n";
  //points.push_back(gemetryCenter);
  for(int i=0;i<8;i++){
	Point3D newPoint(0,0,0);
	if((i%2) ==0){
	  newPoint.val[0]=gemetryCenter.val[0];
	}
	int r=i/2;
	if((r%2) ==0){
	  newPoint.val[1]=gemetryCenter.val[1];
	}
    r=i/4;
	if((r%2) ==0){
	  newPoint.val[2]=gemetryCenter.val[2];
	}
	points.push_back(newPoint);
  }
  return points;
}

void PointCloud::normalize(){
  getCloudDim();
  for(int i=0;i<points.size();i++){
    Point3D * current=&points.at(i);
     for(int j=0;j<3;j++){
	  current->val[j]-=min.val[j];
	 }
  }
  double r=L2(cloudDim);
  for(int i=0;i<points.size();i++){
    Point3D * current=&points.at(i);
	for(int j=0;j<3;j++){
	  current->val[j]/=r;
	  current->val[j]*=1000.0;
	}
  }
  cloudDim*=1000;
}

void PointCloud::addAction(vector<CDepthMap *> action){
  for(int i=0;i<action.size();i++){
	  addDepthMap(action.at(i));
  }
}


double PointCloud::r(){
	return L2(cloudDim);
}

Point3D PointCloud::getCenteroid(){
  Point3D centroid;
  centroid.zeros();
  double size=(double) points.size();
  for(int i=0;i<points.size();i++){
    Point3D current=points.at(i);
	for(int j=0;j<3;j++){
		centroid.val[j]+=current.val[j];
	}
  }
  for(int j=0;j<3;j++){
	centroid.val[j]/=size;
  } 
  return centroid;
}


Point3D PointCloud::getCenter(){
  center=cloudDim/2;
  return center;
}

void PointCloud::save(string name){
 ofstream myfile;
 myfile.open (name);
 for(int i=0;i<points.size();i++){
   Point3D p=points.at(i);
   myfile << p.val[0]<< " "<<p.val[1] << " " << p.val[2] <<"\n";
  }
  myfile.close();
}

void PointCloud::show(){
  for(int i=0;i<points.size();i++){
    if(i % 100 ==0 ){
     cout << points.at(i) << endl;
	}
  }
}

pair<Point3D, Point3D> PointCloud::computeExtremes(){
  pair<Point3D, Point3D> extremes;
  Point3D minV(99999999,99999999999,9999999999);
  Point3D maxV(-9999999,-9999999999,-999999999);

  for(int i=0;i<points.size();i++){
    Point3D current=points.at(i);
	for(int j=0;j<3;j++){
	  if(current.val[j]< minV.val[j]){
		minV.val[j]=current.val[j];
	  }
	}
	for(int j=0;j<3;j++){
	  if(current.val[j]> maxV.val[j]){
		maxV.val[j]=current.val[j];
	  }
	}
  }

  extremes.first =minV;
  extremes.second=maxV;
  return extremes;
}

Point3D getPoint(double depthX,double depthY,double depthZ,double resolutionX,double resolutionY){
  double horizontalFov = 58.5;   
  double verticalFov   = 45.6;  
  double xzFactor = tan( (horizontalFov /2) * ( M_PI /180)) * 2;
  double yzFactor = tan( (verticalFov /2)   * (M_PI/180)) * 2;
  double normalizedX = (depthX / resolutionX)- 0.5;
  double normalizedY = 0.5 - (depthY / resolutionY);
  Point3D point;
  point.val[0] = normalizedX * depthZ * xzFactor;
  point.val[1] = normalizedY * depthZ * yzFactor;
  point.val[2] = depthZ;
  return point;
}

double L2(Point3D point){
  double x=point.val[0];
  double y=point.val[1];
  double z=point.val[2];
  return sqrt(x*x+y*y+z*z);
}