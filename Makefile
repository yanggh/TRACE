CXX = g++
RM = rm -f
CXXFLAGS = -Wall -g -std=c++11 -static
INCLUDES = -I/usr/local/include/
LDPATH = 
#LDPATH = -L/usr/local/arm/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin \
#	 -L/usr/local/arm/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/lib/gcc/arm-linux-gnueabihf/4.8.3 \

OBJS = Pack.o Reader.o  Record.o  wraptermios.o error.o  Glue.o Daemon.o JsonConf.o
PROGS = Trace

.PHONY: all
all: $(PROGS)
	@echo "build OK"
		
clean:
	$(RM) $(PROGS)  *.o
	@echo "clean OK"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)  $(LDFLAGS) $(LDPATH) 

Trace: $(OBJS)
	$(CXX) TraceMonitorApp.cpp Add_file_fix.cpp Glue.cpp Reader.cpp Pack.cpp  Record.cpp Getdiskinfo.cpp error.cpp JsonConf.cpp wraptermios.cpp Udp_recv.cpp  sock_ntop.cpp wrapsock.cpp  Daemon.cpp  -I/usr/local/include/   -pthread -std=c++11    -Wl,--no-as-needed  -o Trace
#	$(CXX) TraceMonitorApp.cpp Glue.cpp Reader.cpp Pack.cpp  Record.cpp Getdiskinfo.cpp error.cpp JsonConf.cpp wraptermios.cpp Daemon.cpp  -I/usr/local/include/   -pthread -std=c++11    -Wl,--no-as-needed  -o Trace
#	$(CXX) -o $@ Main.o $(OBJS) $(CXXFLAGS) $(INCLUDES)  $(LDFLAGS) $(LDPATH)

#Reader_test:Reader_test.o $(OBJS)
#	$(CXX) -o $@ test/Reader_test.o $(OBJS) $(CXXFLAGS) $(INCLUDES)  $(LDFLAGS) $(LDPATH)
#
#Record_test: Record_test.o
#	$(CXX) -o $@  Record.o test/Record_test.o  -std=c++11
#
#Glue_test:Glue_test.o
#	$(CXX) -o $@ Glue.o  test/Glue_test.o  -std=c++11
