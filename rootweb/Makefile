CXXFLAGS = -I/usr/include/root -I. -g 
LDFLAGS  =  -L/usr/lib/x86_64-linux-gnu -lNet -lRint 
AR       = ar rcs

all: rootshellclient rootshellserver

rootshellclient: rootshellclient.cxx TJSRootShellClient.cxx  TJSRootShellClient.h
	$(CXX) $(CXXFLAGS) $< TJSRootShellClient.cxx -o $@ $(LDFLAGS) 

rootshellserver: rootshellserver.cxx TJSRootShellServer.cxx  TJSRootShellServer.h  TStdIOHandler.h
	$(CXX) $(CXXFLAGS) $< TJSRootShellServer.cxx -o $@ $(LDFLAGS) 

	
clean:
	rm -rf *~ *.o *.a rootshellclient rootshellserver