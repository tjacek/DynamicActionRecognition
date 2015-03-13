#include "StdAfx.h"
#include "Dataset.h"
#include "ShapeContext3D.h"
#include "utils.h"

void FeatureExtractor::showTime(){
  cout <<"\n mean: " << mean(actionTime)<< "\n"; 
  cout <<"\n sd: " << standardDeviation(actionTime)<< "\n"; 
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
  clock_t end = clock();
   double elapsed_secs = ( double)(end - begin) / CLOCKS_PER_SEC;
  actionTime.push_back(elapsed_secs);
  return fullVect;
}

int DynamicExtractor::numberOfFeatures(){
  return 3*8*4*2;
}

string DynamicExtractor:: featureName(int i){
  string str="dynamic_shape_context" + intToString(i);
  return str;
}

FeatureVector DynamicExtractor::getFeatures(Action action){
  FeatureVector fullVect;
  clock_t begin = clock();
  pair<Histogram3D *,Histogram3D *> hist=getDynamicShapeContext3D(action,1);
  FeatureVector * part1=hist.first->toVector();
  FeatureVector * part2=hist.second->toVector();
  fullVect.insert(fullVect.end(),part1->begin(),part1->end());
  fullVect.insert(fullVect.end(),part2->begin(),part2->end());
  delete hist.first;
  delete hist.second;
  clock_t end = clock();
   double elapsed_secs = ( double)(end - begin) / CLOCKS_PER_SEC;
  actionTime.push_back(elapsed_secs);
  return fullVect;
}