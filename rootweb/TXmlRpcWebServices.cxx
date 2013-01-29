#include"TXmlRpcWebServices.h"
#include<iostream>
#include<map>
#include<list>
#include<vector>
#include<TRint.h>
#include<TApplication.h>
#include<TException.h>
#include <TROOT.h>
#include <TCanvas.h>
#include<TBenchmark.h>
#include<TThread.h>
#include<TRandom.h>
#include<TUUID.h>

using namespace std;

//the actions are
//authetication
//startSession/stopSession/statusSession
//Web Services Server
//stopServices/statusServices
//startEngine/stopEngine/statusEngine


bool TXmlRpcWebServices::gShellStarted=false;
TStdIOHandler TXmlRpcWebServices::ioHandler;

TXmlRpcWebServices::TXmlRpcWebServices(int argc,char **argv)
{
  this->_signature = "S:ss";  // method's arguments are two strings and return a Struct
  this->_help = "This method process services engines";
}
TXmlRpcWebServices::~TXmlRpcWebServices(){if(fShellThread) delete fShellThread;}

void TXmlRpcWebServices::execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP)
{
   if(gDebug>0){
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
      if(action == "StartEngine")
      {
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
  if(engine=="Shell")
  {
    paramList.verifyEnd(6);
    if(gDebug>0)cout<<"                  : paramList String Engine:       "<<engine<<endl; 
    StartEngineShell(paramList,retvalP);
  }
  
}

void TXmlRpcWebServices::StopEngine(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP)
{
  string engine=paramList.getString(1);
  if(engine=="Shell")
  {
    if(fLogging)cout<<"                  : paramList String Engine:       "<<engine<<endl; 
    StartEngineShell(paramList,retvalP);
  }
  
}



void TXmlRpcWebServices::StartEngineShell(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP)
{
  list<string> params;
  string username = paramList.getString(2);
  
  TString sessionid(username);
  sessionid += gRandom->Integer(1e9);
  sessionid = TUUID(sessionid.Data()).AsString();
    
  string passwd      = paramList.getString(3);
  string port        = paramList.getString(4);
  string auth_method = paramList.getString(5);
  params.push_back(username);
  params.push_back(passwd);
  params.push_back(sessionid.Data());
  params.push_back(port);
  params.push_back(auth_method);
  
  
  if(gDebug>0){
    cout<<"                  : paramList String Username:        "<<username<<endl;
    cout<<"                  : paramList String SessionID:       "<<sessionid.Data()<<endl;
    cout<<"                  : paramList String Port:            "<<port<<endl;
  }
  
  fShellThread=new TThread("Shell",StartEngineShellThread,(void*)&params);
  if(fShellThread->Run())
  {
    
  } else{
    cout<<"Thread not started"<<endl;
  }
}


void TXmlRpcWebServices::StartEngineShellThread(void *prt)
{
   list<string> *params= (list<string>*)prt;
   string username = params->front();
   params->pop_front();
   string passwd   =  params->front();
   params->pop_front();
   string sessionid   =  params->front();
   params->pop_front();
   string port   =  params->front();
   params->pop_front();
   string auth_method   =  params->front();
 
   if(gDebug>0)
   {
    cout<<"                  : Thread: StartEngineShell          "<<endl;
    cout<<"                  : paramList String Username:        "<<username<<endl;
    cout<<"                  : paramList String SessionID:       "<<sessionid<<endl;
    cout<<"                  : paramList String Port:            "<<port<<endl;
    cout<<"                  : paramList String Auth Method:     "<<auth_method<<endl;
   }
 ioHandler.clear();
 ioHandler.InitCapture();
 //paramiters -u=user -p=passwd -s=sessionid -P=port -a=authetication method
 gShellStarted=gSystem->Exec(("./rootwebshell -u="+username+" -p="+passwd+" -s="+sessionid+" -P="+port+" -a="+auth_method).c_str());
 if(gShellStarted){
   
 }
 ioHandler.EndCapture();
 cout<<"stderr = "<<ioHandler.getStderr()<<endl;
 cout<<"stdout = "<<ioHandler.getStdout()<<endl;
}


