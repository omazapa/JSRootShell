void RootWebServer()
{
   TServerSocket *ServerSocket = new TServerSocket(9090, kTRUE);
   printf("Server Started in %d\n",9090);

   // Accept a connection and return a full-duplex communication socket.
   TSocket *sock = ServerSocket->Accept();
   delete ServerSocket;
   TStopwatch timer;
   timer.Start();

   bool status=true;
   
   while(status)
   {
   int bsize=1000;
//   sock->Recv(status,bsize);
   char *buf = new char[bsize];
   int ret = sock->RecvRaw(buf, bsize);
      if (ret < 0) {
         printf("error receiving\n");
         cout<<"RecvBufer = "<<buf<<endl;
         break;
      }
      cout<<"RecvBufer = "<<buf<<endl;
      delete [] buf;
   }

   delete sock;

   // stop timer and print results
   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   printf("%f seconds\n", rtime);
}
