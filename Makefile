# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/
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
	/usr/lib/opencore/libcoremodule.a $(LIBS)

clean:
	rm -f *.o *.exe module.xml
	rm -rf awstatsmodule.app
	rm -f awstatsmodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/usr/include/opencore -c -g $<
