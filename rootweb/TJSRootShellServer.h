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

#ifndef JSRoot_Shell_Server
#define JSRoot_Shell_Server

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include<TSocket.h>
#include<TServerSocket.h>
#include<TMessage.h>
#include<TRint.h>

#include<TStdIOCapture.h>

class TJSRootShellServer: TObject
{
  public:
    TJSRootShellServer(Int_t port);
    ~TJSRootShellServer();
    Bool_t Init();
    Bool_t End();
    void Loop();
    Bool_t tabRequest();
    Bool_t sendStderr();
    Bool_t sendStdout();
    
private:
  Int_t iPort;
  TStdIOCapture stdCapturer;
  TServerSocket *sSocket;
  Bool_t bConnected;
  Bool_t bLoopStatus;
};

#endif
