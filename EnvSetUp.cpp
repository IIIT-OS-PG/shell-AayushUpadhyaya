#include<unistd.h>
#include<sys/types.h>
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<bits/stdc++.h>
#include<algorithm>
#include<stdlib.h>
using namespace std;

int initialiseEnv();

int initialiseEnv()
{
	uid_t myuid;
	myuid=getuid();
	//cout<<myuid<<endl;
	ifstream file("/etc/passwd");
	string input_line;
	unsigned long long  int cuid=(unsigned long long int)myuid;
	string uuid=to_string(myuid);
	string user_data;
	//cout<<uid<<endl;
	while(getline(file,input_line))
	{	
		if(input_line.find(uuid)!=std::string::npos)
		{ //cout<<input_line<<endl;
           user_data=input_line;
		}

	} //end of while loop.

	ifstream file1("/etc/environment");
	string upath;
	while(getline(file1,input_line))
	{
		upath=input_line;
	}

    //upath.erase(remove_if(upath.begin(),upath.end(),isspace));
    int r_loc=upath.rfind("\"");
    r_loc=r_loc-upath.find("\"");
	//cout<<upath.substr(upath.find("\"")+1,r_loc-1)<<endl;

	stringstream check1(user_data);
	string intermediate;
   vector <string> mydata;
   while(getline(check1,intermediate,':'))
   {
   	mydata.push_back(intermediate);
   }
   //cout<<"Login Name : "<<mydata[0]<<endl;
   //cout<<"Encrypted password : "<<mydata[1]<<endl;
   //cout<<"Numeric User id : "<<mydata[2]<<endl;
   //cout<<"Group Id : "<<mydata[3]<<endl;
   //cout<<"Comment field : "<<mydata[4]<<endl;
   //cout<<"Home Dir : "<<mydata[5]<<endl;
   //cout<<"Bin dir : "<<mydata[6]<<endl;

   ifstream file2("/etc/hostname");
   string my_hostname;
   getline(file2,my_hostname);
   //cout<<"Hostname : "<<my_hostname<<endl;

   //start setting environment variables.

   setenv("USER",mydata[0].c_str(),0);
   setenv("PATH",upath.substr(upath.find("\"")+1,r_loc-1).c_str(),0);
   setenv("HOME",mydata[5].c_str(),0);
   setenv("HOSTNAME",my_hostname.c_str(),0);
   setenv("PS1","#:$",0);

	return 0;
} //end of function initialiseEnv