#include"TJSRootShellClient.h"
#include<iostream>
#include<TROOT.h>
#include<TSystem.h>
#include<TObjString.h>
#include<TInterpreter.h>

using namespace std;

TJSRootShellClient::TJSRootShellClient(TString url,Int_t argc,Char_t **argv,Bool_t logging)
{
  bLogging=logging;
  //loaing includes
  gROOT->ProcessLine("#include <iostream>");
  gROOT->ProcessLine("#include <string>"); // for std::string iostream.
  gROOT->ProcessLine("#include <DllImport.h>");// Defined R__EXTERN
  gROOT->ProcessLine("#include <vector>");  // Needed because std::vector and std::pair are
  gROOT->ProcessLine("#include <pair>");    // used within the core ROOT dictionaries
//   fCache.open("/tmp/jsrootcache.C",std::ios_base::app);
//   gROOT->LoadMacro("/tmp/jsrootcache.C",0,kTRUE);
}

Bool_t   TJSRootShellClient::processLine(std::string code)
{
      if(gROOT->ProcessLine(code.c_str())==0)
      {
//          fCache<<code+"\n";
// 	 fCache.flush();
// 	 fCache.close();
      }
}

