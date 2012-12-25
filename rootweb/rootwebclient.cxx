#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<TRint.h>
#include<TSocket.h>
#include<TMessage.h>

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
  cout<<buffer<<" "<<bsize_readed<<endl;
  }
  
  return 0;
}
