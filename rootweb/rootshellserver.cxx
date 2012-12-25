#include"TJSRootShellServer.h"
#include<iostream>
using namespace std;

int main(int argc,char *argv[])
{
  TJSRootShellServer rootshellserver(9090,argc,argv);
  
  rootshellserver.Init();
  
  rootshellserver.Loop();
  return true;
}