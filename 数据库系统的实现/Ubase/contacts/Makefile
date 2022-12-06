#
# 测试程序的Makefile文件
#

.SUFFIXES: .o .cpp

#
# 编译器、连接器定义，编译连接参数设置
#

LD =		ld
LDFLAGS =

CXX =           g++
CXXFLAGS =	-g -Wall -DDEBUG

#
# general definitions
#

MAKEFILE =      Makefile

#
# 目标文件和源文件清单
#

OBJS =  Person.o ContactHashTable.o Dictionary.o ContactApp.o
SRCS =	Person.cpp ContactHashTable.cpp Dictionary.cpp ContactApp.cpp

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

