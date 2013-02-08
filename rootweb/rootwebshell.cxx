#define WIN32_LEAN_AND_MEAN 

#include <cassert>
#include <stdexcept>
#include <iostream>
extern "C"
{
#include <stdlib.h>  
}
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_pstream.hpp>
#include <xmlrpc-c/server_abyss.hpp>

using namespace std;

#ifdef _WIN32
  #define SLEEP(seconds) SleepEx(seconds * 1000);
#else
  #define SLEEP(seconds) sleep(seconds);
#endif

#include"TXmlRpcShellEngine.h"

#include <unistd.h>
#include <TString.h>
#include <TError.h>

int main(int argc,char ** argv)
{
    int port=8082;
    string server_master = "http://localhost:8081";
    string uriPath = "/rootrpcshell";
    string origin = "http://localhost";
    string db = "mysql://localhost/rootweb";
    string dbuser = "rootdbadmin";
    string dbpass = "";
    
    //options are -P=port -S=master server  -U=uriPath -O=origin(cross site authorization) -d=database -u=databaseuser -p=databasepass
      TString arg;
      for(int i=1;i<argc;i++)
      {
	arg=argv[i];
	if(arg[2] != '=')
	{
	  cout<<arg[2]<<endl;
	  Error("rootwebshell","Bad command line parameter in separator = ");
	  exit(1);
	}
	TString opt = arg(0,2);
	TString value = arg(3,arg.Length());
	if(opt=="-P")
	{
         port=value.Atoi();
	}
	if(opt=="-S")
	{
	  if(!value.Contains(":"))
	  {
	  Error("rootwebshell","Bad command line parameter, master server arg dont have : and port");
	  exit(1);	  
	  }
	server_master=value.Data();
	}
	if(opt=="-U")
	{
         uriPath=value.Data();
	}

	if(opt=="-O")
	{
         origin=value.Data();
	}
	
	if(opt=="-d")
	{
         db=value.Data();
	}
	
	if(opt=="-p")
	{
         dbpass=value.Data();
	 Info("rootwebshell","|%s|",dbpass.c_str());
	}

	if(opt=="-u")
	{
         dbuser=value.Data();
	}
	
      }

    try {
        xmlrpc_c::registry registry;
	registry.disableIntrospection();
	
	registry.setDialect(xmlrpc_dialect_apache);


        xmlrpc_c::methodPtr const ShellEngineP(new TXmlRpcShellEngine(db,dbuser,dbpass));

        registry.addMethod("ShellEngine", ShellEngineP);
        
	gDebug=true;
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
	    .allowOrigin(origin.c_str())
            .registryP(&registry)
            .portNumber(port)
	    .uriPath(uriPath.c_str()));
	myAbyssServer.run();
	
// 	while(true){
// 	  myAbyssServer.runOnce();	  
// 	}
        assert(false);
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
    return 0;
}
