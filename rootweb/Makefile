CXXFLAGS = -I/usr/include/root
LDFLAGS  = -lNet -lRint

all: capture

rootweb: rootweb.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)


capture: capture.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
