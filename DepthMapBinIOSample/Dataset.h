#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "io.h"
#include <ctime>

typedef vector<double> FeatureVector;

class DatasetParametrs{
  public:
    int rBins;
    int thetaBins;
	int betaBins;
	int reductedDim;
	string output;

	DatasetParametrs();
};

class FeatureExtractor{
  public:
	virtual int numberOfFeatures()=0;
	virtual string featureName(int i)=0;
    virtual FeatureVector getFeatures(Action action)=0;
	virtual void showTime();
  protected:
    vector<double> actionTime;
	void saveTime(clock_t begin);
};

class FeatureExtractorImpl: public FeatureExtractor{
  public:
	FeatureExtractorImpl(  DatasetParametrs params);
	int numberOfFeatures();
    string featureName(int i);
    FeatureVector getFeatures(Action action);
 private:
   int numberOfDims;
   DatasetParametrs params;
};

class DynamicExtractor: public FeatureExtractor{
  public:
	DynamicExtractor(DatasetParametrs  params);
	int numberOfFeatures();
    string featureName(int i);
    FeatureVector getFeatures(Action action);
  private:
    int numberOfDims;
    DatasetParametrs params;
};

class Dataset{
  public:
    FeatureExtractor* extractor;
	Dataset(DatasetParametrs params);
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
extern void buildFullDataset(DatasetParametrs params,Categories categories);
extern vector<string> getClassNames();
extern void saveDataset(Dataset dataset,Labels labels);