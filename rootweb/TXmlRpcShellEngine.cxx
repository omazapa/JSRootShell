#include"TXmlRpcShellEngine.h"
#include<iostream>
#include<map>
#include<vector>
#include<TRint.h>
#include<TApplication.h>
#include<TException.h>
#include <TROOT.h>
#include <TCanvas.h>
#include<TBenchmark.h>
#include"TApplicationRemoteShell.h"
#include<TSystem.h>
#include<TRandom.h>
#include<TUUID.h>
#include<signal.h>
#include<list>
#include<TROOT.h>

using namespace std;

std::list<string>  gStdout;
std::list<string>  gStderr;
bool 	           gCapturing=false;

void signal_callback_handler_int(int signum)
{
  cout<<"Captured Interrupt"<<endl;
//   if(gROOT->IsLineProcessing()){
//     gROOT->SetInterrupt(true);
//   } 
//   gROOT->SetInterrupt(true);
//   gApplication->HandleException(2);
  //gROOT->SetInterrupt(true);
}

TXmlRpcShellEngine::TXmlRpcShellEngine(int argc,char **argv,bool logging)
{
  fLogging=logging;
  this->_signature = "S:ss";  // method's arguments are two strings and return a Struct
  this->_help = "This method process c++ code using gROOT";
  gROOT->SetName("JSRootShell");
  if(gBenchmark==NULL){
    gBenchmark=new TBenchmark();
  }
  signal(SIGINT, signal_callback_handler_int);
   gROOT->ProcessLineSync("#include <iostream>");
   gROOT->ProcessLineSync("#include <string>");
   gROOT->ProcessLineSync("#include <DllImport.h>");// Defined R__EXTERN
   gROOT->ProcessLineSync("#include <vector>");  // Needed because std::vector and std::pair are
   gROOT->ProcessLineSync("#include <pair>");    //
}

void TXmlRpcShellEngine::execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP)
{
   if(fLogging){
     cout<<"TXmlRpcShellEngine: paramList Size: "<<paramList.size()<<endl;
    }
    string action;
    if(paramList.size()>0)
     {
     action=paramList.getString(0);
     if(fLogging){     cout<<"                  : paramList String Action: "<<action<<endl;  }
     }else{
       paramList.verifyEnd(1);
    }
    
    if(action == "Login")
    {
      paramList.verifyEnd(3);
      
      string user=paramList.getString(1);
      string passwd=paramList.getString(2);
      if(fLogging){     
	cout<<"                  : paramList String User: "<<user<<endl;  
      }
      Login(user,passwd,retvalP);
    }    

    if(action == "Logout")
    {
      paramList.verifyEnd(3);
      
      string user=paramList.getString(1);
      string sessionid=paramList.getString(2);
      if(fLogging){     
	cout<<"                  : paramList String User: "<<user<<endl;  
	cout<<"                  : paramList String SessionID: "<<sessionid<<endl;  
      }
      Logout(user,sessionid,retvalP);
    }    
    
    
    if(action == "ProcessLine")
    {
      paramList.verifyEnd(5);
      string user=paramList.getString(1);
      string sessionid=paramList.getString(2);
      string promptid=paramList.getString(3);
      string code=paramList.getString(4);
      if(fLogging){     
	cout<<"                  : paramList String User:      "<<user<<endl;  
	cout<<"                  : paramList String SessionID: "<<sessionid<<endl;
	cout<<"                  : paramList String PromptID:  "<<promptid<<endl;
	cout<<"                  : paramList String Code:      "<<code<<endl;	
      }

      ProcessLine(user,sessionid,promptid,code,retvalP);
    }
    
    if(action == "Interrupt")
    {
      string user=paramList.getString(1);
      string sessionid=paramList.getString(2);
      Interrupt(user,sessionid,retvalP);
    }

    if(action == "Output")
    {
      string user=paramList.getString(1);
      string sessionid=paramList.getString(2);
      Output(user,sessionid,retvalP);
    }

}
void TXmlRpcShellEngine::Interrupt(std::string user,std::string sessionid,xmlrpc_c::value *   const  retvalP)
{
  
//   if(gROOT->IsLineProcessing())
//   {
//     cout<<"Interrupt while IsLineProcessing\n;";
//     gROOT->SetInterrupt();
//   }
cout<<"Interrupt pid = "<<getpid()<<endl;
cout<<"Interrupt while IsLineProcessing\n;";
//       gROOT->SetInterrupt();
       kill(17,getpid());
      gApplication->HandleException(2);
*retvalP=xmlrpc_c::value_boolean(true); 
}

