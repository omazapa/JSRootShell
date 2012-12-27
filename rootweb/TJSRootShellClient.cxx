#include"TJSRootShellClient.h"
#include<iostream>

using namespace std;

TJSRootShellClient::TJSRootShellClient(Int_t port,Int_t argc,Char_t **argv,Bool_t logging)
{
  iPort=port;
  bConnected=false;
  sSocket=NULL;
  bLogging=logging;
  iArgc=argc;
  cArgv=argv;  
}

std::string TJSRootShellClient::getPipe()
{
  string pipe_msg;
  //rading 1024 bytes 
  while(true)
  {
  Char_t* buffer=new Char_t[1024];
  Int_t bsize_readed = read(STDIN_FILENO,buffer,1024);
    if(bsize_readed>0)
    {
      Char_t *realbuffer=new Char_t[bsize_readed];
      memcpy(realbuffer,buffer,bsize_readed);
      pipe_msg+=realbuffer;
      delete[] buffer;
      delete[] realbuffer;
    }else{break;}
  }
  if(bLogging){cout<<"getPipe: buffer = "<<pipe_msg<<endl;}
  return pipe_msg;
}

Int_t TJSRootShellClient::Init()
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
Bool_t TJSRootShellClient::recvStderr(std::string &msg)
{
    Int_t return_code;
    Int_t bsize;
    TMessage *msg_size=new TMessage;
    return_code = sSocket->Recv(msg_size);
        if (return_code < 0) {
         cerr<<"Error receiving TMessage with message's size in method recvStderr.\n";
         delete msg_size;
        return false;
     }
     msg_size->ReadInt(bsize);

     Char_t *stderrbuf = new Char_t[bsize];
     return_code = sSocket->RecvRaw(stderrbuf,bsize);
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

Bool_t   TJSRootShellClient::processLineRequest(std::string code)
{
    Int_t return_code;
    TMessage *msg_size=new TMessage;
    msg_size->WriteInt(code.length());
    return_code = sSocket->Send(*msg_size);
    if (return_code < 0) {
         cerr<<"Error sending TMessage with message size in method sendStderr.\n";
         delete msg_size;
        return false;
     }
 
     return_code = sSocket->SendRaw(code.c_str(),code.length());
     
     if (return_code < 0) {
         cerr<<"Error sending SendRaw's data with message size in method sendStderr.\n";
         delete msg_size;
        return false;
     }
     
    delete msg_size;
    return true;
}

Bool_t TJSRootShellClient::recvStdout(std::string &msg)
{
    Int_t return_code;
    Int_t bsize;
    TMessage *msg_size=new TMessage;
    return_code = sSocket->Recv(msg_size);
        if (return_code < 0) {
         cerr<<"Error receiving TMessage with message's size in method recvStdout.\n";
         delete msg_size;
        return false;
     }
     msg_size->ReadInt(bsize);

     Char_t *stdoutbuf = new Char_t[bsize];
     return_code = sSocket->RecvRaw(stdoutbuf,bsize);
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


