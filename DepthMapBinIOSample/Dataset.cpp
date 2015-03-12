#include "StdAfx.h"
#include "Dataset.h"
#include "pca.h"
#include "DepthMap.h"

pair<Dataset,Labels> buildFullDataset(Categories categories){
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
  dataset.dimReduction(50);
  //dataset.normalize();
  dataset.toArff(labels);
  ofstream myfile;
  myfile.open ("shapeContext3DFull.arff");
  myfile << dataset.toArff(labels);
  myfile.close();
  pair<Dataset,Labels> pair;
  pair.first=dataset;
  pair.second=labels;
  return pair;
}

void Dataset::normalize(){
  for(int j=0;j<numberOfFeatures();j++){
	normalizeAtribute(j);
  }
}

void Dataset::normalizeAtribute(int i){
  double maxValue=-1.0;
  for(int j=0;j<desc->size();j++){
	double value=desc->at(j).at(i);
	if(maxValue<value){
	  maxValue=value;
	}
  }

  if(maxValue!=0){
  for(int j=0;j<desc->size();j++){
	double value=desc->at(j).at(i);
	//cout << value <<"\n";
	//cout << maxValue <<"\n";

    desc->at(j).at(i)=value/maxValue;
  }
  }
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
  dimReducted=false;
  extractor=new DynamicExtractor();
  desc=new vector<vector<double>>();
}

void Dataset::addActions(vector<Action> actions){
  for(int i=0;i<actions.size();i++){
	  addExample(actions.at(i));
  }

}

void Dataset::addExample(Action depthMap){
	desc->push_back(extractor->getFeatures(depthMap));
	freeAction(depthMap);
}

void  Dataset::dimReduction(int k){
  dimReducted=true;
  reducedDim=k;
  vector<vector<double>> * old=desc;
  this->desc=new vector<vector<double>>();
  int size=old->size();
  MatrixXd pca_projc=pca(k,vectorsToMat(*old));
  //cout << pca_projc <<"\n";
  for(int i=0;i<size;i++){
    vector<double> point=old->at(i);
	//cout << point.size() <<"\n";
	vector<double> newPoint= applyProjection(point,pca_projc);
	//cout << newPoint.size() <<"\n";
	desc->push_back(newPoint);
  }
}

vector<double> Dataset::getSample(int i){
  return desc->at(i);
}

int Dataset::numberOfFeatures(){
  if(dimReducted){
    return reducedDim;
  }
  return extractor->numberOfFeatures();
}

string Dataset::toArff(Labels labels){
  string arff="@RELATION DepthMaps \n";
  arff+=getAttributes();
  arff+="\n @DATA \n";
  arff+=getData(labels);
  extractor->showTime();
  return arff;
}

string  Dataset::getAttributes(){
  string str="";
  vector<float>* fullFeatures = new vector<float>();
  int size=numberOfFeatures();
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
  for(int i=0;i<desc->size();i++){
    string line="";
    vector<double> v=desc->at(i);
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

int Dataset::size(){
	return desc->size();
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

string doubleToString(double raw){
  string tmp; 
  sprintf((char*)tmp.c_str(), "%f", (float)raw);
  string * str=new string(tmp.c_str());
  return *str;
}

void saveToFile(string filename,string data){
 ofstream myfile;
  myfile.open(filename);
  myfile << data;
  myfile.close();
}

void saveDataset(Dataset dataset,Labels labels){
 
  short size0= floor((2.0/3.0)*((double)dataset.size())  );
  int size1= dataset.size()-size0;
  int dim=dataset.numberOfFeatures();
  string data1="" + intToString(size0*dim)+"\n";
  string data2="" + intToString(size1*dim)+"\n";
  string labels1=intToString(size0) +"\n" + intToString(20)+"\n";
  string labels2=intToString(size1) +"\n" + intToString(20)+"\n";

  for(int i=0;i<dataset.size();i++){
	 FeatureVector instance=dataset.getSample(i);
	 if((i %2)==0){
	   for(int j=0;j<dataset.numberOfFeatures();j++){
		 data1+=doubleToString(instance.at(j));
		 if(j!=(instance.size()-1)){
           data1+=",";
		 }else{
		   data1+="\n";  
		 }
	   }
	   labels1+=intToString(labels.at(i))+"\n";
	 }else{
       for(int j=0;j<instance.size();j++){
		 data2+=doubleToString(instance.at(j));
		 if(j!=(instance.size()-1)){
           data2+=",";
		 }else{
		   data2+="\n";  
		 }
	   }
	   labels2+=intToString(labels.at(i))+"\n";
	 }
  }
    saveToFile("TrainData.txt",data1);

  saveToFile("TestData.txt", data2);
  saveToFile("TrainLabels.txt",labels1);
  saveToFile("TestLabels.txt", labels2);

}