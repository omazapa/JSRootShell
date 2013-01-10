CXXFLAGS = -I/usr/include/root -I. -g -Wall
LDFLAGS  =  -L/usr/lib/x86_64-linux-gnu -lNet -lRint -lxmlrpc++ -lxmlrpc_server++ -lxmlrpc_server_abyss++ -lxmlrpc_server_pstream++ -lxmlrpc_server_abyss++ -lpthread
AR       = ar rcs

all:rootwebshell runweb

rootwebshell: rootwebshell.cxx TXmlRpcShellEngine.cxx  TXmlRpcShellEngine.h
	$(CXX) $(CXXFLAGS) $< TXmlRpcShellEngine.cxx -o $@ $(LDFLAGS) 

rootweb:rootweb.cxx TXmlRpcWebServices.cxx TXmlRpcWebServices.h
	$(CXX) $(CXXFLAGS) $< TXmlRpcWebServices.cxx -o $@ $(LDFLAGS) 

	
clean:
	rm -rf *~ *.o *.a jsrootshell
runshell: jsrootshell
	./$<
runweb:rootweb
	./$<