# This file demonstrates how to compile the MapConverter project on Linux.
#
# To compile MapConverter do:
#
# make MapConverter
#

GPP = g++ -m32 -std=c++14 -Ilib -fno-stack-protector
GCC = gcc -m32 -Ilib -fno-stack-protector
MapConverter_OUTFILE = "./MapConverter.so"

COMPILE_FLAGS = -c -O3 -fpack-struct=1 -fPIC -w -DLINUX

MapConverter = -D MapConverter $(COMPILE_FLAGS)

all: MapConverter

clean:
	-rm -f *~ *.o *.so

MapConverter: clean
	$(GPP) $(MapConverter) ./lib/sdk/*.cpp
	$(GPP) $(MapConverter) ./lib/pugixml/*.cpp
	$(GPP) $(MapConverter) ./MapConverter/*.cpp
	$(GCC) -nodefaultlibs -fshort-wchar -shared -o $(MapConverter_OUTFILE) *.o
