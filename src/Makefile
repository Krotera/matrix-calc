CXX=g++
RM=rm -f
CXXFLAGS= -Wall -g -std=c++11
LDFLAGS= -lm
 
SRCS=main.cpp matrix.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

.PHONY: all clean distclean

all: matrix_calc
 
matrix_calc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o matrix_calc $(LDFLAGS) $(OBJS)

main.o: matrix.h main.cpp

matrix.o: matrix.h matrix.cpp


clean:
	$(RM) *.o

distclean: clean
	$(RM) matrix_calc