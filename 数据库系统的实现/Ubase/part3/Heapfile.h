/* 
 * File:   Heapfile.h
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午3:58
 */
#ifndef HEAPFILE_H
#define HEAPFILE_H

#include "BufferManager.h"

// 两个重要对象的声明（全局外部变量）
class DB;
extern DB db;  // 声明在其他文件(不需要include)中定义的全局变量
class BufferManager;
extern BufferManager* bufMgr;

// 表名称最大长度
const unsigned MAXNAMESIZE = 72;

enum Datatype {
    STRING, INTEGER, FLOAT
}; // 表字段（属性、或列）的数据类型

enum Operator {
    LT, // <（小于，Less Than） 
    LTE, // <=（小于等于，Less Than or Equal）
    EQ, // =（等于，EQual）
    GTE, // >=（大于等于，Greater Than or Equal）
    GT, // >（大于，Greater Than）
    NE // !=（不等于，Not Equal）
}; // 扫描时使用的比较操作符

struct pageEntry {  
    int pageNo; // 页编号
    int freeSpace; // 页内空白可用字节数
}; // 目录中的项目，用于描述数据页的基本信息


// 目录的条目数量
const int DIRCNT = (PAGESIZE - 2 * sizeof (int)-MAXNAMESIZE) / sizeof (pageEntry);


// 目录页类

class DirectoryPage {
public:
    char fileName[MAXNAMESIZE]; // 表（堆文件）名称
    int recCnt; // 表中记录总数
    int pageCnt; // pages数组中已经被使用的条目数量（数据页的总数1）或者 pages数组最后被使用的元素下标0
    pageEntry pages[DIRCNT];
};


// 堆文件类声明

class HeapFile {
protected:
    File* filePtr; // I/O层文件对象
    
    DirectoryPage* directoryPage; // 缓冲池中钉住的目录页
    int directoryPageNo; // 目录页的页号
    bool directoryDirtyFlag; // 目录页是否被修改

    Page* curPage; // 当前正在处理的数据页（缓冲池中钉住的数据页）
    int curPageNo; // 当前正在处理的数据页页号
    int curIdx; // 当前正在处理的数据页在pages[]数组的下标
    bool curDirtyFlag; // 当前正在处理的数据页是否被修改
    
    RID curRec; // 当前记录的rid

public:
    HeapFile(const string &name, Status &returnStatus);
    ~HeapFile();

    // 返回文件的记录总数量

    const int getRecCnt() const {
        return directoryPage->recCnt;
    };

    // 给定一个RID，从文件读取并返回记录（包括记录指针和记录长度）
    const Status getRecord(const RID &rid, Record &rec);
};

// 堆文件扫描器类

class HeapFileScan : public HeapFile {
public:

    HeapFileScan(const string &name, Status &status) : HeapFile(name, status) {
        filter = NULL;
    };

    ~HeapFileScan() {
        endScan();
    };

    const Status startScan(const int offset,
            const int length,
            const Datatype type,
            const char* filter,
            const Operator op);

    const Status endScan() {
        return OK;
    }; // 终止扫描器

    const Status markScan(); // 保存（标记）扫描器的当前位置

    const Status resetScan(); // 恢复设置扫描器到最后一个保存的标记位置

    // 返回下一个满足扫描条件的记录RID
    const Status scanNext(RID& outRid);

    // 读取当前记录，返回记录指针和记录长度
    const Status getRecord(Record &rec) {
        return curPage->getRecord(curRec, rec);
    };

    // 删除当前记录
    const Status deleteRecord();

    // 将扫描器正在处理的当前页标记为脏页
    const Status markDirty() {
        curDirtyFlag = true;
        return OK;
    };

private:
    int offset; // 过滤条件属性的字节偏移量
    int length; // 过滤条件属性的长度
    Datatype type; // 过滤条件属性的数据类型
    const char* filter; // 过滤(筛选)条件的具体比较值
    Operator op; // 过滤条件的比较操作符

    // 以下3各数据成员用来保存调用markScan()方法时扫描器的状态
    // 之后调用resetScan()将使扫描器回滚到以下位置：
    int markedPageNo; // 扫描器最后一次保存标记的curPageNo值
    RID markedRec; // 扫描器最后一次保存标记的curRec值
    int markedIdx; // 扫描器最后一次保存标记的curIdx值

    const bool matchRec(const Record &rec) const {
        // 无过滤条件
        if (!filter) return true;

        // 检查offset + length是否超越记录尾部
        // 这种情况也许是一种错误???
        if ((offset + length - 1) >= rec.length)
            return false;

        float diff = 0; // < 0 if attr < fltr
        
        switch (type) {
            case INTEGER:
                int iattr, ifltr; // 可能存在字对齐问题
                memcpy(&iattr, (char *) rec.data + offset, length);
                memcpy(&ifltr, filter, length);
                diff = iattr - ifltr;
                break;

            case FLOAT:
                float fattr, ffltr; // 可能存在字对齐问题
                memcpy(&fattr, (char *) rec.data + offset, length);
                memcpy(&ffltr, filter, length);
                diff = fattr - ffltr;
                break;

            case STRING:
                diff = strncmp((char *) rec.data + offset, filter, length);
                break;
        }

        switch (op) {
            case LT: if (diff < 0.0) return true;
                break;
            case LTE: if (diff <= 0.0) return true;
                break;
            case EQ: if (diff == 0.0) return true;
                break;
            case GTE: if (diff >= 0.0) return true;
                break;
            case GT: if (diff > 0.0) return true;
                break;
            case NE: if (diff != 0.0) return true;
                break;
        }

        return false;
    };
};

// 堆文件插入扫描器类
class InsertFileScan : public HeapFile {
public:

    InsertFileScan(const string &name, Status &status) : HeapFile(name, status) {
        // Heapfile的构造函数应当保证目录页和第一个数据页在缓存池中
        curRec = NULLRID;
        cout << "InsertFileScan类的构造函数被执行"<< endl;
    };
    
    ~InsertFileScan();

    // 向堆文件插入记录，返回记录的RID
    const Status insertRecord(const Record &rec, RID &outRid);
};

#endif
