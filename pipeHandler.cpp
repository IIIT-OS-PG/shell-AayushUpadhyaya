#include <iostream>
#include<unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h> 
#include<vector>
using namespace std;
#include<cstring>
int k=0;
extern int return_of_last_cmd;

vector<string>  getTokensForPipe(string input)
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


char ** getPipeCommands(string input)
{
	
    char **commands=new char*[30];
	char ch;
	
	
	string temp="";
	for(int i=0;i<input.length();i++)
	{
		ch=input.at(i);
		if(ch=='|'||ch=='\0'||ch=='\n')
		{ char *temp1=new char[50];
		  strcpy(temp1,temp.c_str());
		  commands[k++]=temp1;
		  temp="";
		  //delete [] temp1;
		}
		else
		temp+=ch;
		
	}
	char *temp1=new char[50];
		  strcpy(temp1,temp.c_str());
		  commands[k++]=temp1;
		  temp="";
   return commands;
}

bool hasPipe(string input)
{
  if(input.find("|")!=std::string::npos) return true;
  else return false;

} //end of function.

void executePipeCommands(char **commands)
{
	int pipeid[2];
	int READ_END;
	pid_t pid1,pid2,pid3;

	if(pipe(pipeid)<0)
	{
		cout<<"Error Intialising Pipe!."<<endl;
		return;
	}
	pid1=fork();
	if(pid1==0) //the first child
	{
      close(pipeid[0]);
      dup2(pipeid[1],STDOUT_FILENO);
      READ_END=pipeid[1];
      close(pipeid[1]);
      if(execlp(commands[0],commands[0],NULL)<0)
      {
      	cout<<"Error executing Pipe Command 1!"<<endl;
      	exit(1);
      }
	} //end of if pid==0.
	else //the parent process executes this
	{
         wait(NULL);

         for(int i=1;i<k-1;i++) //for each of the remaining n-1 commands.
         {
         	pid2=fork();
         	if(pid2==0) //for second child.
         	{   pipe(pipeid);
         		dup2(READ_END,STDIN_FILENO);
         	  dup2(pipeid[1],STDOUT_FILENO);
         	  READ_END=pipeid[1];
         	   //finally execute the command.
         	  strcpy(commands[i],"/bin/");
         	  if(execlp(commands[i],commands[i],NULL)<0)
                {
      	          cout<<"Error executing Pipe Command :"<<(i+1)<<endl;
      	          exit(1);
                 }
         	} //end of if pid2==0
         	wait(NULL);
         } //end of for loop.
         //wait(NULL);
         //wait(NULL);
         pid3=fork();
         if(pid3==0) //for this child process only.
         {  pipe(pipeid);
         	close(pipeid[1]);
         	dup2(READ_END,STDIN_FILENO);
         	strcpy(commands[k-1],"/bin/");
         	close(pipeid[0]);
         	if(execlp(commands[k-1],commands[k+1],NULL)<0)
                {
      	          cout<<"Error executing Pipe Command/last command :"<<(k)<<endl;
      	          exit(1);
                 }
         }
        wait(NULL);
	}
} //end of function executePipeCommands.

char ** returnCommandArguments(vector<string> input)
{
     const char * command_tbe;
     char **arguments =new char *[input.size()];

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
      arguments[k]=NULL;
      return arguments;
}

void executeCommandsWithPipe(char ** list_of_commands)
{
   for(int  i=0; i<k-1; i++)
    {
        int pd[2];
        pipe(pd);
        vector<string> input_tokens=getTokensForPipe(list_of_commands[i]);

        char ** command_w_arg=new char *[input_tokens.size()];

        command_w_arg=returnCommandArguments(input_tokens);
        if (!fork()) {
            dup2(pd[1], 1); // remap output back to parent
            execvp(command_w_arg[0], command_w_arg);
            perror("exec");
            abort();
        }

        // remap output from previous child to input
        dup2(pd[0], 0);
        close(pd[1]);
    }

    vector<string> input_tokens=getTokensForPipe(list_of_commands[k-1]);
 
        char ** command_w_arg=new char *[input_tokens.size()];
                command_w_arg=returnCommandArguments(input_tokens);

    return_of_last_cmd=execvp(command_w_arg[0], command_w_arg);
    perror("exec");
    abort();

} //end of function executeCommandsWithPipe.


