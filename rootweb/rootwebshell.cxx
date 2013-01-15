#define WIN32_LEAN_AND_MEAN 

#include <cassert>
#include <stdexcept>
#include <iostream>
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

int main(int argc,char ** argv) {

    try {
        xmlrpc_c::registry registry;
	registry.disableIntrospection();
	
	registry.setDialect(xmlrpc_dialect_apache);


        xmlrpc_c::methodPtr const ShellEngineP(new TXmlRpcShellEngine(argc,argv,true));

        registry.addMethod("ShellEngine", ShellEngineP);
        
	 
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
	    .allowOrigin("http://localhost")
            .registryP(&registry)
            .portNumber(8082)
	    .uriPath("/rootrpcshell"));
// 	myAbyssServer.run();
	
	while(true){
	  myAbyssServer.runOnce();	  
	}
        assert(false);
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
    return 0;
}
