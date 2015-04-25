#include "StdAfx.h"
#include "Difference.h"

void differenceOfGaussian3D(Action * action){
  ActionBackground background(action);
  Kernel * exp=new ExpKernel(1.0,1.0,1.0);
  background.convol(exp,action);
 /* for(int i=0;i<action.size();i++){
	CDepthMap* dmap=action.at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
		float val=dmap->GetItem(j,k)- background.data[i][j][k];
		dmap->SetItem(j,k,val);
	  }
	}
  }*/
}

ActionBackground::ActionBackground(Action * action){
  this->frames=action->size();
  this->rows=action->at(0)->GetNRows();
  this->cols=action->at(0)->GetNCols();
  data=new double**[frames];
  for(int i=0;i<frames;i++){
	data[i]=new double*[rows];
	for(int j=0;j<rows;j++){
	  data[i][j]=new double[cols];
    }
  }
}

void ActionBackground::convol(Kernel * kernel,Action * action){
  for(int i=0;i<frames;i++){
	for(int j=0;j<rows;j++){
	  for(int k=0;k<cols;k++){
	    data[i][j][k]=weightedSum(i,j,k, action, kernel);
      }
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

double weightedSum(int x0,int y0,int t0,Action * action,Kernel * kernel){
  double sum=0.0;
  for(int i=0;i<action->size();i++){
	CDepthMap* dmap=action->at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
        double w=kernel->eval(i-x0,j-y0,k-t0);
		sum+=w*dmap->GetItem(j,k);
	   }
	}
  }
  return sum;
}

