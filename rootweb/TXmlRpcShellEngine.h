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
#include <map>
#include <string>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>

#include<map>

#include"TXmlRpcSession.h"

class TApplicationRemoteShell;
class TSQLServer;


class TXmlRpcShellEngine:public xmlrpc_c::method
{
  public:
    TXmlRpcShellEngine(std::string db,std::string dbuser,std::string dbpasswd);
    void ProcessLine(std::string user,std::string sessionid,std::string promptid,std::string code,xmlrpc_c::value *   const  retvalP);
    void Login(std::string user,std::string passwd,xmlrpc_c::value *  const  retvalP);
    void Logout(std::string user,std::string sessionid,xmlrpc_c::value *   const  retvalP);
    void execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP);
    
private:
  TSQLServer *fDb;
  TStdIOHandler ioHandler;
  bool fStatus;
  std::string fUser;
  std::string fSessionID;
};

#endif
