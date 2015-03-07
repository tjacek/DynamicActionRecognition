#include "StdAfx.h"
#include "Dataset.h"
#include "ShapeContext3D.h"

int FeatureExtractorImpl::numberOfFeatures(){
  return 3*8*4*numberOfInstants;
}

string FeatureExtractorImpl:: featureName(int i){
  string str="shape_context" + intToString(i);
  return str;
}

FeatureVector FeatureExtractorImpl::getFeatures(Action action){
  FeatureVector fullVect;
  vector<Instant> instants=getInstants( action);
  for(int i=0;i<instants.size();i++){
	Instant instant=instants.at(i);
	Histogram3D * histogram=getShapeContext3D(instant);
	//histogram->show();
	FeatureVector * part=histogram->toVector();
	fullVect.insert(fullVect.end(),part->begin(),part->end());
	delete histogram;
  }
  return fullVect;
}