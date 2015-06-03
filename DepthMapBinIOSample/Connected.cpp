#include "StdAfx.h"
#include "Connected.h"

void connectedCommponents(CDepthMap * dimage){
  int ** relation=new int*[dimage->GetNCols()];
  for(int i=0;i<dimage->GetNCols();i++){
	relation[i]=new int[dimage->GetNRows()];
  }
  init( relation,dimage);
  int maxComponentSize=0;
  int maxComponent=1;
  int currentComponent=2;
  for(int i=1;i<dimage->GetNCols()-1;i++){
	  for(int j=1;j<dimage->GetNRows()-1;j++){
        if(relation[i][j]==0){
			int sizeOfComponent=markComponent(i,j,currentComponent,relation,dimage,1000);
			if(maxComponentSize<sizeOfComponent ){
				maxComponentSize=sizeOfComponent;
				maxComponent=currentComponent;
			}
			currentComponent++;
		}
	}
  }
  clean(maxComponent,relation,dimage);
}

void init(int **  table,CDepthMap * dimage){
  for(int i=0;i<dimage->GetNCols();i++){
	table[i][0]=border;
	table[i][dimage->GetNRows()-1]=border;
  }
  for(int j=0;j< dimage->GetNRows();j++){
    table[0][j]=border;
    table[dimage->GetNCols()-1][j]=border;
  }
  for(int i=1;i<dimage->GetNCols()-1;i++){
    for(int j=1;j< dimage->GetNRows()-1;j++){
	  if(dimage->GetItem(j,i)!=0){
		table[i][j]=unmarked;
	  }else{
        table[i][j]=valueless;
	  }
    }
  }
}
bool checkBounds(int x,int y,int ** relation,CDepthMap * dimage){
  if(x<=0){
	return false;
  }
  
  if(y<=0){
	return false;
  }

  if(dimage->GetNCols() <=x){
	return false;
  }

  if(dimage->GetNRows() <=y){
	return false;
  }

  return true;
}

int markComponent(int x,int y,int componentNumber,int ** relation,CDepthMap * dimage,int iter){

  if(componentNumber<2){
	  return 0;
  }

  if(iter<0){
	return 0;
  }
  if(checkBounds( y, x, relation,dimage)){
    if(relation[x][y]==border){
	  return 0;
    }
    if(relation[x][y]==valueless){
	  return 0;
    }
	if(relation[x][y]==unmarked){
      int numberOfPixels=0;
      relation[x][y]=componentNumber;
	  numberOfPixels+=markComponent(x  ,y+1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x+1,y  ,componentNumber,relation, dimage,iter-1);
      numberOfPixels+=markComponent(x+1,y+1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x  ,y-1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x-1,y  ,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x-1,y-1,componentNumber,relation, dimage,iter-1);
      numberOfPixels+=markComponent(x-1,y+1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x+1,y-1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=1;
	  return numberOfPixels;
    }
  }
  return 0 ;
}

void clean(int maxComponent,int ** relation,CDepthMap * dimage){
  for(int i=0;i<dimage->GetNCols();i++){
	  //cout << "\n";
	for(int j=0;j<dimage->GetNRows();j++){
      //cout << relation[j][i] << " ";
	  if(relation[j][i]!=maxComponent){
		  dimage->SetItem(j,i,0);
	  }
	}
  }
}