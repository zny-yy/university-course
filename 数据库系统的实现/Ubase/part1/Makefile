# 
# File:   Makefile
# 功能：   测试程序的工程构建配置文件,make程序将依据此文件对项目进行编译和连接，生成可执行程序
# Author: 张太红
#
# Created on 2015年11月5日, 上午11:31
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

OBJS =  Page.o Error.o OpenFileHashTable.o File.o DB.o BufferManager.o main.o
SRCS =	Page.cpp Error.cpp OpenFileHashTable.cpp File.cpp DB.C BufferManager.cpp main.cpp

all:		BufferApp

BufferApp:	$(OBJS)
		$(CXX) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<

clean:
		rm -f core \#* *.bak *~ *.o BufferApp test.*

depend:
		makedepend -I /s/gcc/include/g++ -f$(MAKEFILE) \
		$(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

