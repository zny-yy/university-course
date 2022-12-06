#
# UBASE Makefile文件
#

.SUFFIXES: .o .C

#
# 编译器、连接器定义
#

LD =		ld
LDFLAGS =	

CXX =	         g++

CXXFLAGS =	-g -Wall

MAKEFILE =	Makefile

#
# 所有目标文件和源文件列表
#

OBJS =		db.o bufferManager.o page.o heapfile.o error.o \
		catalog.o print.o quit.o insert.o delete.o load.o\
		select.o join.o sort.o partition.o update.o

DBOBJS =	db.o bufferManager.o page.o heapfile.o catalog.o error.o print.o

NONCATOBJS =	db.o bufferManager.o page.o heapfile.o error.o sort.o 

SRCS =		db.cpp bufferManager.cpp page.cpp heapfile.cpp error.cpp \
		sort.cpp catalog.cpp print.cpp load.cpp\
		quit.cpp insert.cpp delete.cpp select.cpp update.cpp join.cpp ubase.cpp \
		dbcreate.cpp dbdestroy.cpp partition.cpp

LIBS =		parser.o

all:		ubase dbcreate dbdestroy

ubase:	ubase.o $(OBJS) $(LIBS)
		$(CXX) -o $@ $@.o $(OBJS) $(LIBS) $(LDFLAGS) -lm

parser.o:
		(cd parser; make)

dbcreate:	dbcreate.o $(DBOBJS)
		$(CXX) -o $@ $@.o $(DBOBJS) $(LDFLAGS) -lm

dbdestroy:	dbdestroy.o
		$(CXX) -o $@ $@.o

.C.o:
		$(CXX) $(CXXFLAGS) -c $<

clean:
		(rm -f core *.bak *~ *.o ubase dbcreate dbdestroy;cd parser;make clean)

depend:
		makedepend -I /s/gcc/include/g++ -f$(MAKEFILE) \
		$(SRCS)


# DO NOT DELETE THIS LINE -- make depend depends on it.
