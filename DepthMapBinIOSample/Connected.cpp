#include "StdAfx.h"
#include "Connected.h"

void removeIsolatedPoints(CDepthMap * dimage){
 for(int i=1;i<dimage->GetNRows()-1;i++){
   for(int j=1;j<dimage->GetNCols()-1;j++){
	    /*if(dimage->GetItem(i,j)!=0){
			//cout << dimage->GetItem(i,j) <<"\n";
		    dimage->SetItem(i,j,255);
		}*/
	  if(dimage->GetItem(i,j)!=0){
	    float value=1.0;
	   //float value2=0.0;
	    //value*=dimage->GetItem(i+1,j+1);
	    value*=dimage->GetItem(i+1,j);
	    value*=dimage->GetItem(i+1,j-1);
	    value*=dimage->GetItem(i  ,j+1);
	    value*=dimage->GetItem(i  ,j-1);
	    value*=dimage->GetItem(i-1,j+1);
	    value*=dimage->GetItem(i-1,j);
	    value*=dimage->GetItem(i-1,j-1);
	    if(value==0.0){
		   dimage->SetItem(i,j,-1.0);
	    }
	 }
   }
 }

  for(int i=1;i<dimage->GetNRows()-1;i++){
   for(int j=1;j<dimage->GetNCols()-1;j++){
	  if(dimage->GetItem(i,j)== -1.0){
		   dimage->SetItem(i,j,0.0);
	 }
   }
 }
}


void connectedCommponents(CDepthMap * dimage){
  int ** relation=new int*[dimage->GetNCols()];
  for(int i=0;i<dimage->GetNCols();i++){
	relation[i]=new int[dimage->GetNRows()];
  }
  init( relation,dimage);
  int maxComponentSize=-1;
  int maxComponent=minComponent-1;
  int currentComponent=minComponent;
  for(int i=1;i<dimage->GetNCols()-1;i++){
	  for(int j=1;j<dimage->GetNRows()-1;j++){
        
			int sizeOfComponent=markComponent(i,j,currentComponent,relation,dimage,1000);
			if(sizeOfComponent!=0){
			  if(maxComponentSize<sizeOfComponent ){
				maxComponentSize=sizeOfComponent;
				maxComponent=currentComponent;
			  }
			  currentComponent++;
			}
// cout << currentComponent <<" ";		
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

  if(componentNumber<minComponent){
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
	  numberOfPixels+=markComponent(x+1,y+1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x+1,y,componentNumber,relation, dimage,iter-1);
      numberOfPixels+=markComponent(x+1,y-1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x  ,y+1,componentNumber,relation, dimage,iter-1);
	  //numberOfPixels+=markComponent(x  ,y,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x  ,y-1,componentNumber,relation, dimage,iter-1);
      numberOfPixels+=markComponent(x-1,y+1,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x-1,y,componentNumber,relation, dimage,iter-1);
	  numberOfPixels+=markComponent(x-1,y-1,componentNumber,relation, dimage,iter-1);

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
	  if(relation[i][j]!=maxComponent){
		  dimage->SetItem(j,i,0);
	  }
	}
  }
}