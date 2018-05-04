CXX = g++
#CXX = arm-linux-gnueabihf-g++
RM = rm -f
CXXFLAGS = -Wall -g -std=c++11 -static
INCLUDES = 
LDFLAGS =
LDPATH = 
#LDPATH = -L/usr/local/arm/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin \
#	 -L/usr/local/arm/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/lib/gcc/arm-linux-gnueabihf/4.8.3 \

OBJS = Pack.o Reader.o  Record.o  wraptermios.o error.o  Glue.o 
PROGS = Demo  Reader_test Record_test Glue_test

.PHONY: all
all: $(PROGS)
	@echo "build OK"
		
clean:
	$(RM) $(PROGS)  *.o
	@echo "clean OK"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)  $(LDFLAGS) $(LDPATH) 

Demo: Main.o $(OBJS)
	$(CXX) -o $@ Main.o $(OBJS) $(CXXFLAGS) $(INCLUDES)  $(LDFLAGS) $(LDPATH)

Reader_test:Reader_test.o $(OBJS)
	$(CXX) -o $@ Reader_test.o $(OBJS) $(CXXFLAGS) $(INCLUDES)  $(LDFLAGS) $(LDPATH)

Record_test: Record_test.o
	$(CXX) -o $@  Record.o Record_test.o  -std=c++11

Glue_test:Glue_test.o
	$(CXX) -o $@ Glue.o  Glue_test.o  -std=c++11