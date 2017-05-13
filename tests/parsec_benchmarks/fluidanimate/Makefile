include ../Makefile.inc

TARGET   = $(platform)fluidanimate
OBJS     = pthreads.o cellpool.o parsec_barrier.o

CXXFLAGS += -Wno-invalid-offsetof
CXXFLAGS += -pthread -D_GNU_SOURCE -D__XOPEN_SOURCE=600

# To enable visualization comment out the following lines (don't do this for benchmarking)
#OBJS     += fluidview.o
#CXXFLAGS += -DENABLE_VISUALIZATION
#LIBS     += -lglut

all: clean pthreads fluidcmp

pthreads: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $(TARGET)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

fluidcmp: fluidcmp.cpp
	rm -rf fluidcmp
	$(CXX) $(CXXFLAGS) fluidcmp.cpp -o fluidcmp

clean:
	rm -rf $(TARGET)
	rm -rf fluidcmp
	rm -rf *.o
