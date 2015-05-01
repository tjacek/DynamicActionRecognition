#pragma once
#include "StdAfx.h"
#include "io.h"

Categories readFulldataset(string dir){
  Categories categories;
  ImageList dirList=getImageList(dir);
  int category=0;
  for(int i=0;i<dirList->size();i++){
    string dirName=dirList->at(i);
    //cout << "\n "<< dirName <<"\n";
	ImageList images=getImageList(dirName);
	for(int i=0;i<images->size();i++){
	  categories.insert( std::pair<string,int>(images->at(i),category));    
	}
	//showFiles(images );
	category++;
  }
  return categories;
}

ImageList getImageList(string dirName){
  ImageList imageList=new vector<string>();
  getFilesList(stringToTCHAR(dirName),imageList);
  appendFullPath(imageList,dirName);
  return imageList;
}

void showFiles(ImageList imageList ){
  for(int i=0;i<imageList->size();i++){
    cout << imageList->at(i) <<"\n";
  }
}

Categories readCategories(string name){
  Categories categories;
  ifstream in_stream;

  string line;
  in_stream.open(name);

  while(!in_stream.eof()){
    in_stream >> line;
	string filename;
	int category=0;
	std::stringstream ss(line);
    std::string catStr;
	getline(ss, filename, ',');
	getline(ss, catStr, ',');
	istringstream iss(catStr);
	iss >> category;
	categories.insert ( std::pair<string,int>(filename,category));
  }
  return categories;
}

Labels getLabels(ImageList imageList,Categories categories ){
  vector<string>::iterator it;
  vector<int> fullFeatures;
  for(it=imageList->begin(); it!=imageList->end(); ++it )
  {
	 string filename= *it;
	 int i=categories[filename];
	 fullFeatures.push_back(i);
  }
  return fullFeatures;
}

void appendFullPath(ImageList imageList, string dirName){
  vector<string>::iterator it;
  int k=0;
  for( it=imageList->begin(); it!=imageList->end(); ++it )
  {
	 string oldName= *it;
	 string newName= dirName+ "/" + oldName;
	 (*imageList)[k] =newName;
	 k++;
  }
}

TCHAR * stringToTCHAR(string input){
  wstring * wtmp =new wstring();
  for(int i = 0; i < input.length(); ++i)
  *wtmp += wchar_t( input[i] );

  const wchar_t* output = wtmp->c_str();
  return (TCHAR*) output;
}

string * WCHARToString(WCHAR * wc){
    //convert from wide char to narrow char array
    char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP,0,wc,-1, ch,260,&DefChar, NULL);
    
    //A std:string  using the char* constructor.
    return new string(ch);
}

int getFilesList(TCHAR * directory,ImageList  files)
{
 WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   
   // If the directory is not specified as a command-line argument,
   // print usage.



   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.

   StringCchLength(directory, MAX_PATH, &length_of_arg);

   if (length_of_arg > (MAX_PATH - 3))
   {
      _tprintf(TEXT("\nDirectory path is too long.\n"));
      return -1;
   }

   _tprintf(TEXT("\nDataset directory is %s\n\n"), directory);

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.

   StringCchCopy(szDir, MAX_PATH,directory);
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
     // DisplayErrorBox(TEXT("FindFirstFile"));
      return dwError;
   } 
   
   // List all the files in the directory with some info about them.

   do
   {
      WCHAR * filename=ffd.cFileName;
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
		string * str =WCHARToString(filename);
		char a=str->c_str()[0];
		char b='.';
		if(a!=b){
		files->push_back(*str);
		}
        // _tprintf(TEXT("T  %s   <DIR>\n"), filename);
      }
      else
      {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
		 string * str =WCHARToString(filename);
		 files->push_back(*str);
         //_tprintf(TEXT("  %s   %ld bytes\n"), filename, filesize.QuadPart);
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
     // DisplayErrorBox(TEXT("FindFirstFile"));
   }

   FindClose(hFind);
   return dwError;
 }