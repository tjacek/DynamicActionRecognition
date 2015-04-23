#include "Difference.h"

ActionBackground::ActionBackground(int frames,int rows,int cols){
  this->frames=frames;
  this->rows=rows;
  this->cols=cols;
  data=new double**[frames];
  for(int i=0;i<frames;i++){
	data[i]=new double*[rows];
	for(int j=0;i<rows;j++){
	  data[i][j]=new double[cols];
    }
  }
}

double weightedSum(Action action,Kernel kernel){
  double sum=0.0;
  for(int i=0;i<action.size();i++){
	CDepthMap* dmap=action.at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
        double w=kernel.K(i,j,k);
		sum+=w*dmap->GetItem(j,k);
	  }
	}
  }
  return sum;
}