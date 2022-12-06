/* 
 * File:   Error.h
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午3:58
 */

#ifndef ERROR_H
#define	ERROR_H
#include <errno.h>     // errno,EEXIST等
#include <iostream>    // cerr, endl,cout,cin等

using namespace std;

// 该头文件定义了ubase项目所有的出错状态代码

// 你可以在合适的位置添加其它出错状态代码
// 但别忘了在Error.print()方法中添加对应的错误信息
enum Status {
    OK = 0,  // 一切正常，没有发生错误
 
    NOTUSED1 = -999,   // 出错状态代码的下限

    // File和DB类错误
    BADFILEPTR, BADFILE, FILETABFULL, FILEOPEN, FILENOTOPEN,
    UNIXERR, BADPAGEPTR, BADPAGENO, FILEEXISTS,

    // 缓冲管理（BufMgr）和散列表（HashTable）错误
    HASHTBLERROR, HASHNOTFOUND, BUFFEREXCEEDED, PAGENOTPINNED,
    BADBUFFER, PAGEPINNED,

    // Page类错误
    NOSPACE, NORECORDS, ENDOFPAGE, INVALIDSLOTNO, INVALIDRECLEN,

    // HeapFile错误
    BADRID, BADRECPTR, BADSCANPARM, BADSCANID, 
    SCANTABFULL, FILEEOF, FILEHDRFULL,

    // Index错误，本项目暂不实现索引
    BADINDEXPARM, RECNOTFOUND, BUCKETFULL, DIROVERFLOW,
    NONUNIQUEENTRY, NOMORERECS,

    // SortedFile错误，本项目暂不实现排序文件
    BADSORTPARM, INSUFMEM,

    // Catalog错误
    BADCATPARM, RELNOTFOUND, ATTRNOTFOUND,
    NAMETOOLONG, DUPLATTR, RELEXISTS, NOINDEX,
    INDEXEXISTS, ATTRTOOLONG,

    // Utility错误

    // Query错误
    ATTRTYPEMISMATCH, TMP_RES_EXISTS,
    
    NOTUSED2 // 出错状态代码的上限，不要删除，请在此代码之前添加其它出错状态代码
};


// Error类的声明

class Error {
public:
    void print(Status status);
};

//应当返回OK的函数调用该宏进行测试
#define CALL(c)\
{    Status s; \
     if ((s = c) != OK) { \
        cerr << "没能通过main.cpp的第" << __LINE__ << "行测试语句！" << endl; \
        cerr << "此时调用的函数为：" << #c << endl;\
        error.print(s); \
        cerr << "很遗憾，没有通过全部测试！" <<endl; \
        exit(1); \
      } \
}

//应当返回错误的函数调用该宏进行测试
#define ASSERT(c)\
{\
    if (!(c)) { \
         cerr << "没能通过main.cpp的第" << __LINE__ << "行测试语句！" << endl; \
         cerr << #c << "断言没用满足！" << endl; \
         cerr << "很遗憾，没有通过全部测试！" << endl;\
         exit(1); \
    }\
}

//应当返回错误的函数调用该宏进行测试
#define FAIL(c)\
{\
	Status s;\
	if ((s = c) == OK)\
	{\
		cerr << "没能通过main.cpp的第" << __LINE__ << "行测试语句！" << endl;\
		cerr << "调用函数" #c << "应当发生错误却返回了OK" << endl;\
		cerr << "很遗憾，不能通过测试！" <<endl;\
		exit(1);\
	}\
}

#endif	/* ERROR_H */

