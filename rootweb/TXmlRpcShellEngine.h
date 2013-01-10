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

#ifndef ROOT_TXmlRpcShellEngine
#define ROOT_TXmlRpcShellEngine


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

class TXmlRpcShellEngine:public xmlrpc_c::method
{
  public:
    TXmlRpcShellEngine(int argc,char **argv,bool logging=true);
    void execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP);
    
    
private:
  TStdIOHandler ioHandler;
  bool fLogging;
};

#endif
