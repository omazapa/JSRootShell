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
#include<TSQLServer.h>
#include<TSQLResult.h>
#include<TSQLRow.h>
#include<TError.h>
using namespace std;




TXmlRpcShellEngine::TXmlRpcShellEngine(string db,string dbuser,string dbpasswd)
{

  this->_signature = "S:ss";  // method's arguments are two strings and return a Struct
  this->_help = "This method process c++ code using gROOT";
  gROOT->SetName("JSRootShell");
  fDb=TSQLServer::Connect(db.c_str(),dbuser.c_str(),dbpasswd.c_str());
  if(!fDb)
  {
    Error("TXmlRpcShellEngine","Can not connect to database = %s  with user = %s",db.c_str(),dbuser.c_str());
    exit(1);
  }
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
   if(gDebug){
     cout<<"TXmlRpcShellEngine: paramList Size: "<<paramList.size()<<endl;
    }
    string action;
    if(paramList.size()>0)
     {
     action=paramList.getString(0);
     if(gDebug){     cout<<"                  : paramList String Action: "<<action<<endl;  }
     }else{
       paramList.verifyEnd(1);
    }
    
    if(action == "Login")
    {
      paramList.verifyEnd(3);
      
      string user=paramList.getString(1);
      string passwd=paramList.getString(2);
      if(gDebug){     
        cout<<"                  : paramList String User: "<<user<<endl;  
      }
      Login(user,passwd,retvalP);
    }    

    if(action == "Logout")
    {
      paramList.verifyEnd(3);
      
      string user=paramList.getString(1);
      string sessionid=paramList.getString(2);
      if(gDebug){     
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
      if(gDebug){     
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
 fStatus=false;
 fUser=user; 
 ioHandler.clear();
 ioHandler.InitCapture();
 if(fDb->IsConnected())
 {
  TSQLResult *result=fDb->Query(("select names,lastnames,email,services,enabled from rootweb.user where username='"+user+"' and passwd=PASSWORD('"+passwd+"');").c_str());
  if(result->GetRowCount()>=0)
  {
    TSQLRow* row=result->Next();
    fStatus=(Bool_t)atoi(row->GetField(4));

  }else{
    Error("TXmlRpcShellEngineUser","Bad username or password");
  }   
 }else{
   Error("TXmlRpcShellEngineUser","Database's connection is closed");
 }
 ioHandler.EndCapture();
 if(fStatus)
  {
    TString token(user);
    token+=gRandom->Uniform();
//     Info("TXmlRpcShellEngine::Login",token.Data());
    fSessionID=TUUID(token.Data()).AsString();
  }


 std::map<std::string, xmlrpc_c::value> result;
 std::pair<string, xmlrpc_c::value> statusm("status", xmlrpc_c::value_boolean(fStatus));
 std::pair<string, xmlrpc_c::value> sessionidm("sessionid", xmlrpc_c::value_string(fSessionID));
 
 result.insert(statusm);
 result.insert(sessionidm);
 if(fStatus)
 {
 TString tuser(user);
 tuser.ReplaceAll("@","-");
 gSystem->mkdir(tuser.Data());
 gSystem->cd(fUser.c_str());
 }
 *retvalP=xmlrpc_c::value_struct(result);
 Info("TXmlRpcShellEngine::Login","fUser=%s  fSessionID=%s",fUser.c_str(),fSessionID.c_str());
}

void TXmlRpcShellEngine::Logout(std::string user,std::string sessionid,xmlrpc_c::value *   const  retvalP)
{
//if user found
std::map<std::string, xmlrpc_c::value> result;
std::pair<string, xmlrpc_c::value> statusm;
std::pair<string, xmlrpc_c::value> messagem;
statusm.first="status";
messagem.first = "message";

if(user != fUser)
{
  //validating token id 
  if(fSessionID == sessionid)
  {
    statusm.second = xmlrpc_c::value_boolean(true);

    messagem.second = xmlrpc_c::value_string("User logged out");
  }else{
    statusm.second = xmlrpc_c::value_boolean(false);
    messagem.second = xmlrpc_c::value_string("Error: bad session id");
  }
}else{
    statusm.second = xmlrpc_c::value_boolean(false);
    messagem.second = xmlrpc_c::value_string("Error: user not found");
}

result.insert(statusm);
result.insert(messagem);
*retvalP=xmlrpc_c::value_struct(result);
}

void TXmlRpcShellEngine::ProcessLine(string user,string sessionid,string promptid,string code,xmlrpc_c::value *   const  retvalP)
{
//if user found
Info("USERS","in %s ex %s",user.c_str(),fUser.c_str());
Info("SESSID","in %s ex %s",sessionid.c_str(),fSessionID.c_str());
if(fUser == user)
{
  Info("HERE","%d",__LINE__);
  //validating token id 
  if(fSessionID == sessionid)
  {
    Info("HERE","%d",__LINE__);
    if(fStatus)
    {
    Int_t canvases_size;
    Int_t errorcode;
    std::vector<xmlrpc_c::value> canvases_names;
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
  }else
  {
//     capture error here
  }
}else
{
//     capture error here  
}
}
