#include"TJSRootShellClient.h"
#include<iostream>

using namespace std;

TJSRootShellClient::TJSRootShellClient(Int_t port,Int_t argc,Char_t **argv,Bool_t logging=true)
{
  iPort=port;
  bConnected=false;
  sSocket=NULL;
  bLogging=logging;
  iArgc=argc;
  cArgv=argv;  
}


Bool_t TJSRootShellClient::Init()
{
    sSocket = new TSocket("localhost",iPort, kTRUE);

    switch(sSocket->GetErrorCode())
    {
      case 0 :{
	cout<<"Server started in port "<<iPort<<endl; 
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


Bool_t TJSRootShellClient::tabRequest()
{
  
}
Bool_t TJSRootShellClient::promptRequest()
{
  
}
Bool_t TJSRootShellClient::recvStderr(TSocket *sock,std::string &msg)
{
    Int_t return_code;
    Int_t bsize;
    TMessage *msg_size=new TMessage;
    return_code = sock->Recv(msg_size);
        if (return_code < 0) {
         cerr<<"Error receiving TMessage with message's size in method recvStderr.\n";
         delete msg_size;
        return false;
     }
     msg_size->ReadInt(bsize);

     Char_t *stderrbuf = new Char_t[bsize];
     return_code = sock->RecvRaw(stderrbuf,bsize);
     if (return_code < 0) {
         cerr<<"Error receiving SendRaw's data with stderr content in method recvStderr.\n";
         delete msg_size;
	 delete[] stderrbuf;
        return false;
     }
     msg=stderrbuf;
    delete msg_size;
    delete[] stderrbuf;
    return true;
}
Bool_t TJSRootShellClient::recvStdout(TSocket *sock,std::string &msg)
{
    Int_t return_code;
    Int_t bsize;
    TMessage *msg_size=new TMessage;
    return_code = sock->Recv(msg_size);
        if (return_code < 0) {
         cerr<<"Error receiving TMessage with message's size in method recvStdout.\n";
         delete msg_size;
        return false;
     }
     msg_size->ReadInt(bsize);

     Char_t *stdoutbuf = new Char_t[bsize];
     return_code = sock->RecvRaw(stdoutbuf,bsize);
     if (return_code < 0) {
         cerr<<"Error receiving SendRaw's data with stderr content in method recvStdout.\n";
         delete msg_size;
	 delete[] stdoutbuf;
        return false;
     }
     msg=stdoutbuf;
    delete msg_size;
    delete[] stdoutbuf;
    return true;  
}


