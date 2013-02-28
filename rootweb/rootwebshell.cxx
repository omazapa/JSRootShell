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
#include <signal.h>

int main(int argc,char ** argv) {

    InitInterruptHandler();
    try {
        xmlrpc_c::registry registry;
// 	registry.disableIntrospection();
	
// 	registry.setDialect(xmlrpc_dialect_apache);


        xmlrpc_c::methodPtr const ShellEngineP(new TXmlRpcShellEngine(argc,argv,true));

        registry.addMethod("ShellEngine", ShellEngineP);
	
        
	 
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
	    .allowOrigin("http://localhost")
// 	    .chunkResponse(true)
            .registryP(&registry)
            .portNumber(8082)
// 	    .keepaliveTimeout(3600)
// 	    .timeout(3600)
// 	    .accessCtlMaxAge(3600)
	    .keepaliveMaxConn(1e9)//este parametro es supor importante para que no se creen multiples instancias y se pierdan las variables globales.
	    .serverOwnsSignals(false)//this is the configutation .serverOwnsSignals(false) .expectSigchld(true) to captured our own signal
	    .expectSigchld(true)
	    .uriPath("/rootrpcshell"));
	std::cout<<"Server Started"<<std::endl;
// 	myAbyssServer.run();
	while(true){
// 	 pid_t childPID = fork();
// 
        myAbyssServer.runOnce();	  
// 	   
// 	if(childPID >= 0) // fork was successful
//         {
// 	    if(childPID == 0) // child process
// 	    {
//              myAbyssServer.runOnce();	  
// 	    }
// 	    else //Parent process
//             {
// 	       myAbyssServer.runOnce();	  
//             }
//         }
//         else // fork failed
//        {
//         printf("\n Fork failed, quitting!!!!!!\n");
//         return 1;
//         }
	}
        assert(false);
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
    return 0;
}
