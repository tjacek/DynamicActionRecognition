#include "StdAfx.h"
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

ExpKernel::ExpKernel( double sigmaX,double sigmaY,double sigmaZ){
  this->sigmaX=sigmaX;
  this->sigmaY=sigmaY;
  this->sigmaZ=sigmaZ;
}

double ExpKernel::eval(int x,int y,int t){
  double d= (x*x)/sigmaX + (y*y)/sigmaY + (t*t)/sigmaZ;
  d/=2.0;
  d*= -1.0;
  double C=1.0/(sqrt( 2*M_PI*M_PI*M_PI * sigmaX*sigmaY*sigmaZ));
  return exp(C*d);
}

double weightedSum(int x0,int y0,Action action,Kernel * kernel){
  double sum=0.0;
  for(int i=0;i<action.size();i++){
	CDepthMap* dmap=action.at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
        double w=kernel->eval(i-x0,j-y0,k);
		sum+=w*dmap->GetItem(j,k);
	  }
	}
  }
  return sum;
}