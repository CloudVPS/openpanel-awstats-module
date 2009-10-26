include makeinclude

OBJ	= main.o cfgawstats.o version.o

all: awstatsmodule.exe module.xml
	./addflavor.sh
	grace mkapp awstatsmodule 

version.cpp:
	grace mkversion version.cpp

module.xml: module.def
	mkmodulexml < module.def > module.xml

#module.xml: module.xmlsrc
#	cpp -C -P $(CXXFLAGS) module.xmlsrc | grep -v "^$$" > module.xml

awstatsmodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o awstatsmodule.exe $(OBJ) \
	../opencore/api/c++/lib/libcoremodule.a $(LIBS) -lz -lssl

clean:
	rm -f *.o *.exe module.xml
	rm -rf awstatsmodule.app
	rm -f awstatsmodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I../opencore/api/c++/include -c -g $<
