#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "io.h"
typedef vector<double> FeatureVector;

class FeatureExtractor{
  public:
	virtual int numberOfFeatures()=0;
	virtual string featureName(int i)=0;
    virtual FeatureVector getFeatures(Action action)=0;
};

class FeatureExtractorImpl: public FeatureExtractor{
  public:
	int numberOfFeatures();
    string featureName(int i);
    FeatureVector getFeatures(Action action);
};

class Dataset{
  public:
    FeatureExtractor* extractor;
	Dataset();
	void addActions(vector<Action> actions);
    void addExample(Action depthMap);
	int numberOfFeatures();
	vector<double> getSample(int i);
	string toArff(Labels labels);
  private:
	vector<FeatureVector> desc;
	string getAttributes();
	string getData(Labels labels);
};

extern void buildDataset(vector<Action> actions,Labels labels);
extern void buildFullDataset(Categories categories);
extern vector<string> getClassNames();