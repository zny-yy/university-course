#
# Makefile文件
# 作者：张太红
#

.SUFFIXES: .o .c .cpp

#
# 编译器、连接器定义，编译连接参数设置
#

LD =		ld
LDFLAGS =       -ldl -lpthread

CXX =           g++
CXXFLAGS =	-g -Wall -DDEBUG

#
# general definitions
#

MAKEFILE =      Makefile

#
# 目标文件和源文件清单
#

OBJS =  DB.o sqlite3.o ContactApp.o
SRCS =	DB.cpp sqlite3.c ContactApp.C

all:		ContactApp

ContactApp:	$(OBJS)
		$(CXX) -o $@ $(OBJS) $(LDFLAGS)

.C.o:
		$(CXX) $(CXXFLAGS) -c $<

clean:
		rm -f core \#* *.bak *~ *.o ContactApp

depend:
		makedepend -I /s/gcc/include/g++ -f$(MAKEFILE) \
		$(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

