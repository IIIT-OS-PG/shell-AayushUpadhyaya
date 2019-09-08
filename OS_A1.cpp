#include<unistd.h>
#include<iostream>
#include<stdio.h>
#include<bits/stdc++.h>
#include<readline/history.h>
#include<cstring> 
#include<fcntl.h>
#include<vector>
#include<sys/wait.h>
#include<pwd.h>
#include "pipeHandler.cpp" //for Pipe Statements
#include "EnvSetUp.cpp" //for Environment Setup.
#include "AliasHandler.cpp"
#define clear() printf("\033[H\033[J")


using namespace std;

vector<string> getTokens(string);
void BuildAndExecute(vector<string>);
void handleIO(string);
void handleOutput(string);
void checkSpecialCommand(string);


pid_t global_pid;
int return_of_last_cmd=0;


 vector<string>  getTokens(string input)
{ //string input;
  //getline(cin,input);
       vector<string> tokens;

  const char *myin=input.c_str();
  
  if(input.length()!=0)
  { 
    //add_history(myin);
    stringstream ss1(input);
   string nextToken="";
  while(getline(ss1,nextToken,' '))
  {
     tokens.push_back(nextToken);
  }
    
     } //end of if length!=0.
 
return tokens;


} //end of function getTokens;


void BuildAndExecute(vector<string> input)
{
 
     const char * command_tbe;
     char **arguments =new char *[input.size()+1];

     command_tbe=input[0].c_str();
     char mycommand[300];
     char *bin_path="/bin/";
     strcpy(mycommand,bin_path);
     strcat(mycommand,command_tbe);int k=0;
     for(int i=0;i<input.size();i++)
      {  
        
        char *mtemp=new char[300];
        strcpy(mtemp,input[i].c_str());
        arguments[k++]=mtemp;
        
      }

      return_of_last_cmd=execvp(mycommand,arguments);
      //cout<<endl;



} //end of function BuildAndExecute.

void handleIO(string input)
{

  size_t position;
  position=input.find(">");
  if(position!=std::string::npos)
    handleOutput(input);

} //end of function I/O.

void handleOutput(string input)
{
  size_t position=input.find(">");
   string input_command,file_to_write;
   input_command=input.substr(0,input.find(">"));
   file_to_write=input.substr(input.rfind(">")+1,input.length());
  int fd;
  if(position+1==input.rfind(">")) //append to file.
  {
     fd=open(file_to_write.c_str(),O_RDWR|O_CREAT|O_APPEND);
    dup2(fd,1);

  }
 else //write new file.
 {
   
   fd=open(file_to_write.c_str(),O_RDWR|O_CREAT);
   dup2(fd,1);
   

 }

   vector<string> l_input=getTokens(input_command);
   BuildAndExecute(l_input);
   close(fd);
} //end of function handleOutput.

void checkSpecialCommand(string input)
{
   if(input.compare("exit")==0)
   {
    cout<<"Bye\n";
    exit(0);
   }

   if(input.at(0)=='c'&&input.at(1)=='d'&&input.at(2)==' ')
   {
     
     chdir(input.substr(input.find(' ')+1,input.length()).c_str());
   }

   if(input.compare("$$")==0)
    cout<<global_pid<<endl;

  if(input.compare("$?")==0)
    cout<<return_of_last_cmd<<endl;
  if(input.compare("clear")==0) clear();

} //end of void checkSpecialCommand.

string getCurrentPath1()
{

  char *usern;
    char hostname[100];
  char mcwd[200];
  getcwd(mcwd,200);
  usern=getlogin();
  //cout<<usern<<endl;
  gethostname(hostname,100);
  string return_result=mcwd;
  struct passwd *info = getpwnam(usern);
  //cout<<"\n"<<info->pw_dir<<endl;
  //cout<<mcwd<<endl;
  if(return_result.find(info->pw_dir)!=std::string::npos)
  {
    return_result.replace(return_result.find(info->pw_dir),strlen(info->pw_dir),"~");
  }
  
 return  return_result;
} //end of function getCurrentPath1


int main()
{  initialiseEnv();
   global_pid=getpid();
   char ** arguments;
   char *usern;
  char hostname[100];
  char mcwd[200];
  getcwd(mcwd,200);
  usern=getlogin();
  //cout<<usern<<endl;
  gethostname(hostname,100);
  string user_input;
  //cout<<"\n"<<getCurrentPath<<endl;
  while(1)
  {  mcwd[0]='\0';
    getcwd(mcwd,200);
    cout<<usern<<"@"<<hostname<<":"<<getCurrentPath1()<<"$";
    vector<string> input_tokens;
    getline(cin,user_input);

    if(ifAlias(user_input))
   {   
      setUpAlias(user_input); //set up alias first if any.

      continue;
   }
      user_input=checkAliasAndExpand(user_input);
      cout<<user_input<<endl;
      checkSpecialCommand(user_input);
      pid_t pid1;

   char ** pipe_commands=new char*[100];

  if(hasPipe(user_input)) //if piped command is given then.
  {  
     
    pid1=fork();
    if(pid1==0){   
      pipe_commands=getPipeCommands(user_input);
    
    //executePipeCommands(commands);
     executeCommandsWithPipe(pipe_commands);
   } //child process code.
   wait(NULL);


  }

  else{   //Normal Code-Without pipe
    input_tokens=getTokens(user_input);
    for(auto it =input_tokens.begin();it!=input_tokens.end();it++)
      cout<<*it<<endl;

    /*for(int i=0;i<input_tokens.size();i++)
      cout<<"\t"<<input_tokens[i];
     cout<<endl; */
    
    
    pid_t c_pid;
    c_pid=fork();
   if(c_pid==0) //for child process
   {
     /*const char * command_tbe;
     char **arguments =new char *[input_tokens.size()+1];

     command_tbe=input_tokens[0].c_str();
     char mycommand[300];
     char *bin_path="/bin/";
     strcpy(mycommand,bin_path);
     strcat(mycommand,command_tbe);int k=0;
     for(int i=0;i<input_tokens.size();i++)
      {  
        
        char *mtemp=new char[300];
        strcpy(mtemp,input_tokens[i].c_str());
        arguments[k++]=mtemp;
        
      }

      
     
      execv(mycommand,arguments);
      exit(0); */
       if(user_input.find(">")!=std::string::npos)
       {
        handleOutput(user_input);
       }
       else{
      BuildAndExecute(input_tokens);
      //cout<<endl;
         }
      

   }  
     wait(NULL);
   } //end of else.  
  
  } //end of while loop.
  

 return 0;
} //end of main.
