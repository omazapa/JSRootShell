/*************************************************************************
* Copyright (C) 2013,  Gfif Developers                                   *
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

#ifndef JSRoot_Shell_Process_Line
#define JSRoot_Shell_Process_Line

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include"TStdIOHandler.h"

#define WIN32_LEAN_AND_MEAN 

#include <cassert>
#include <stdexcept>
#include <iostream>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>

#ifdef _WIN32
  #define SLEEP(seconds) SleepEx(seconds * 1000);
#else
  #define SLEEP(seconds) sleep(seconds);
#endif

class TRint;
class TApplicationRemote;

class TJSRootShellProcessLine:public xmlrpc_c::method
{
  public:
    TJSRootShellProcessLine(Int_t argc,Char_t **argv,Bool_t logging=true);
    void execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP);
    
    
private:
  TStdIOHandler ioHandler;
  Bool_t fLogging;
  TRint  *fShell;
};

#endif
