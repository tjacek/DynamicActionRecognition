#include "StdAfx.h"
#include "Vibe.h"

class BackgroundModel{
  public:
    int width ; 
    int height;
	int nbSamples;  
	float *** samples;
	BackgroundModel(Action * action,VibeParams * vibeParams);
	int compare(int x,int y,uchar point ,VibeParams * vibeParams);
	void update(int x,int y,uchar point ,VibeParams * vibeParams);
	void updateNeighbor(int x,int y,uchar point ,VibeParams * vibeParams);


  private:
    static int nearX[8] ;
    static int nearY[8] ;
	void alocateSamples();
	void initSamples(Action * action);
};

extern void vibeFrame(CDepthMap * depth,BackgroundModel * model,VibeParams * params);


static BackgroundModel * model=NULL;

BackgroundModel::BackgroundModel(Action * action,VibeParams * vibeParams){
	cout << action->size();
	this->width = action->at(0)->GetNCols();
    this->height=action->at(0)->GetNRows();
    this->nbSamples=vibeParams->nbSamples;
    alocateSamples();
    initSamples(action);
}

void vibeAction(Action * action,VibeParams * params){
  if(model==NULL){
    model =new  BackgroundModel(action,params);
  }
  for(int i=0; i<action->size();i++){
	vibeFrame(action->at(i),model,params);
	connectedCommponents(action->at(i));
  }
}

void vibeFrame(CDepthMap * depth,BackgroundModel * model,VibeParams * params){
  for(int i=1;i<depth->GetNCols()-1;i++){
	for(int j=1;j<depth->GetNRows()-1;j++){
	  float point=depth->GetItem(j,i);
	  int count=model->compare(j,i,point,params);
	  bool isInBackground=  (count >= params->reqMatches);
	  if(isInBackground){
		if(params->decideUpdate()){
		  model->update(j,i,point ,params);
		}
		if(params->decideUpdate()){
		  model->updateNeighbor(j,i,point ,params);
		}
		depth->SetItem(j,i,0);
	  }else{}
    }
  }
}

void BackgroundModel::alocateSamples( ){
  samples=new float**[height];
  for(int i=0;i<height;i++){
	samples[i]=new float*[width];
	for(int j=0;j<width;j++){
	  samples[i][j]=new float[nbSamples];
	  for(int k=0;k<nbSamples;k++){
		samples[i][j][k]= 0;
	  }
	}
  }
}

void BackgroundModel::initSamples(Action * action){
  CDepthMap * dimage=  action->at(0);
  samples=new float**[height];
  for(int i=0;i<height;i++){
	samples[i]=new float*[width];
	for(int j=0;j<width;j++){
	  samples[i][j]=new float[nbSamples];
	  for(int k=0;k<nbSamples;k++){
		 // int n=rand() % action->size();	  
	     // float value=action->at(n)->GetItem(i,j);
		  samples[i][j][k]= dimage->GetItem(i,j);
	  }
	}
  }
}

int BackgroundModel::nearX[8]={-1,0,1,-1, 1,-1, 0, 1};
int BackgroundModel::nearY[8]={ 1,1,1, 0, 0,-1,-1,-1};

int BackgroundModel::compare(int x,int y,uchar point ,VibeParams * vibeParams){
	int count = 0,index=0;
	while ((count < vibeParams->reqMatches) && (index < nbSamples)){
		uchar sample = samples[x][y][index];
		uchar distance=metric(point,sample);
		if(distance < vibeParams->radius){
			count++;
		}
		index++;
	}
	return count;
}

void BackgroundModel::updateNeighbor(int x,int y,uchar point ,VibeParams * vibeParams){
    int randn= rand() % 9;
	int x0 = BackgroundModel::nearX[randn];
	int y0 = BackgroundModel::nearY[randn];

	x0+= x;
	y0+= y;
	if(x0<0){
		x0=0;
	}
	if(y0<0){
		y0=0;
	}
	if(x0>= this->height){
		x0=0;
	}
	if(y0>= this->width){
		y0=0;
	}
}

void BackgroundModel::update(int x,int y,uchar point ,VibeParams * vibeParams){
  int n=std::rand() % vibeParams->subsamplingFactor;
  samples[x][y][n]=point;
}

VibeParams::VibeParams(){
    this->nbSamples = 20;                  
    this->reqMatches = 7;                   
    this->radius = 1;                     
    this->subsamplingFactor = 17;  
}

VibeParams::VibeParams(int nbSamples,int reqMatches,int radius,int subsamplingFactor){
    this->nbSamples = nbSamples;                  
    this->reqMatches = reqMatches;                   
    this->radius = radius;                     
    this->subsamplingFactor = radius;
	std::srand((unsigned int) std::time(0));
}

bool VibeParams::decideUpdate(){
  int n=std::rand() % subsamplingFactor;
  return n==0;
}

float inline metric(float  x,float y){
	if(x<y){
		return y-x;
	}
	return x-y;
}

/*

*/