#include"TJSRootShellProcessLine.h"
#include<iostream>
#include<map>
#include<TRint.h>
#include<TApplication.h>
using namespace std;

TJSRootShellProcessLine::TJSRootShellProcessLine(Int_t argc,Char_t **argv,Bool_t logging)
{
  fLogging=logging;
  this->_signature = "S:ss";  // method's arguments are two strings and return a Struct
  this->_help = "This method process c++ code using gROOT";
  fShell=new TRint("jsrootshell",&argc,argv,0,0,false);
}

void TJSRootShellProcessLine::execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP)
{
   if(fLogging){
     cout<<"TJSRootShellProcessLine: paramList Size: "<<paramList.size()<<endl;
     if(paramList.size()>1)
     {
     cout<<"                       : paramList String PormptID: "<<paramList.getString(0)<<endl;  
     cout<<"                       : paramList String Code:     "<<paramList.getString(1)<<endl;  
     }
    }
    
    string code;
    string promptid;
    if(paramList.size()>0)
    {
      promptid=paramList.getString(0);
      code=paramList.getString(1);
    }
    paramList.verifyEnd(2);

    ioHandler.clear();
    ioHandler.InitCapture();
    
    if(fShell->ProcessLine(code.c_str()))
    {
     
    }
//    cout<<"cout output"<<endl;
//    cerr<<"cerr output"<<endl;
    ioHandler.EndCapture();

    std::map<std::string, xmlrpc_c::value> result;
    
    std::pair<string, xmlrpc_c::value> promptidm("promptid", xmlrpc_c::value_string(promptid));
    std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(ioHandler.getStdout()));
    std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(ioHandler.getStderr()));

    result.insert(promptidm);
    result.insert(stdoutm);
    result.insert(stderrm);
    
    *retvalP = xmlrpc_c::value_struct(result);
}