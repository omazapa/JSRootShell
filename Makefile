CXXFLAGS = -I/usr/include/root -I/opt/xmlrpc/include -I. -g -Wall
LDFLAGS  =  -L/usr/lib/x86_64-linux-gnu -L/opt/xmlrpc/lib -lNet -lRint -lxmlrpc  -lxmlrpc++  -lxmlrpc_abyss  -lxmlrpc_client  -lxmlrpc_client++  -lxmlrpc_cpp  -lxmlrpc_packetsocket  -lxmlrpc_server  -lxmlrpc_server++  -lxmlrpc_server_abyss  -lxmlrpc_server_abyss++  -lxmlrpc_server_cgi  -lxmlrpc_server_cgi++  -lxmlrpc_server_pstream++  -lxmlrpc_util  -lxmlrpc_xmlparse  -lxmlrpc_xmltok -lpthread  
#-lxmlrpc++ -lxmlrpc_server++ -lxmlrpc_server++ -lxmlrpc_server_abyss++ -lxmlrpc_packetsocket -lxmlrpc_server_pstream++ -lxmlrpc_server_abyss++ -lpthread
AR       = ar rcs
SRCDIR   = rootweb

all:$(SRCDIR)/rootwebshell

$(SRCDIR)/rootwebshell: $(SRCDIR)/rootwebshell.cxx $(SRCDIR)/TXmlRpcShellEngine.cxx $(SRCDIR)/TApplicationRemoteShell.cxx $(SRCDIR)/TXmlRpcShellEngine.h
	$(CXX) $(CXXFLAGS) $< $(SRCDIR)/TXmlRpcShellEngine.cxx $(SRCDIR)/TApplicationRemoteShell.cxx -o $@ $(LDFLAGS) 

$(SRCDIR)/rootweb:$(SRCDIR)/rootweb.cxx $(SRCDIR)/TXmlRpcWebServices.cxx  $(SRCDIR)/TXmlRpcWebServices.h
	$(CXX) $(CXXFLAGS) $< $(SRCDIR)/TXmlRpcWebServices.cxx -o $@ $(LDFLAGS) 

	
clean:
	find . -name "*~" -exec rm '{}' \;
	find . -name "*.o" -exec rm '{}' \;
	rm -f $(SRCDIR)/*.png
	rm -f $(SRCDIR)/rootweb $(SRCDIR)/rootwebshell

run:runwebshell
	
runwebshell: $(SRCDIR)/rootwebshell
	LD_LIBRARY_PATH=/opt/xmlrpc/lib/ ./$<
# runweb:$(SRCDIR)/rootweb
# 	LD_LIBRARY_PATH=/opt/xmlrpc/lib/ ./$<
