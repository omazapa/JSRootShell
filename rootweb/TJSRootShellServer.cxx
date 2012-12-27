#include"TJSRootShellServer.h"
#include<TROOT.h>
#include<iostream>

using namespace std;

TJSRootShellServer::TJSRootShellServer(Int_t port,Int_t argc,Char_t **argv,Bool_t logging)
{
  iPort=port;
  bConnected=false;
  sSocket=NULL;
  bLogging=logging;
  iArgc=argc;
  cArgv=argv;
  bLoopStatus=true;
}

TJSRootShellServer::~TJSRootShellServer()
{
  if(sSocket) delete sSocket;
}

Bool_t TJSRootShellServer::Init()
{
    sSocket = new TServerSocket(iPort, kTRUE);

    switch(sSocket->GetErrorCode())
    {
      case 0 :{
	cout<<"Server started in port "<<iPort<<endl; 
	bConnected=true;
	break;
      }
      case -1 :{
	cerr<<"low level socket() call failed\n";  
	break;
      }
      case -2 :{
	cerr<<"low level bind() call failed\n";  
	break;
      }
      case -3 :{
	cerr<<"low level listen() call failed\n";  
	break;
      }
    }
    tShell = new TRint("rootwebshell",&iArgc,cArgv);
    return sSocket->GetErrorCode(); 
}

Bool_t TJSRootShellServer::End()
{
  sSocket->Close();
    switch(sSocket->GetErrorCode())
    {
      case 0 :{
	cout<<"Server finished "<<endl; 
	bConnected=true;
	break;
      }
      case -1 :{
	cerr<<"Error: low level socket() call failed\n";  
	break;
      }
      case -2 :{
	cerr<<"Error: low level bind() call failed\n";  
	break;
      }
      case -3 :{
	cerr<<"Error: low level listen() call failed\n";  
	break;
      }
    }
  return sSocket->GetErrorCode();
}

void TJSRootShellServer::Loop()
{
  Int_t return_code;
  Int_t buffer_size;
  while(true)
  {
     
     TSocket *sock = sSocket->Accept();
     TMessage *msg_size=new TMessage();
     return_code = sock->Recv(msg_size);
     if (return_code < 0) {
         cerr<<"Error receiving TMessage with message size in method Loop.\n";
         delete sock;
	 delete msg_size;
         continue;
     }
     msg_size->ReadInt(buffer_size);
     if(bLogging){cout<<"Loop: Recv(TMessage): msg_size = "<<buffer_size<<endl;}
     
     TMessage *msg=new TMessage;
     
     return_code = sock->Recv(msg);
     TString buffer;
     msg->ReadTString(buffer);
     if (return_code < 0) {
         cerr<<"Error receiving Raw data with in method Loop.\n";
         delete sock;
         delete msg_size;
         continue;
     }
    if(bLogging){cout<<"Loop: RecvRaw: buffer = "<<buffer<<endl;}
    
    ioHandler.clear();
    ioHandler.InitCapture();
    tShell->ProcessLine(buffer.Data());
    ioHandler.EndCapture();
    sendStdout(sock,ioHandler.getStdout());
    sendStderr(sock,ioHandler.getStderr());
     
     delete sock;
     delete msg_size;
  }
}

Bool_t TJSRootShellServer::sendStderr(TSocket *sock,std::string msg)
{
    Int_t return_code;
    TMessage *msg_size=new TMessage;
    msg_size->WriteInt(msg.length());
    return_code = sock->Send(*msg_size);
    if (return_code < 0) {
         cerr<<"Error sending TMessage with message size in method sendStderr.\n";
         delete msg_size;
        return false;
     }
 
     return_code = sock->SendRaw(msg.c_str(),msg.length());
     
     if (return_code < 0) {
         cerr<<"Error sending SendRaw's data with message size in method sendStderr.\n";
         delete msg_size;
        return false;
     }
     
    delete msg_size;
    return true;
}

Bool_t TJSRootShellServer::sendStdout(TSocket *sock,std::string msg)
{
    Int_t return_code;
    TMessage *msg_size=new TMessage;
    msg_size->WriteInt(msg.length());
    return_code = sock->Send(*msg_size);
    if (return_code < 0) {
         cerr<<"Error sending TMessage with message size in method sendStdout.\n";
         delete msg_size;
        return false;
     }
 
     return_code = sock->SendRaw(msg.c_str(),msg.length());
     
     if (return_code < 0) {
         cerr<<"Error sending SendRaw's data with message size in method sendStdout.\n";
         delete msg_size;
        return false;
     }
     
    delete msg_size;
    return true;
  
}



