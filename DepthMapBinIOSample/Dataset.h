#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "io.h"
#include <ctime>

typedef vector<double> FeatureVector;

class FeatureExtractor{
  public:
	virtual int numberOfFeatures()=0;
	virtual string featureName(int i)=0;
    virtual FeatureVector getFeatures(Action action)=0;
	virtual void showTime();
  protected:
    vector<double> actionTime;
};

class FeatureExtractorImpl: public FeatureExtractor{
  public:
	int numberOfFeatures();
    string featureName(int i);
    FeatureVector getFeatures(Action action);
};

class DynamicExtractor: public FeatureExtractor{
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
	int size();
	vector<double> getSample(int i);
	string toArff(Labels labels);
	void dimReduction(int k);
	void normalize();
  private:
    bool dimReducted;
	int reducedDim;
	vector<FeatureVector> * desc;
	string getAttributes();
	string getData(Labels labels);
	void normalizeAtribute(int i);
};

extern void buildDataset(vector<Action> actions,Labels labels);
extern pair<Dataset,Labels> buildFullDataset(Categories categories);
extern vector<string> getClassNames();
extern void saveDataset(Dataset dataset,Labels labels);