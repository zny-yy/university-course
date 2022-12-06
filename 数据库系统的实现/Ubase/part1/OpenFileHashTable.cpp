/* 
 * File:   OpenFileHashTable.cpp
 * Author: 张太红
 * Created on 2015年11月5日, 上午11:35
 */

#include "OpenFileHashTable.h"

/******************************************************************
 * 以下为文件管理散列表的类OpenFileHashTable公开方法具体实现，包括：
 * 1、构造函数：OpenFileHashTbl()
 * 2、析构函数：~OpenFileHashTbl()
 * 3、散列节点插入函数：insert()
 * 4、散列节点查找函数：find()
 * 5、散列节点删除函数：erase()
 ******************************************************************/
OpenFileHashTable::OpenFileHashTable() {
    HTSIZE = 113; // hack
    // 分配文件散列的指针数组
    ht = new fileNode* [HTSIZE];
    for (size_t i = 0; i < HTSIZE; i++) ht[i] = NULL;    
}

OpenFileHashTable::~OpenFileHashTable() {
    for (size_t i = 0; i < HTSIZE; i++) {
        fileNode* tmpBuf = ht[i];
        while (ht[i]) {
            tmpBuf = ht[i];
            ht[i] = ht[i]->next;
            // 删除文件对象
            if (tmpBuf->file != NULL) delete tmpBuf->file;
            delete tmpBuf;
        }
    }
    delete [] ht;
}

// 当一个文件首次打开时，向散列表插入一个节点，如果一切正常则返回OK，否则返回HASHTBLERROR
Status OpenFileHashTable::insert(const string fileName, File* file){
    size_t index = hash(fileName);
    fileNode* tmpBuc = ht[index];
    while (tmpBuc) {
        if (tmpBuc->fname == fileName) return HASHTBLERROR;
        tmpBuc = tmpBuc->next;
    }
    tmpBuc = new fileNode;
    if (!tmpBuc) return HASHTBLERROR;
    tmpBuc->fname = fileName;
    tmpBuc->file = file;
    tmpBuc->next = ht[index];
    ht[index] = tmpBuc;

    return OK;    
}

/* 
 * 在散列表中查找名为fileName的文件是否存在，如果存在则返回OK并通过参数file返回该文件对象指针，
 * 否则返回HASHNOTFOUND
 */
Status OpenFileHashTable::find(const string fileName, File*& file){
    size_t index = hash(fileName);
    fileNode* tmpBuc = ht[index];
    while (tmpBuc) {
        if (tmpBuc->fname == fileName) {
            file = tmpBuc->file;
            return OK;
        }
        tmpBuc = tmpBuc->next;
    }
    return HASHNOTFOUND;    
}

// 当一个文件被彻底关闭时，从散列表删除该文件节点，如果一切正常则返回OK，否则返回HASHTBLERROR
Status OpenFileHashTable::erase(const string fileName){
    size_t index = hash(fileName);
    fileNode* tmpBuc = ht[index];
    fileNode* prevBuc = ht[index];

    while (tmpBuc) {
        if (tmpBuc->fname == fileName) {
            if (tmpBuc == ht[index]) ht[index] = tmpBuc->next;
            else prevBuc->next = tmpBuc->next;
            tmpBuc->file = NULL;
            delete tmpBuc;
            return OK;
        } else {
            prevBuc = tmpBuc;
            tmpBuc = tmpBuc->next;
        }
    }
    return HASHTBLERROR;  
}

/******************************************************************
 * 以下为文件管理散列表的类OpenFileHashTable私有方法具体实现，包括：
 * 1、散列函数：hash()
 ******************************************************************/

// 散列函数，返回值在0和HTSIZE-1之间，其实就是用来计算散列表数组下标的
size_t OpenFileHashTable::hash(string fileName){
    size_t len = (size_t) fileName.length();
    size_t value = 0;

    for (size_t i = 0; i < len; i++) {
        value = 31 * value + (size_t) fileName[i];
    }
    value = value % HTSIZE;
    return value;    
}