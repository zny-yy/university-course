/* 
 * File:   DB.h
 * Author: zhangtaihong
 *
 * Created on 2015年11月5日, 下午1:13
 */

#ifndef DB_H
#define	DB_H

#include <stdlib.h>    // exit 等
#include "OpenFileHashTable.h"

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

#endif	/* DB_H */

