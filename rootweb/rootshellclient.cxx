#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<TSocket.h>
#include<TMessage.h>
#include"TJSRootShellClient.h"
#include<stdlib.h>
using namespace std;


int main(int argc,char *argv[])
{
  string json_msg;

  TJSRootShellClient client(9090,argc,argv,false);
  if(client.Init() != 0) return false;
  
  //current prompid is set use env variables in php pipe
  string JSRPromptID=string(getenv("JSRPromptID"));
  if(JSRPromptID.empty()){
    cerr<<"Error: can not read current JSRPromptID from envoriment variables.";
    return false;
  }

  string JSRPromptCode=string(getenv("JSRPromptCode"));
  if(JSRPromptCode.empty()){
    cerr<<"Error: can not read current JSRPromptCode from envoriment variables.";
    return false;
  }
  client.processLineRequest(JSRPromptCode);
  
  string msg;
  client.recvStdout(msg);
  cout<<msg;
  client.recvStderr(msg);
  cerr<<msg;
  return true;
}
