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

  TJSRootShellClient client(9090,argc,argv);
  client.Init();

  json_msg = "{'promptid':'"+string(getenv("JSRPromptID"))+"',";
   string msg=client.getPipe();

  client.processLineRequest(msg);
  client.recvStdout(msg);
  json_msg += msg+",";
  client.recvStderr(msg);
  json_msg += msg+"}";
  cout<<json_msg<<endl;
  return 0;
}
