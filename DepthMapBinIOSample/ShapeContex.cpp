#include "StdAfx.h"
#include "ShapeContext3D.h"

Histogram3D * getShapeContext3D(Instant instant){
  PointCloud * pointCloud=getPointCloud(instant.at(0));
  Histogram3D * histogram=new Histogram3D(pointCloud->r());
 
  for(int i=1;i<instant.size();i++){
	CDepthMap* depthMap=instant.at(i);
	pointCloud->addDepthMap(depthMap);
  }
  //pointCloud->normalize();
  Point3D center=pointCloud->getCenteroid();
    cout << "|| "<< center << "||\n";

  //cout << center;
  //pointCloud->show();
 // cout << pointCloud->points.size() << "&\n";
  addPoints(center, pointCloud->points, histogram);
  histogram->show();

  histogram->normalize();
  delete pointCloud;
  return histogram;
}

PointCloud * getPointCloud(CDepthMap * depthMap){
  PointCloud * pointCloud=new PointCloud();
  pointCloud->addDepthMap( depthMap);
  //pointCloud->normalize();
  return pointCloud;
}

void addPoints(Point3D  centre,vector<Point3D> points,Histogram3D * histogram){
  cout << "|| "<< centre << "||\n";
  for(int i=0;i<points.size();i++){
    Point3D rawpoint=points.at(i);
    Point3D point= rawpoint - centre;
	double ksi=log(L2(point));
	double theta=atan2(point.val[1],point.val[0]) + M_PI;
	double x=point.val[0];
	double y=point.val[1];
	double beta=atan2(point.val[2],sqrt(x*x+y*y)) + M_PI;
	histogram->addToHistogram(ksi,theta,beta);
  }
}

Histogram3D::Histogram3D(double r){
  this->size=4;
  maxValues.val[0]=log(r)+0.1;
  maxValues.val[1]=2*M_PI + 0.1;
  maxValues.val[2]=2*M_PI + 0.1;
  bins=new double **[size];
  for(int i=0;i<size;i++){
	bins[i]=new double *[size];
	for(int j=0;j<size;j++){
      bins[i][j]=new double[size];
      for(int k=0;k<size;k++){
        bins[i][j][k]=0; 
	  }
	}
  }
}

void Histogram3D::addToHistogram(double ksi,double theta,double psi){
 // cout << "@ " << ksi <<" "<< theta <<" "<< psi <<"\n";
  int i=getIndex(ksi,size,maxValues.val[0]);
  int j=getIndex(theta,size,maxValues.val[1]);
  int k=getIndex(psi,size,maxValues.val[2]);
  bins[i][j][k]+=1.0;
}

void Histogram3D::normalize(){
  double normalizeConst=0.0;
  for(int i=0;i<size;i++){
	for(int j=0;j<size;j++){
      for(int k=0;k<size;k++){
        normalizeConst+=bins[i][j][k];
	  }
	}
  }
  for(int i=0;i<size;i++){
	for(int j=0;j<size;j++){
      for(int k=0;k<size;k++){
        bins[i][j][k]/=normalizeConst;
	  }
	}
  }
}

vector<double>* Histogram3D::toVector(){
  vector<double>* vect= new vector<double>();
  for(int i=0;i<size;i++){
	for(int j=0;j<size;j++){
	  for(int k=0;k<size;k++){
		vect->push_back(bins[i][j][k]); 
	  }
	}
  }
  return vect;
}

void Histogram3D::show(){
  for(int i=0;i<size;i++){
	for(int j=0;j<size;j++){
	  for(int k=0;k<size;k++){
		cout << bins[i][j][k] << " "; 
	  }
	  cout << "\n";
	}
	cout << "\n \n";
  }
}

int getIndex(double value,double size,double max){
  if(value<0.0){
	  return 0;
  }
  double step=max/ ((double)size);
  int index=floor(value/step);
  if(index<size){
    return index;
  }else{
	cout << "Out of bins " << max << " " << value <<"\n";
	return size-1;
  }
}