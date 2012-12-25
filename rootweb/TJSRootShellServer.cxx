#include<TJSRootShellServer.h>

TJSRootShellServer::TJSRootShellServer(Int_t port)
{
  iPort=port;
  bConnected=false;
  sSocket=NULL;
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
    return sSocket->GetErrorCode(); 
}

Bool_t TJSRootShellServer::End()
{
  sSocket->Close();
    switch(sSocket->GetErrorCode())
    {
      case 0 :{
	cout<<"Server finished "<endl; 
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
  return return sSocket->GetErrorCode();
}

void TJSRootShellServer::Loop()
{
  while(bLoopStatus)
  {
    
  }
}
