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

#ifndef ROOT_TXmlRpcWebServices
#define ROOT_TXmlRpcWebServices



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
#include <xmlrpc-c/client_simple.hpp>
#include "TStdIOHandler.h"
class TThread;

class TXmlRpcWebServices:public xmlrpc_c::method
{
  public:
    TXmlRpcWebServices(std::string db,std::string dbuser,std::string dbpass,std::string origin,int port);
    ~TXmlRpcWebServices();
    void execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP);
    
    void StartEngine(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP);
    void StopEngine(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP);
    void StartEngineShell(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *const  retvalP);
    static void StartEngineShellThread(void *prt);
    
private:
  std::string fDb;
  std::string fDbUser;
  std::string fDbPass;
  std::string fOrigin;
  TThread *fShellThread;
  static bool gShellStarted;
  static TStdIOHandler ioHandler;
  xmlrpc_c::clientSimple myClient;
};

#endif
