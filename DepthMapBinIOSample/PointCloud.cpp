#include "StdAfx.h"
#include "PointCloud.h"

PointCloud::PointCloud(){}

void PointCloud::addDepthMap(CDepthMap * depthMap){
  int height=depthMap->GetNRows();
  int width=depthMap->GetNRows();
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

void PointCloud::normalize(){
  pair<Point3D, Point3D> extremes=computeExtremes();
  Point3D min=extremes.first;
  Point3D max=extremes.second;
  for(int i=0;i<points.size();i++){
    Point3D * current=&points.at(i);
     for(int j=0;j<3;j++){
	  current->val[j]-=min.val[j];
	 }
  }
 
  cloudDim=max-min;
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