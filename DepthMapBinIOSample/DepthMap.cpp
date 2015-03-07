#include "StdAfx.h"
#include <math.h>
#include "DepthMap.h"

void freeAction(Action action){
  for(int i=0;i<action.size();i++){
	CDepthMap * dmap=  action.at(i);
	delete dmap;
  }
}

vector<Instant> getInstants(Action action){
  vector<Instant> instants;
  int sizeOfInstants=action.size()/numberOfInstants;
  for(int i=0;i<numberOfInstants;i++){
     Instant instant;
     for(int j=0;j<sizeOfInstants;j++){
		 int index=i*sizeOfInstants +j;
		 instant.push_back(action.at(index));
     }
	 instants.push_back(instant);
  }
  return instants;
}

CDepthMap::CDepthMap(void)
{
	m_ncols = 0;
	m_nrows = 0;
	m_depthVals = NULL;
}

CDepthMap::~CDepthMap(void)
{
	if(m_depthVals)
	{
		delete[] m_depthVals;
		m_depthVals = NULL;
	}
}

void CDepthMap::SetSize(int ncols, int nrows) //allocate space
{
	if(m_depthVals != NULL)
	{
		delete[] m_depthVals;
		m_depthVals = NULL;
	}
	m_nrows = nrows;
	m_ncols = ncols;
	
	m_depthVals = new float[nrows*ncols];
	return;
}

void CDepthMap::SetItem(int r, int c, float val)
{
	m_depthVals[r * m_ncols + c] = val;
}

float CDepthMap::GetItem(int r, int c) const
{
	return m_depthVals[r * m_ncols + c] ;
}

//scale the depth values
void CDepthMap::Scale(float s)
{
	int r,c;
	for(r=0; r<GetNRows(); r++)
	{
		for(c=0; c<GetNCols(); c++)
		{
			float temp = GetItem(r,c);
			SetItem(r,c, temp*s);
		}
	}
}

//scale the dimensions:
//ncols = floor(scaleFactor * ncols), nrows = floor(scaleFactor * nrows)
void CDepthMap::ScaleSizeNN(float scaleFactor)
{
	//buffer the original depthmap
	CDepthMap bufDepthMap;
	CopyDepthMapTo(bufDepthMap);

	int new_ncols = floor(m_ncols * scaleFactor);
	int new_nrows = floor(m_nrows * scaleFactor);

	SetSize(new_ncols, new_nrows);

	int r,c;
	for(r=0; r<GetNRows(); r++)
	{
		for(c=0; c<GetNCols(); c++)
		{
			int origR = floor(r/scaleFactor);
			int origC = floor(c/scaleFactor);
			float origVal = bufDepthMap.GetItem(origR, origC);
			SetItem(r,c, origVal);
		}
	}
}

void CDepthMap::CopyDepthMapTo(CDepthMap & dstMap) const
{
	dstMap.SetSize(m_ncols, m_nrows);
	int r,c;
	for(r=0; r<GetNRows(); r++)
	{
		for(c=0; c<GetNCols(); c++)
		{
			float temp = GetItem(r,c);
			dstMap.SetItem(r,c, temp);
		}
	}
}

int CDepthMap::NumberOfNonZeroPoints() const
{
	int count=0;
	int r,c;
	for(r=0; r<GetNRows(); r++)
	{
		for(c=0; c<GetNCols(); c++)
		{
			float temp = GetItem(r,c);
			if(temp != 0)
			{
				count++;
			}
		}
	}
	return count;
}

float CDepthMap::AvgNonZeroDepth() const
{
	int count=0;
	float sum = 0;
	int r,c;
	for(r=0; r<GetNRows(); r++)
	{
		for(c=0; c<GetNCols(); c++)
		{
			float temp = GetItem(r,c);
			if(temp != 0)
			{
				count++;
				sum += temp;
			}
		}
	}
	if(count == 0)
		return 0;
	else
		return sum/count;
}

