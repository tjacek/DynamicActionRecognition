

#include "stdafx.h"
#include "dataset.h"

#include <windows.h>
#include <tchar.h> 
#include <strsafe.h>


typedef vector<string>* ImageList;
typedef map<string,int> Categories;

extern ImageList getImageList(string dirName);
extern Categories readCategories(string name);
extern Labels getLabels(ImageList imageList,Categories categories );
extern int getFilesList(TCHAR * directory,ImageList  files);
extern TCHAR * stringToTCHAR(string input);
extern void appendFullPath(ImageList imageList, string dirName);
extern string * WCHARToString(WCHAR * wc);
extern Labels getLabels(ImageList imageList,Categories categories );