#include"TXmlRpcWebServices.h"
#include<iostream>
#include<map>
#include<vector>
#include<TRint.h>
#include<TApplication.h>
#include<TException.h>
#include <TROOT.h>
#include <TCanvas.h>
#include<TBenchmark.h>
#include<TThread.h>
using namespace std;

//the actions are
//authetication
//startSession/stopSession/statusSession
//Web Services Server
//stopServices/statusServices
//startEngine/stopEngine/statusEngine

bool TXmlRpcWebServices::gShellStarted=false;
TStdIOHandler TXmlRpcWebServices::ioHandler;

TXmlRpcWebServices::TXmlRpcWebServices(int argc,char **argv,bool logging)
{
  fLogging=logging;
  this->_signature = "S:ss";  // method's arguments are two strings and return a Struct
  this->_help = "This method process services engines";
}
TXmlRpcWebServices::~TXmlRpcWebServices(){if(fShellThread) delete fShellThread;}

void TXmlRpcWebServices::execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP)
{
   if(fLogging){
     cout<<"TXmlRpcWebServices: paramList Size: "<<paramList.size()<<endl;
     if(paramList.size()>1)
     {
     cout<<"                  : paramList String Action:       "<<paramList.getString(0)<<endl;

     }
    }
    string action;
    if(paramList.size()>0)
    {
      action=paramList.getString(0);
      if(action == "startEngine")
      {
	paramList.verifyEnd(2);
	StartEngine(paramList,retvalP);
      }else{
      }
    }
      
// 
    std::map<std::string, xmlrpc_c::value> result;
//     
//     std::pair<string, xmlrpc_c::value> promptidm("promptid", xmlrpc_c::value_string(promptid));
    
//     std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(ioHandler.getStdout()));
//     std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(ioHandler.getStderr()));
//     std::pair<string, xmlrpc_c::value> canvases_sizem("canvases_size", xmlrpc_c::value_int(canvases_size));
//     std::pair<string, xmlrpc_c::value> canvases_namesm("canvases_names",xmlrpc_c::value_array(canvases_names));
    

//     result.insert(promptidm);
//     result.insert(stdoutm);
//     result.insert(stderrm);
//     result.insert(canvases_sizem);
//     result.insert(canvases_namesm);
    
    *retvalP = xmlrpc_c::value_struct(result);
}

void TXmlRpcWebServices::StartEngine(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP)
{
  string engine=paramList.getString(1);
  if(engine=="shell")
  {
    
    if(fLogging)cout<<"                  : paramList String Engine:       "<<engine<<endl; 
    StartEngineShell(retvalP);
  }
  
}

void TXmlRpcWebServices::StopEngine(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP)
{
  string engine=paramList.getString(1);
  if(engine=="shell")
  {
    
    if(fLogging)cout<<"                  : paramList String Engine:       "<<engine<<endl; 
    StartEngineShell(retvalP);
  }
  
}



void TXmlRpcWebServices::StartEngineShell(xmlrpc_c::value *const  retvalP)
{
  
  fShellThread=new TThread("shell",StartEngineShellThread);
  if(fShellThread->Run())
  {
    
  }
  
}


void TXmlRpcWebServices::StartEngineShellThread(void *prt)
{
 ioHandler.clear();
 ioHandler.InitCapture();
 gShellStarted=gSystem->Exec("./rootwebshell");
 ioHandler.EndCapture();
 cout<<"stderr = "<<ioHandler.getStderr()<<endl;
 cout<<"stdout = "<<ioHandler.getStdout()<<endl;
}


