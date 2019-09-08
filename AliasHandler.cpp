#include<iostream>
#include<stdlib.h>
#include<unordered_map>
#include<utility>
#include<vector>
#include<bits/stdc++.h>
using namespace std;

static unordered_map<string,string> umap;

void setAlias(string key,string value)
{
  
  umap[key]=value;
}

string getAlias(string key)
{
	if(umap.find(key)!=umap.end())
		return umap[key];
	else return "FAILURE"; //report no such key
}

vector<string> splitAliasInput(string user_input)
{
	string key,value;
	
	
	key=user_input.substr(user_input.find(" ")+1,user_input.find("=")-user_input.find(" ")-1);
	value=user_input.substr(user_input.find("\"")+1,user_input.rfind("\"")-(user_input.find("\"")+1));
	vector<string> ret_val;
	ret_val.push_back(key);
	ret_val.push_back(value);
	return ret_val;
}

int ifAlias(string input)
{
	if(input.find("alias")!=std::string::npos)
		return 1;
	else return 0;
}

void setUpAlias(string input)
{
	if(ifAlias(input))
	{

	  vector<string> mv=splitAliasInput(input);
      setAlias(mv[0],mv[1]);
      //return mv[1]; //return value associated.
	}
	//else return "FAILURE";
}

string checkAliasAndExpand(string input)
{
	stringstream ch1(input);
	string input_copy=input;
	string temp="";
	string bld;
	while(getline(ch1,temp,' '))
	{
      if(getAlias(temp).compare("FAILURE")!=0) //alias found
      {
      	input_copy.replace(input.find(temp),temp.length(),getAlias(temp));
      }
	}
	return input_copy;
}


