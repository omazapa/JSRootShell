CXXFLAGS = -I/usr/include/root -I. -g -Wall
LDFLAGS  =  -L/usr/lib/x86_64-linux-gnu -lNet -lRint -lxmlrpc++ -lxmlrpc_server++ -lxmlrpc_server_abyss++ -lxmlrpc_server_pstream++ -lxmlrpc_server_abyss++ -lpthread
AR       = ar rcs
SRCDIR   = rootweb

all:$(SRCDIR)/rootwebshell $(SRCDIR)/rootweb

$(SRCDIR)/rootwebshell: $(SRCDIR)/rootwebshell.cxx $(SRCDIR)/TXmlRpcShellEngine.cxx $(SRCDIR)/TApplicationRemoteShell.cxx $(SRCDIR)/TXmlRpcShellEngine.h
	$(CXX) $(CXXFLAGS) $< $(SRCDIR)/TXmlRpcShellEngine.cxx $(SRCDIR)/TApplicationRemoteShell.cxx -o $@ $(LDFLAGS) 

$(SRCDIR)/rootweb:$(SRCDIR)/rootweb.cxx $(SRCDIR)/TXmlRpcWebServices.cxx  $(SRCDIR)/TXmlRpcWebServices.h
	$(CXX) $(CXXFLAGS) $< $(SRCDIR)/TXmlRpcWebServices.cxx -o $@ $(LDFLAGS) 

	
clean:
	find . -name "*~" -exec rm '{}' \;
	find . -name "*.o" -exec rm '{}' \;
	rm -f $(SRCDIR)/rootweb $(SRCDIR)/rootwebshell
	
runwebshell: $(SRCDIR)/jsrootshell
	./$<
runweb:$(SRCDIR)/rootweb
	./$<
