/* 
 * File:   File.h
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午4:18
 */

#ifndef FILE_H
#define	FILE_H

#include <fcntl.h>     // ::open, ::close等
#include <unistd.h>
#include <stdio.h>     // ::remove
#include "Page.h"


// 文件头页结构

typedef struct {
    int nextFree; // 指向第一个空白页
    int firstPage; // 指向第一个数据页
    int numPages; // 文件总页数
} HeadPage;

#define HP(p)      (*(HeadPage*)&p) //强制将数据页转换为头页的宏定义

class File {
    friend class DB;
    friend class OpenFileHashTable;
public:
    const Status getFirstPage(int &pageNo) const; // 返回第一页的页号
    const string getFileName(); //返回文件名称
    const Status allocatePage(int &pageNo); // 分配一个新页
    // 从文件中将一页读入内存
    const Status readPage(const int pageNo, Page* pagePtr) const;
    // 将内存中的一页写入文件
    const Status writePage(const int pageNo, const Page* pagePtr);
    const Status disposePage(const int pageNo); // 释放一页
    void listFree(); // 列出文件中所有空白页，用于调试

    bool operator==(const File &other) const; //文件对象的相等比较操作符的实现

private:
    File(const string &fname); // 构造函数
    virtual ~File(); // 析构函数

    static const Status create(const string &fileName); // 创建文件
    static const Status destroy(const string &fileName); // 删除文件

    const Status open(); // 打开文件
    const Status close(); // 关闭文件

    string fileName; // 文件名称
    int openCnt; // 文件打开的次数
    int unixFile; // 文件的unix系统编号
};

#endif	/* FILE_H */

