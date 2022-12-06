/* 
 * File:   OpenFileHashTable.h
 * Author: 张太红
 *
 * Created on 2015年11月5日, 上午11:35
 */

#ifndef OPENFILEHASHTABLE_H
#define	OPENFILEHASHTABLE_H

#include "File.h"

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

#endif	/* OPENFILEHASHTABLE_H */