#include "StdAfx.h"
#include "Dataset.h"
#include "ShapeContext3D.h"
#include "utils.h"
#include "Difference.h"
#include "ActionVariance.h"

void FeatureExtractor::showTime(){
  cout <<"\n mean: " << mean(actionTime)<< "\n"; 
  cout <<"\n sd: " << standardDeviation(actionTime)<< "\n"; 
}

void FeatureExtractor::saveTime(clock_t begin){
  clock_t end = clock();
  double elapsed_secs = ( double)(end - begin) / CLOCKS_PER_SEC;
  actionTime.push_back(elapsed_secs);
}

FeatureExtractorImpl::FeatureExtractorImpl(  DatasetParametrs params){
  numberOfDims=params.rBins*params.betaBins*params.thetaBins*3;
  this->params=params;
}

int FeatureExtractorImpl::numberOfFeatures(){
  return numberOfDims;
}

string FeatureExtractorImpl:: featureName(int i){
  string str="shape_context" + intToString(i);
  return str;
}

FeatureVector FeatureExtractorImpl::getFeatures(Action action){
  FeatureVector fullVect;
  clock_t begin = clock();

  vector<Instant> instants=getInstants( action);
  for(int i=0;i<instants.size();i++){
	Instant instant=instants.at(i);
	Histogram3D * histogram=getShapeContext3D( params,instant);
	//histogram->show();
	FeatureVector * part=histogram->toVector();
	fullVect.insert(fullVect.end(),part->begin(),part->end());
	delete histogram;
  }
  saveTime(begin);
  return fullVect;
}

DynamicExtractor::DynamicExtractor(DatasetParametrs params){
  numberOfDims=params.rBins*params.betaBins*params.thetaBins;
  this->params=params;
}

int DynamicExtractor::numberOfFeatures(){
  return numberOfDims;
}

string DynamicExtractor:: featureName(int i){
  string str="dynamic_shape_context" + intToString(i);
  return str;
}

FeatureVector DynamicExtractor::getFeatures(Action action){
  FeatureVector fullVect;
  clock_t begin = clock();
  //differenceOfGaussian3D(&action,false);
  
  Histogram3D * histogram=getSimpeShapeContext(params, action);
  FeatureVector * part=histogram->toVector();
  fullVect.insert(fullVect.end(),part->begin(),part->end());
  delete histogram;
  saveTime(begin);
  return fullVect;
}


//**********

VarianceExtractor::VarianceExtractor(DatasetParametrs params){
  numberOfDims=params.rBins*params.betaBins*params.thetaBins;// *3;
  this->params=params;
}

int VarianceExtractor::numberOfFeatures(){
  return numberOfDims;
}

string VarianceExtractor:: featureName(int i){
  string str="variance_shape_context" + intToString(i);
  return str;
}

FeatureVector VarianceExtractor::getFeatures(Action action){
  FeatureVector fullVect;
  clock_t begin = clock();
  
  PointCloud * cloud=actionVar(&action,projectionXY);
  Histogram3D * hist=getActionShapeContext(  params, cloud);
  FeatureVector * part=hist->toVector();
  fullVect.insert(fullVect.end(),part->begin(),part->end());
  delete cloud;
  delete hist;
  
  /*cloud=actionVar(&action,projectionZX);
  hist=getActionShapeContext(  params, cloud);
  part=hist->toVector();
  fullVect.insert(fullVect.end(),part->begin(),part->end());
  delete cloud;
  delete hist;
  
  cloud=actionVar(&action,projectionZY);
  hist=getActionShapeContext(  params, cloud);
  part=hist->toVector();
  fullVect.insert(fullVect.end(),part->begin(),part->end());
  delete cloud;
  delete hist;*/

  saveTime(begin);
  return fullVect;
}
