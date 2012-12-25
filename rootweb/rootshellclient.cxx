#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<TRint.h>
#include<TSocket.h>
#include<TMessage.h>
#include<stdlib.h>
using namespace std;


int main(int argc,char *argv[])
{
  if(argc == 2)
  {
  int bsize = atoi(argv[1]);
  int bsize_readed = 0;
  char buffer[bsize];
  bsize_readed = read(STDIN_FILENO,buffer,bsize);
  TSocket *sock = new TSocket("localhost", 9090);
  
  char *sock_buffer = new char[bsize_readed];
  
  memcpy(sock_buffer,buffer,bsize_readed);
  
  TMessage msg_size;
  msg_size.WriteInt(bsize_readed);
  int ret = sock->Send(msg_size);
  
  ret = sock->SendRaw(sock_buffer,bsize_readed);
  
  for(int i=0;i<2;i++)
  {
    TMessage *rmsg_size=new TMessage();
     ret = sock->Recv(rmsg_size);
     if (ret< 0) {
         cerr<<"Error receiving TMessage with message size in method Loop.\n";
         delete sock;
	 delete rmsg_size;
     }
     rmsg_size->ReadInt(bsize);
     
     char *bufferr = new char[bsize];
     ret = sock->RecvRaw(bufferr,bsize);
     
     if (ret< 0) {
         cerr<<"Error receiving Raw data with in method Loop.\n";
         delete sock;
         delete rmsg_size;
         delete bufferr;
     }  
  cout<<bufferr<<" "<<bsize_readed<<endl;      
  }

  
  
  }
  
  return 0;
}