// void TXmlRpcShellEngine::execute(xmlrpc_c::paramList const& paramList,const xmlrpc_c::callInfo * const  callInfoP,xmlrpc_c::value *const resultP)
// {

// }
void TXmlRpcShellEngine::Output(std::string user,std::string sessionid,xmlrpc_c::value *   const  retvalP)
{
  std::map<std::string, xmlrpc_c::value> result;
  std::pair<string, xmlrpc_c::value> capturing("capturing", xmlrpc_c::value_boolean(gCapturing));
  std::string stdout_m;
  std::string stderr_m;
//   sleep(10);
//   if(gCapturing)
//   {
    int size=gStdout.size();
    for(int i=0;i<size;i++)
    {
      stdout_m+=gStdout.front();
      gStdout.pop_front();
    }

    size=gStderr.size();
    for(int i=0;i<size;i++)
    {
      stderr_m+=gStderr.front();
      gStderr.pop_front();
    }
   
//   }
  cout<<"CALLED OUTPUT = STDOUT = "<<stdout_m<<"\n STDERR = "<<stderr_m<<endl;
  std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(stdout_m));
  std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(stderr_m));
  result.insert(capturing);
  result.insert(stderrm);
  result.insert(stdoutm);
    
  *retvalP = xmlrpc_c::value_struct(result);
}

void TXmlRpcShellEngine::Login(std::string user,std::string passwd,xmlrpc_c::value *   const  retvalP)
{
 std::map<std::string, xmlrpc_c::value> result;
 std::pair<string, xmlrpc_c::value> statusm("status", xmlrpc_c::value_boolean(true));
 std::pair<string, xmlrpc_c::value> sessionidm("sessionid", xmlrpc_c::value_string("sid"));
 
 result.insert(statusm);
 result.insert(sessionidm);
 *retvalP=xmlrpc_c::value_struct(result); 
}

void TXmlRpcShellEngine::Logout(std::string user,std::string sessionid,xmlrpc_c::value *   const  retvalP)
{
//if user found
std::map<std::string, xmlrpc_c::value> result;
std::pair<string, xmlrpc_c::value> statusm;
std::pair<string, xmlrpc_c::value> messagem;
statusm.first="status";
messagem.first = "message";

statusm.second = xmlrpc_c::value_boolean(true);
messagem.second = xmlrpc_c::value_string("User logged out");
result.insert(statusm);
result.insert(messagem);
*retvalP=xmlrpc_c::value_struct(result);
}

void TXmlRpcShellEngine::ProcessLine(string user,string sessionid,string promptid,string code,xmlrpc_c::value *   const  retvalP)
{
  
    cout<<"ProcessLine pid = "<<getpid()<<endl;
    Int_t canvases_size;
    Int_t errorcode;
    std::vector<xmlrpc_c::value> canvases_names;
    ioHandler.clear();
    ioHandler.InitCapture();
    gROOT->SetLineIsProcessing();
     TRY {
	   gROOT->SetLineIsProcessing();
           gROOT->ProcessLine(TString(code.c_str()).ReplaceAll("\n","").Data(),&errorcode);
         } CATCH(excode) {
           Throw(excode);
         } ENDTRY;
// 	 
     gROOT->SetLineHasBeenProcessed();
    ioHandler.EndCapture();

    canvases_size=gROOT->GetListOfCanvases()->GetSize();
    for(Int_t i=0;i<canvases_size;i++)
    {
      TCanvas *c=(TCanvas*)gROOT->GetListOfCanvases()->At(i);
      canvases_names.push_back(xmlrpc_c::value_string(c->GetName()));
    }
    gROOT->GetListOfCanvases()->Print(".png");
    std::map<std::string, xmlrpc_c::value> result;
    
    std::pair<string, xmlrpc_c::value> errorcodem("errorcode", xmlrpc_c::value_int(errorcode));
    std::pair<string, xmlrpc_c::value> promptidm("promptid", xmlrpc_c::value_string(promptid));
//     std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(ioHandler.getStdout()));
//     std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(ioHandler.getStderr()));
        std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(""));
    std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(""));
    std::pair<string, xmlrpc_c::value> canvases_sizem("canvases_size", xmlrpc_c::value_int(canvases_size));
    std::pair<string, xmlrpc_c::value> canvases_namesm("canvases_names",xmlrpc_c::value_array(canvases_names));
    
    result.insert(canvases_namesm);
    result.insert(canvases_sizem);
    result.insert(errorcodem);
    result.insert(promptidm);
    result.insert(stderrm);
    result.insert(stdoutm);
    
    *retvalP = xmlrpc_c::value_struct(result);
}
