/* 
 * File:   BufferManager.h
 * Author: 张太红
 * Created on 2015年11月12日, 下午5:31
 */

#ifndef BUFFERMANAGER_H
#define	BUFFERMANAGER_H

#include "DB.h"
#include <cmath>    //sqrt()

//判断一个正整数是否为素数

static bool isPrime(int number) {
    if (number <= 3) return true;
    int sq_root = sqrt(number);
    for (int i = 2; i < sq_root; i++) {
        if ((number % i) == 0) return false;
    }
    return true;
}

// 缓存页帧散列节点
struct BufferPageNode {
    File* file; // 文件对象指针
    int pageNo; // 文件中的页号
    int frameNo; // 缓冲池中的帧号
    BufferPageNode* next; // 指向下一个节点的指针
};

// 管理已用缓冲帧的散列表

class BufferHashTable {
    friend class BufferManager;
public:

private:
    int HTSIZE;
    BufferPageNode** ht; // 散列表

    BufferHashTable(const int htSize); // 构造函数
    ~BufferHashTable(); // 析构函数

    int hash(const File* file, const int pageNo); // 返回0到HTSIZE-1之间的整数

    // 向散列表插入映射(file,pageNo)到frameNo的散列节点
    // 成功返回OK，发生错误返回HASHTBLERROR
    Status insert(const File* file, const int pageNo, const int frameNo);

    // 检查(file,pageNo)是否在缓冲池中（是否在散列表中)
    // 如果在，返回OK并通过参数frameNo返回对应的帧号。否则返回HASHNOTFOUND
    Status lookup(const File* file, const int pageNo, int &frameNo);

    // 如果(file,pageNo)节点存在，删除散列节点并返回OK，否则返回HASHTBLERROR
    Status remove(const File* file, const int pageNo);
};

// 维护缓冲池每个帧的信息类

class BufferInfo {
    friend class BufferManager;

private:
    File* file; // 文件对象指针
    int pageNo; // 文件中的页号
    int frameNo; // 缓冲池中的帧号
    int pinCnt; // 摁钉数量
    bool empty; // 是否为空闲帧，true：空闲帧，false：以被占用的帧
    bool dirty; // 修改状态，true：内容被修改过，false：内容没有被修改过
    bool refbit; // 访问状态,reference bit：访问位、引用位

    // 初始化帧的描述信息

    void Clear() {
        pinCnt = 0;
        file = NULL;
        pageNo = -1;
        empty = true;
        dirty = false;
        refbit = false;
    };

    // 类构造函数的实现

    BufferInfo() {
        Clear();
    }

    // 设置帧的描述信息

    void Set(File* filePtr, const int pageNum) {
        file = filePtr;
        pageNo = pageNum;
        pinCnt = 1;
        empty = false;
        dirty = false;
        refbit = true;
    }
};

class BufferManager {
public:
    Page* bufPool; // 缓冲池
    BufferManager(const int bufs); // 构造函数
    virtual ~BufferManager(); // 析构函数

    // 读页（注意：与File.h中的readPage进行比较，
    // 可知是利用File中的readPage进行读页的，也就是重新包装了一下该函数
    // 从而利用缓存管理器统一管理读入内存的页，而不是用File随便将也放入内存
    const Status readPage(File* file, const int pageNo, Page* &page);
    // 解除摁钉
    const Status unPinPage(const File* file, const int pageNo, const bool dirty);

    // 分配新页
    const Status allocPage(File* file, int &pageNo, Page* &page);
    
    // 释放页帧，使其成为空闲页帧
    const Status disposePage(File* file, const int pageNo);

    // 提交所有修改，即将所有修改过的页写回磁盘文件
    const Status commit();

private:
    int clockHand; // 时钟指针 
    int bufferNumbers; // 缓冲池总帧数
    BufferHashTable* bufferhashTable; // 已用缓冲帧的散列表
    BufferInfo* bufferInfo; // 帧信息数组

    // 时钟走一步
    void advanceClock() {
        clockHand = (clockHand + 1) % bufferNumbers;
    }
    
    // 按时钟替代策略在缓冲池中选择一帧
    const Status chooseFrame(int &frame);

    // 计算>=n的z最小素数，用于管理已用缓冲帧散列表ht的桶数（HTSIZE）
    int prime(int n) {
        while (!isPrime(n)) n++;
        return n;
    };
};

#endif	/* BUFFERMANAGER_H */