void RootWebServer()
{
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
         cout<<"RecvBufer = "<<buffer<<endl;
         break;
    }
    cout<<"RecvBufer = "<<buffer<<endl;
    gROOT->ProcessLine(buffer);

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
