# Makefile for streamcluster

TARGET=$(platform)streamcluster

include ../Makefile.inc

#CXXFLAGS := $(CXXFLAGS) -O3 -funroll-loops -fpermissive -fno-exceptions
#CXXFLAGS := $(CXXFLAGS) -static-libgcc -Wl,--hash-style=both,--as-needed

all:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) streamcluster.cpp parsec_barrier.cpp -o $(TARGET)

#%.o : %.cpp
#	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

install:
	mkdir -p ./bin
	cp -f $(TARGET) ./bin/$(TARGET)

