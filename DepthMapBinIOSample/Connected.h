#pragma once
#include "stdafx.h"
#include "DepthMap.h"

#define border   -1
#define valueless 0
#define unmarked  1

extern void connectedCommponents(CDepthMap * dimage);
extern void init(int **  table,CDepthMap * dimage);
extern int markComponent(int x,int y,int componentNumber,int ** relation,CDepthMap * dimage,int iter);
extern void clean(int maxComponent,int ** relation,CDepthMap * dimage);