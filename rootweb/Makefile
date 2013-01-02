CXXFLAGS = -I/usr/include/root -I. -g -Wall
LDFLAGS  =  -L/usr/lib/x86_64-linux-gnu -lNet -lRint -lxmlrpc++ -lxmlrpc_server++ -lxmlrpc_server_abyss++ -lxmlrpc_server_pstream++ -lxmlrpc_server_abyss++ -lpthread
AR       = ar rcs

all: jsrootshell

jsrootshell: jsrootshell.cxx TJSRootShellProcessLine.cxx  TJSRootShellProcessLine.h
	$(CXX) $(CXXFLAGS) $< TJSRootShellProcessLine.cxx -o $@ $(LDFLAGS) 


clean:
	rm -rf *~ *.o *.a rootweb
run: jsrootshell
	./$<
