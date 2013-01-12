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
using namespace std;




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
   gROOT->SetApplication(new TApplicationRemoteShell("rootjs@gfifdev.udea.edu.co","qazsewq"));
}

void TXmlRpcShellEngine::execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP)
{
   if(fLogging){
     cout<<"TXmlRpcShellEngine: paramList Size: "<<paramList.size()<<endl;
     if(paramList.size()>1)
     {
     cout<<"                  : paramList String PormptID: "<<paramList.getString(0)<<endl;  
     cout<<"                  : paramList String Code:     "<<paramList.getString(1)<<endl;  
     }
    }
    
    string code;
    string promptid;
    Int_t canvases_size;
    std::vector<xmlrpc_c::value> canvases_names;

    if(paramList.size()>0)
    {
      promptid=paramList.getString(0);
      code=paramList.getString(1);
    }
    paramList.verifyEnd(2);

    ioHandler.clear();
    ioHandler.InitCapture();
    
     TRY {
            gROOT->ProcessLine(TString(code.c_str()).ReplaceAll("\n","").Data());
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
    
    std::pair<string, xmlrpc_c::value> promptidm("promptid", xmlrpc_c::value_string(promptid));
    std::pair<string, xmlrpc_c::value> stdoutm("stdout", xmlrpc_c::value_string(ioHandler.getStdout()));
    std::pair<string, xmlrpc_c::value> stderrm("stderr", xmlrpc_c::value_string(ioHandler.getStderr()));
    std::pair<string, xmlrpc_c::value> canvases_sizem("canvases_size", xmlrpc_c::value_int(canvases_size));
    std::pair<string, xmlrpc_c::value> canvases_namesm("canvases_names",xmlrpc_c::value_array(canvases_names));
    

    result.insert(promptidm);
    result.insert(stdoutm);
    result.insert(stderrm);
    result.insert(canvases_sizem);
    result.insert(canvases_namesm);
    
    *retvalP = xmlrpc_c::value_struct(result);
}
