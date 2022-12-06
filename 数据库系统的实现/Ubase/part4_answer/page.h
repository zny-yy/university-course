/* 
 * File:   page.h
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午4:54
 */

#ifndef PAGE_H
#define PAGE_H
#include <stdio.h>      // printf()
#include <string.h>     //memset(), memcpy()等,
#include "error.h"

// 记录ID结构

struct RID {
    int pageNo;
    int slotNo;
};

const RID NULLRID = {-1, -1};

// 记录结构

struct Record {
    void* data;
    int length;
};

// 插槽结构

struct slot_t {
    short offset;
    short length; // 未使用的插槽该值为-1
};

const unsigned PAGESIZE = 4096; // 页大小
const unsigned DPFIXED = sizeof (slot_t) + 4 * sizeof (short) + 2 * sizeof (int);

/*
 * UBase数据页类的定义
 * 要求执行删除操作后记录要压缩，但插槽数组不能随意压缩
 * 支持存储变长记录，不关心记录内部字段的结构
 * 字段结构由UBase上层模块负责处理
 */
class Page {
private:
    char data[PAGESIZE - DPFIXED];
    slot_t slot[1]; // 插槽数组，该数组反向增长！
    short slotCnt; // 已经使用的插槽数量
    short freePtr; // data[]中可用空间的起始位置 
    short freeSpace; // data[]中可用空间的字节数
    short dummy; // 用于4字节对齐
    int nextPage; // 下一页编号（用来形成数据页链表）
    int curPage; // 本页的编号

public:

    //  新页初始化

    void init(const int pageNo) {
        memset(data, 0, PAGESIZE - DPFIXED); // 清空data[]
        nextPage = -1;
        slotCnt = 0; // 已经使用的插槽数量
        curPage = pageNo;
        freePtr = 0; // data[]中可用空间的起始位置
        freeSpace = PAGESIZE - DPFIXED + sizeof (slot_t); // 可用空间（以字节为单位）
        dummy = 0; // 避免编译警告信息
    };

    // 输出本页信息，用于测试和调试

    void dumpPage() const {
        int i;

        // 输出记录内容
        cout << (char*) data << endl;

        // 输出插槽数组
        for (i = slotCnt + 1; i <= 0; i++)
            cout << "slot[" << i << "].offset=" << slot[i].offset << "\t" \
                    << "slot[" << i << "].length=" << slot[i].length << endl;

        // 输出本页其它信息
        cout << "slotCnt=" << slotCnt << "\t" << "freePtr=" << freePtr << endl;
        cout << "freeSpace=" << freeSpace << "\t" << "dummy=" << dummy << endl;

        cout << "nextPage=" << nextPage << endl;
        cout << "curPage=" << curPage << endl;


    };

    // 返回nextPage的值

    const Status getNextPage(int &pageNo) const {
        pageNo = nextPage;
        return OK;
    };

    // 设置nextPage值为pageNo

    const Status setNextPage(const int pageNo) {
        nextPage = pageNo;
        return OK;
    };

    // 返回freeSpace的值

    short getFreeSpace() const {
        return this->freeSpace;
    };

    // 将新记录rec插入页中，通过rid返回记录的RID，一切正常则返回OK
    // 如果本页无法容纳要插入的记录请返回NOSPACE
    const Status insertRecord(const Record &rec, RID &rid);
    
    // 用rec更新记录rid
    const Status updateRecord(const Record &rec, const RID &rid);

    // 删除由rid指定的记录，一切正常则返回OK
    // 压缩剩余的记录，请使用bcopy函数进行压缩操作
    const Status deleteRecord(const RID &rid);

    // 通过firstRid返回本页的第一条记录的RID
    // 如果本页无记录则返回NORECORDS否则返回OK
    const Status firstRecord(RID &firstRid) const;

    // 通过nextRid返回本页相对于curRid的下一条记录的RID
    // 如果curRid已经是本页的最后一条记录则返回ENDOFPAGE，否则返回OK
    const Status nextRecord(const RID &curRid, RID &nextRid) const;

    // 返回rid记录的rec（包含记录长度和记录指针）
    const Status getRecord(const RID &rid, Record &rec);

    // 用于测试

    short getFreePtr() {
        return freePtr;
    };

    // 用于测试

    short getSlotCnt() {
        return slotCnt;
    };

    // 用于测试

    short getOffset(short index) {
        return this->slot[index].offset;
    };

    // 用于测试

    short getLength(short index) {
        return this->slot[index].length;
    };
};

#endif