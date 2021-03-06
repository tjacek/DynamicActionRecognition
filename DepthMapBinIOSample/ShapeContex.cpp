#include "StdAfx.h"
#include "ShapeContext3D.h"

DynamicPointCloud * summaryCloud(Action * action){
 // differenceOfGaussian3D(action,false);
 // ActionSummarry summary( action);
  DynamicPointCloud * pointCloud=new DynamicPointCloud();
  //pointCloud->addDepthMap(&summary.variance);  
  pointCloud->addAction(*action);
  pointCloud->normalize();
  return pointCloud;
}

DynamicPointCloud * timeCloud(Action * action){
//  differenceOfGaussian3D(action,false);
  DynamicPointCloud * pointCloud=new DynamicPointCloud();
  pointCloud->addTimeAction(action);
  pointCloud->normalize();
  return pointCloud;
}

Histogram3D * getActionShapeContext( DatasetParametrs params,PointCloud * pointCloud){ 
  Histogram3D * histogram=new Histogram3D(params.rBins,params.thetaBins,params.betaBins,1000.0);
  centerHistogram( histogram, pointCloud);
  histogram->normalize();
  histogram->show();

  return histogram;
}

Histogram3D * getSimpeShapeContext( DatasetParametrs params, Action action){ 
  DynamicPointCloud * pointCloud=summaryCloud(&action);	

  //vector<Point3D> points=pointCloud.getExtremePoints();
  Histogram3D * histogram=new Histogram3D(params.rBins,params.thetaBins,params.betaBins,1000.0);
  centerHistogram( histogram, pointCloud);
  histogram->normalize();
  histogram->show();

  return histogram;
}

void centerHistogram(Histogram3D * histogram,PointCloud * cloud){
  Point3D current=cloud->getCenter();
  addPoints(current, cloud->points, histogram);
}

void extremeHistogram(Histogram3D * histogram,DynamicPointCloud * cloud){
  vector<Point3D> points=cloud->getExtremePoints();
  cout << cloud->points.size() << " \n";
  for(int i=0;i<points.size();i++){
    Point3D point=points.at(i);
	addPoints(point, cloud->points, histogram);
  }
}

void sampleHistogram(Histogram3D * histogram,PointCloud * cloud){
  //vector<Point3D> points= cloud->sample(4000);
  for(int i=0;i<cloud->points.size();i++){
    Point3D point=cloud->points.at(i);
	addPoints(point, cloud->points,histogram);
  }
}

Histogram3D * getShapeContext3D( DatasetParametrs params,Instant instant){
  PointCloud * pointCloud=getPointCloud(instant.at(0));
  Histogram3D * histogram=new Histogram3D(params.rBins,params.thetaBins,params.betaBins,1000.0);
 
  for(int i=1;i<instant.size();i++){
	CDepthMap* depthMap=instant.at(i);
	pointCloud->addDepthMap(depthMap);
  }
  pointCloud->normalize();
  //Point3D center=pointCloud->getCenteroid();
  //  cout << "|| "<< center << "||\n";
  vector<Point3D> points=pointCloud->getExtremePoints();
  for(int i=0;i<points.size();i++){
	Point3D current=points.at(i);
    addPoints(current, pointCloud->points, histogram);
  }
  histogram->show();

  histogram->normalize();
  delete pointCloud;
  return histogram;
}

PointCloud * getPointCloud(CDepthMap * depthMap){
  PointCloud * pointCloud=new PointCloud();
  pointCloud->addDepthMap( depthMap);
 // pointCloud->getCloudDim();
  //pointCloud->normalize();
  return pointCloud;
}

void addPoints(Point3D  centre,vector<Point3D> points,Histogram3D * histogram){
  //cout << "|| "<< centre << "||\n";
  for(int i=0;i<points.size();i++){
    Point3D rawpoint=points.at(i);
    Point3D point= rawpoint - centre;
	double ksi=  L2(point); //log(L2(point));
	double theta=atan2(point.val[1],point.val[0]) + M_PI;
	double x=point.val[0];
	double y=point.val[1];
	//cout << "%" << point.val[2] << " " << sqrt(x*x+y*y) <<"\n";
	double beta=atan2(point.val[2],sqrt(x*x+y*y)) + (M_PI /2);
	histogram->addToHistogram(ksi,theta,beta);
  }
}

Histogram3D::Histogram3D(int rBins,int thetaBins,int betaBins,double r){
  this->rBins=rBins;
  this->thetaBins=thetaBins;
  this->betaBins=betaBins;
  init(r);
}

Histogram3D::Histogram3D(double r){
  rBins=3.0;
  thetaBins=8.0;
  betaBins=4.0;
  init(r);
}

void Histogram3D::init(double r){
  maxValues.val[0]=(sqrt(3.00)/2.0)*1000.0; //log(r)+0.1;
  maxValues.val[1]=2*M_PI + 0.1;
  maxValues.val[2]=M_PI + 0.1;
  bins=new double **[rBins];
  for(int i=0;i<rBins;i++){
	bins[i]=new double *[thetaBins];
	for(int j=0;j<thetaBins;j++){
      bins[i][j]=new double[betaBins];
      for(int k=0;k<betaBins;k++){
        bins[i][j][k]=0; 
	  }
	}
  }
}

void Histogram3D::addToHistogram(double ksi,double theta,double psi){
 // cout << "@ " << ksi <<" "<< theta <<" "<< psi <<"\n";
 // cout << maxValues << "\n";
  int i=getIndex(ksi,rBins,maxValues.val[0]);
  int j=getIndex(theta,thetaBins,maxValues.val[1]);
  int k=getIndex(psi,betaBins,maxValues.val[2]);
 // cout << "i " << i <<" j "<< j <<" k "<< k <<"\n";

  bins[i][j][k]+=1.0;
}

void Histogram3D::normalize(){
  double normalizeConst=0.0;
  for(int i=0;i<rBins;i++){
	for(int j=0;j<thetaBins;j++){
      for(int k=0;k<betaBins;k++){
        normalizeConst+=bins[i][j][k];
	  }
	}
  }
  for(int i=0;i<rBins;i++){
	for(int j=0;j<thetaBins;j++){
      for(int k=0;k<betaBins;k++){
        bins[i][j][k]/=normalizeConst;
	  }
	}
  }
}

vector<double>* Histogram3D::toVector(){
  vector<double>* vect= new vector<double>();
  for(int i=0;i<rBins;i++){
	for(int j=0;j<thetaBins;j++){
	  for(int k=0;k<betaBins;k++){
		vect->push_back(bins[i][j][k]); 
	  }
	}
  }
  return vect;
}

void Histogram3D::show(){
  for(int i=0;i<rBins;i++){
	for(int j=0;j<thetaBins;j++){
	  for(int k=0;k<betaBins;k++){
		cout << bins[i][j][k] << " "; 
	  }
	  cout << "\n";
	}
	cout << "\n \n";
  }
}

int getIndex(double value,double size,double max){
  if(size==0){
	  return 0;
  }
  if(size<=0.0){
	  return 0;
  }
  double step=max/ ((double)size);
  int index=floor(value/step);
  if(index<=0){
    return 0;
  }
  if(index<size){
    return index;
  }else{
//	cout << "Out of bins " << max << " " << value <<"\n";
	return size-1;
  }
}