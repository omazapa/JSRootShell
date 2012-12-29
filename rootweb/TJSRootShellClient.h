/*************************************************************************
* Copyright (C) 2012,  Gfif Developers                                   *
* Grupo de Fenomenologia de Interacciones Fundamentales                  *
* http://gfif.udea.edu.co                                                *
* División de ciencias de la computación Gfifdev                         *
* http://gfifdev.udea.edu.co                                             *
* Class Created and Maintained By Omar Andres Zapata Mesa                *
* All rights reserved.                                                   *
*                                                                        *
*                                                                        *
* For the list of contributors see $ROOTSYS/README/CREDITS.              *
*************************************************************************/

#ifndef JSRoot_Shell_Client
#define JSRoot_Shell_Client

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include<TApplicationRemote.h>
#include<string>
#include"TStdIOHandler.h"
#include<TRint.h>
#include<TList.h>
#include<TFile.h>
#include<TROOT.h>
class TJSRootShellClient: TObject
{
  public:
    TJSRootShellClient(TString url,Int_t argc,Char_t **argv,Bool_t logging=true);
    ~TJSRootShellClient(){}
    Bool_t processLine(std::string code); 
    
private:
  TStdIOHandler ioHandler;
  Bool_t bLogging;
  TApplicationRemote *rApp;//for remote execution (NOT IMPLEMENTED YET)
  ofstream fCache;
};
#endif
