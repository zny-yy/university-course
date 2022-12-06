/* 
 * File:   db.h
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */
#ifndef DB_H
#define DB_H

#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <fcntl.h>     // ::open, ::close等
#include <stdlib.h>    // exit()
#include <string.h>
#include "error.h"
#include "page.h"
using namespace std;

class BufferManager;
extern BufferManager* bufMgr;

// 文件头页结构
typedef struct {
    int nextFree; // 指向第一个空白页
    int firstPage; // 指向第一个数据页
    int numPages; // 文件总页数
} HeadPage;

#define HP(p)      (*(HeadPage*)&p) //强制将数据页转换为头页的宏定义

// forward class definition for db
class DB;

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

struct fileNode {
    string fname; // 文件名
    File* file; // 指向文件对象的指针
    fileNode* next; // 指向散列表中溢出节点的指针

};

// 管理打开文件的散列表对象
class OpenFileHashTable {
    friend class DB;
public:
  
private:
    OpenFileHashTable();
    virtual ~OpenFileHashTable();
    // 当一个文件首次打开时，向散列表插入一个节点，如果一切正常则返回OK，否则返回HASHTBLERROR
    Status insert(const string fileName, File* file);

    /* 
     * 在散列表中查找名为fileName的文件是否存在，
     * 如果存在则返回OK并通过参数file返回该文件对象指针，否则返回HASHNOTFOUND
     */
    Status find(const string fileName, File* &file);

    /* 
     * 当一个文件被彻底关闭时，从散列表删除该文件节点，
     * 如果一切正常则返回OK，否则返回HASHTBLERROR
     */
    Status erase(const string fileName);
    
    size_t HTSIZE; // 散列表桶数
    fileNode** ht; // 散列表，用指针数组实现
    // 散列函数，返回值在0和HTSIZE-1之间，其实就是用来计算散列表数组下标的
    size_t hash(string fileName); 
};



class DB {
public:
    DB(); // 构造函数
    ~DB(); // 析构函数

    const Status createFile(const string &fileName); // 创建文件，完成后关闭文件

    const Status destroyFile(const string &fileName); // 删除文件

    const Status openFile(const string &fileName, File* &file); // 打开文件

    const Status closeFile(File* &file); // 关闭文件

private:
    OpenFileHashTable openFiles; // 管理打开文件的散列表
};
#endif
