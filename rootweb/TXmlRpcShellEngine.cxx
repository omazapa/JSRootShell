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
using namespace std;

TXmlRpcShellEngineUser::TXmlRpcShellEngineUser(std::string user,std::string passwd)
{
fUser=user;
fShell=new TApplicationRemoteShell(user.c_str(),passwd.c_str());
if(IsValid())
{
TString token(user);
token+=gRandom->Integer(1e8);
fSessionID=TUUID(token.Data()).AsString();
}
}

TXmlRpcShellEngineUser::~TXmlRpcShellEngineUser()
{
    delete fShell;
}

bool TXmlRpcShellEngineUser::IsValid()
{
return fShell->IsValid(); 
}

std::string TXmlRpcShellEngineUser::GetSessionID()
{
    return fSessionID;
}

void TXmlRpcShellEngineUser::ProcessLine(std::string promptid,std::string code,xmlrpc_c::value *   const  retvalP)
{
    Int_t canvases_size;
    Int_t errorcode;
    std::vector<xmlrpc_c::value> canvases_names;
    gROOT->SetApplication(fShell);
    bool userdir=gSystem->cd(fUser.c_str());
    ioHandler.clear();
    ioHandler.InitCapture();
     TRY {
           gROOT->ProcessLine(TString(code.c_str()).ReplaceAll("\n","").Data(),&errorcode);
         } CATCH(excode) {
           Throw(excode);
         } ENDTRY;
    ioHandler.EndCapture();

    canvases_size=gROOT->GetListOfCanvases()->GetSize();
    for(Int_t i=0;i<canvases_size;i++)
    {
      TCanvas *c=(TCanvas*)gROOT->GetListOfCanvases()->At(i);
      canvases_names.push_back(xmlrpc_c::value_string(c->GetName()));
    }
    gROOT->GetListOfCanvases()->Print(".png");
    if(userdir) gSystem->cd("..");
    std::map<std::string, xmlrpc_c::value> result;
    
    std::pair<string, xmlrpc_c::value> errorcodem("errorcode", xmlrpc_c::value_int(errorcode));
    std::pair<string, xmlrpc_c::value> promptidm("promptid", xmlrpc_c::value_string(promptid));
    std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(ioHandler.getStdout()));
    std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(ioHandler.getStderr()));
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

TXmlRpcShellEngine::TXmlRpcShellEngine(int argc,char **argv,bool logging)
{
  fLogging=logging;
  this->_signature = "S:ss";  // method's arguments are two strings and return a Struct
  this->_help = "This method process c++ code using gROOT";
  gROOT->SetName("JSRootShell");
  if(gBenchmark==NULL){
    gBenchmark=new TBenchmark();
  }
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
	cout<<"                  : paramList String Passwd: "<<passwd<<endl;  
      }
      Login(user,passwd,retvalP);
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
}

void TXmlRpcShellEngine::Login(std::string user,std::string passwd,xmlrpc_c::value *   const  retvalP)
{
 ioHandler.clear();
 ioHandler.InitCapture();
 TXmlRpcShellEngineUser *euser=new TXmlRpcShellEngineUser(user,passwd);
 ioHandler.EndCapture();
 std::map<std::string, xmlrpc_c::value> result;
 std::pair<string, xmlrpc_c::value> statusm("status", xmlrpc_c::value_boolean(euser->IsValid()));
 std::pair<string, xmlrpc_c::value> sessionidm("sessionid", xmlrpc_c::value_string(euser->GetSessionID().c_str()));
 
 result.insert(statusm);
 result.insert(sessionidm);
 if(euser->IsValid())
 {
 fUsersShell[user]=euser;
 TString tuser(user);
 tuser.ReplaceAll("@","-");
 gSystem->mkdir(tuser.Data());
 }else{
 delete euser;
}
 *retvalP=xmlrpc_c::value_struct(result); 
}

void TXmlRpcShellEngine::ProcessLine(string user,string sessionid,string promptid,string code,xmlrpc_c::value *   const  retvalP)
{
//if user found
if(fUsersShell.find(user) != fUsersShell.end())
{
  //validating token id 
  if(fUsersShell[user]->GetSessionID() == sessionid)
  {
   
    if(fUsersShell[user]->IsValid())
    {
     fUsersShell[user]->ProcessLine(promptid,code,retvalP);
    }else{
      //     capture error here
    }
  }else
  {
//     capture error here
  }
}else
{
//     capture error here  
}
}
