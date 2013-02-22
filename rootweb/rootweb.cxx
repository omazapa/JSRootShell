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
#include <xmlrpc-c/server_abyss.hpp>

using namespace std;

#ifdef _WIN32
  #define SLEEP(seconds) SleepEx(seconds * 1000);
#else
  #define SLEEP(seconds) sleep(seconds);
#endif

#include"TXmlRpcWebServices.h"


int main(int argc,char ** argv) {
    int port=8081;
    string server_master = "http://localhost:8081";
    string uriPath = "/rootrpcshell";
    string origin = "http://localhost";
    string db = "mysql://localhost/rootweb";
    string dbuser = "rootdbadmin";
    string dbpass = "";
    
    //options are -P=port -S=master server  -U=uriPath -O=origin(cross site authorization) -d=database -u=databaseuser -p=databasepass

  
  
    try {
        xmlrpc_c::registry registry;
	registry.setDialect(xmlrpc_dialect_apache);

        xmlrpc_c::methodPtr const WebServicesP(new TXmlRpcWebServices(db,dbuser,dbpass,origin));

        registry.addMethod("ServiceRequest", WebServicesP);
        
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
            .registryP(&registry)
            .portNumber(8081)
	    .uriPath("/rootrpcadmin"));
	myAbyssServer.run();
// 	while(true){myAbyssServer.runOnce();}
        assert(false);
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
    return 0;
}
