#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<TRint.h>
#include<TSocket.h>
#include<TMessage.h>

class TIOCapture
{
public:
  TIOCapture(){
  capturing=false;  
  }
  void InitCapture()
  {
    if(!capturing)
    {
      oldstdout = std::cout.rdbuf(stdoutbuffer.rdbuf());
      oldstderr = std::cerr.rdbuf(stderrbuffer.rdbuf());
      capturing = true;
    }
  }
  
  void EndCapture()
  {
    if(capturing)
    {
      std::cout.rdbuf(oldstdout);
      std::cerr.rdbuf(oldstderr);
      capturing = false;
    }
  }
  
  std::string getStdout()
  {
   return stdoutbuffer.str(); 
  }

  std::string getStderr()
  {
   return stderrbuffer.str(); 
  }

private:
  bool capturing;
  
  std::stringstream stdoutbuffer;
  std::streambuf * oldstdout;
  
  std::stringstream stderrbuffer;
  std::streambuf * oldstderr;
};



void RootWebServer()
{
   TIOCapture cap;
   TServerSocket *ServerSocket = new TServerSocket(9090, kTRUE);
   printf("Server Started in %d\n",9090);

   
   
   TStopwatch timer;
   timer.Start();

   bool status=true;
   
   while(status)
   {
   TSocket *sock = ServerSocket->Accept();
   int ret;
   int bsize;
   TMessage *msg_size=new TMessage();
   ret = sock->Recv(msg_size);   
   msg_size->ReadInt(bsize);

   cout<<"size = "<<bsize<<endl;
   delete msg_size;
   
   char *buffer = new char[bsize];
   ret = sock->RecvRaw(buffer,bsize);
   
   if (ret < 0) {
         printf("error receiving\n");
         break;
    }
    cout<<"RecvBufer = "<<buffer<<endl;
    cap.InitCapture();
    gROOT->ProcessLine(buffer);
    cap.EndCapture();
    std::string stderr = cap.getStderr();
    TMessage *msg_stderr_size=new TMessage;
    msg_stderr_size.WriteInt(stderr.length());
    ret = sock->Send(msg_stderr_size);
    delete [] buffer;
    delete sock;
   }

   
   delete ServerSocket;
   // stop timer and print results
   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   printf("%f seconds\n", rtime);
}
