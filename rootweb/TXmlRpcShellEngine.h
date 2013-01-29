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

class TXmlRpcShellEngineUser
{
private:
  TApplicationRemoteShell *fShell;
  std::string fSessionID;
  std::string fUser;
  TStdIOHandler ioHandler;
  TXmlRpcUserSession::EAuthMethod fAuthMathod;  
public:
  TXmlRpcShellEngineUser(std::string user,std::string passwd,TXmlRpcUserSession::EAuthMethod authmethod=TXmlRpcUserSession::EAuthMethod::kDB);
  ~TXmlRpcShellEngineUser();
  bool IsValid();
  std::string GetSessionID();
  void ProcessLine(std::string promptid,std::string code,xmlrpc_c::value *   const  retvalP);
};

class TXmlRpcShellEngine:public xmlrpc_c::method
{
  public:
    TXmlRpcShellEngine(int argc,char **argv,bool logging=true);
    void ProcessLine(std::string user,std::string sessionid,std::string promptid,std::string code,xmlrpc_c::value *   const  retvalP);
    void Login(std::string user,std::string passwd,xmlrpc_c::value *   const  retvalP);
    void Logout(std::string user,std::string sessionid,xmlrpc_c::value *   const  retvalP);
    void execute(xmlrpc_c::paramList const& paramList,xmlrpc_c::value *   const  retvalP);
    
    
private:
  TStdIOHandler ioHandler;
  bool fLogging;
  std::map<std::string,TXmlRpcShellEngineUser*> fUsersShell;
};

#endif
