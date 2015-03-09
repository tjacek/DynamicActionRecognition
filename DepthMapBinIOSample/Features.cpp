#include "StdAfx.h"
#include "Dataset.h"
#include "ShapeContext3D.h"
#include "utils.h"

void FeatureExtractor::showTime(){
  cout <<"\n mean: " << mean(actionTime)<< "\n"; 
  cout <<"\n sd: " << standardDeviation(actionTime)<< "\n"; 
}

int FeatureExtractorImpl::numberOfFeatures(){
  return 3*8*4*numberOfInstants;
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
	Histogram3D * histogram=getShapeContext3D(instant);
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