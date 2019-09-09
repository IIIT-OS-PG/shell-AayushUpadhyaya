#include<iostream>
#include<unistd.h>
#include<unordered_map>
#include<vector>
#include<unistd.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include<readline/history.h>
#include<cstring> 
#include<fcntl.h>
#include<vector>
#include<sys/wait.h>
#include<pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
void setEntry(string key,string value);
void openApp(string filename);
void setDefaults();

 static unordered_map <string,string> umap1;


void setEntry(string key,string value)
{
	umap1[key]=value;
}

void setDefaults()
{
	ifstream file1(".myapps");
  string input;
  vector<string> vct;
  while(getline(file1,input))
  {
     stringstream check1(input);
     string temp="";
     while(getline(check1,temp,','))
     {
       vct.push_back(temp);
     }
  }

  setEntry(vct[0],vct[1]);
  setEntry(vct[2],vct[3]);
  setEntry(vct[4],vct[5]);

}

 void appHandler(string user_input){
    //string user_input;
    //setDefaults();
    //getline(cin,user_input);


  //cout<<"for "<<vct[0]<<" files : "<<umap[vct[0]]<<endl;
  //cout<<"for "<<vct[2]<<" files : "<<umap[vct[2]]<<endl;
  //cout<<"for "<<vct[4]<<" files : "<<umap[vct[4]]<<endl;
 	stringstream ch1(user_input);
 	string temp="";
 	string file_name;int k=0;
 	while(getline(ch1,temp,' '))
 	{
         if(k==1) file_name=temp;
         k++;
 	}
  if(user_input.find("openfile")!=string::npos)
 {
  openApp(file_name);

 }
  //return 0;
}

void openApp(string filename)
{
	cout<<filename<<endl;
    char mcwd[200];
    getcwd(mcwd,200);

	string ext=filename.substr(filename.rfind("."));
	//cout<<ext<<endl;
	string def_app=umap1[ext];
	//cout<<def_app<<endl;
	const char *open_binary=new char[30];
	open_binary=def_app.c_str();
	const char *file=filename.c_str();
	char *file_abspath=new char[100];
	strcpy(file_abspath,mcwd);
	strcat(file_abspath,"/");
	strcat(file_abspath,filename.c_str());
	if(!fork())
	{
		execlp(open_binary,open_binary,file_abspath,NULL);
	}
	wait(NULL);
}


 
