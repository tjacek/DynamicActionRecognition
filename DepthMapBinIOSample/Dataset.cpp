#include "StdAfx.h"
#include "Dataset.h"

#include "DepthMap.h"

void buildFullDataset(Categories categories){
  Dataset dataset;
  Labels labels;
  map<string,int>::iterator it;
  for(it=categories.begin();it!=categories.end();it++){
    pair<string,int> p_i=*it;
	cout << p_i.first<<"\n";
	char * c=(char*)p_i.first.c_str();
	Action action=readAction(c);
	dataset.addExample(action);
	labels.push_back(p_i.second);
  }
  dataset.toArff(labels);
  ofstream myfile;
  myfile.open ("shapeContext3DFull.arff");
  myfile << dataset.toArff(labels);
  myfile.close();
}


void buildDataset(vector<Action> actions,Labels labels){
  Dataset dataset;
  dataset.addActions(actions);
  dataset.toArff(labels);
  ofstream myfile;
  myfile.open ("shapeContext3D.arff");
  myfile << dataset.toArff(labels);
  myfile.close();
}

Dataset::Dataset(){
  extractor=new FeatureExtractorImpl();
}

void Dataset::addActions(vector<Action> actions){
  for(int i=0;i<actions.size();i++){
	  addExample(actions.at(i));
  }

}

void Dataset::addExample(Action depthMap){
	desc.push_back(extractor->getFeatures(depthMap));
	freeAction(depthMap);
}

vector<double> Dataset::getSample(int i){
  return desc.at(i);
}

int Dataset::numberOfFeatures(){
  return extractor->numberOfFeatures();
}

string Dataset::toArff(Labels labels){
  string arff="@RELATION DepthMaps \n";
  arff+=getAttributes();
  arff+="\n @DATA \n";
  arff+=getData(labels);
  return arff;
}

string  Dataset::getAttributes(){
  string str="";
  vector<float>* fullFeatures = new vector<float>();
  int size=extractor->numberOfFeatures();
  for(int i=0;i<size;i++){
	 string feature=extractor->featureName(i);
	 str+="@ATTRIBUTE " + feature +" numeric\n";
  }
  vector<string> classes= getClassNames();
  str+="@ATTRIBUTE class { ";
  for(int i=0;i<classes.size();i++){
	  str+=classes.at(i)+" ";
  }
  str+=" } \n";
  return str;
}

string Dataset::getData(Labels labels){
  string str="";
  vector<FeatureVector>::iterator it;
  vector<string> classes= getClassNames();
  for(int i=0;i<desc.size();i++){
    string line="";
    vector<double> v=desc.at(i);
	for(int j=0;j<v.size();j++){
      double raw=v.at(j);
      string tmp; 
      sprintf((char*)tmp.c_str(), "%f", raw);
      string str2 = tmp.c_str();
      line+=str2+",";
	}
    int category=(int) labels.at(i);
    line+=classes.at(category)+"\n";
	str+=line;
  }
  return str;
}

vector<string> getClassNames(){
  vector<string> classNames;
  classNames.push_back("A");
  classNames.push_back("B");
  classNames.push_back("C");
  classNames.push_back("D");
  classNames.push_back("E");
  classNames.push_back("F");
  classNames.push_back("G");
  classNames.push_back("H");
  classNames.push_back("I");
  classNames.push_back("J");
    classNames.push_back("K");
  classNames.push_back("L");
  classNames.push_back("M");
  classNames.push_back("N");
  classNames.push_back("O");
  classNames.push_back("P");
  classNames.push_back("Q");
  classNames.push_back("R");
  classNames.push_back("S");
  classNames.push_back("T");
    classNames.push_back("U");
	  classNames.push_back("V");

  return classNames;
}