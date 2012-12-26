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

#include<TSocket.h>
#include<TServerSocket.h>
#include<TMessage.h>
#include<TRint.h>
#include<string>

class TJSRootShellClient: TObject
{
  public:
    TJSRootShellClient(Int_t port,Int_t argc,Char_t **argv,Bool_t logging=true);
    ~TJSRootShellClient(){if(sSocket) delete sSocket;}
    std::string getPipe();
    Bool_t Init();
    Bool_t tabRequest();
    Bool_t promptRequest();
    Bool_t processLineRequest(std::string code); 
    Bool_t recvStderr(std::string &msg);
    Bool_t recvStdout(std::string &msg);
    
private:
  Int_t iPort;
  TSocket *sSocket;
  Bool_t bConnected;
  Bool_t bLogging;
  Int_t iArgc;
  Char_t **cArgv;
};

#endif
