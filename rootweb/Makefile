CXXFLAGS = -I/usr/include/root -I.
LDFLAGS  = -lNet -lRint

all: rootshellclient rootshellserver

rootshellclient: rootshellclient.cxx TJSRootShellServer.cxx  TJSRootShellServer.h  TStdIOHandler.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

rootshellserver: rootshellserver.cxx TJSRootShellServer.cxx  TJSRootShellServer.h  TStdIOHandler.h
	$(CXX) $(CXXFLAGS) rootshellserver.cxx TJSRootShellServer.cxx -o $@ $(LDFLAGS)

