CXX_EMBEDSAN=/home/hmatar/Research/EmbedSanitizer/arm/bin/clang++
INCS_EMBEDSAN  = -L/usr/arm-linux-gnueabi/lib -fsanitize=thread -g --std=c++11 -static
INCS_EMBEDSAN += -I/usr/arm-linux-gnueabi/include #-rpath=/usr/arm-linux-gnueabi/lib
INCS_EMBEDSAN += -I$(shell find /usr/arm-linux-gnueabi -name iostream | sed 's/\/iostream//g')
INCS_EMBEDSAN += -I$(shell find /usr/arm-linux-gnueabi/include -type d -name arm-linux-gnueabi)

all:    Register.o
	$(CXX_EMBEDSAN) Threads.cpp Register.cpp $(INCS_EMBEDSAN)


Register.o:
	$(CXX_EMBEDSAN) Register.cpp -c $(INCS_EMBEDSAN)
