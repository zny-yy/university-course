/* 
 * File:   DB.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月5日, 下午1:13
 */

#include "DB.h"

/******************************************************************
 * 以下为数据库类DB的具体实现，包括：
 * 1、构造函数：DB()
 * 2、析构函数：~DB()
 * 3、创建文件函数：createFile()
 * 4、删除文件函数：destroyFile()
 * 5、打开文件函数：openFile()
 * 6、关闭文件函数：closeFile()
 ******************************************************************/


/*
 * 构造函数
 * 用来构造一个数据库对象，该对象负责在一个数据库中创建、打开、关闭、删除文件等
*/
DB::DB() {
    // Check that DB header page data fits on a regular data page.

    if (sizeof (HeadPage) >= sizeof (Page)) {
        cerr << "头页和数据页大小不一致: ";
        cerr << sizeof (HeadPage) << " " << sizeof (Page) << endl;
        exit(1);
    }
}


// 析构函数.
DB::~DB() {
    // this could leave some open files open.
    // need to fix this by iterating through the hash table deleting each open file
}



// 创建一个文件

const Status DB::createFile(const string &fileName) {
    File* file;
    if (fileName.empty())
        return BADFILE;

    // First check if the file has already been opened
    if (openFiles.find(fileName, file) == OK) return FILEEXISTS;
    // Do the actual work
    return File::create(fileName);
}


// 删除一个文件

const Status DB::destroyFile(const string &fileName) {
    File* file;

    if (fileName.empty()) return BADFILE;

    // Make sure file is not open currently.
    if (openFiles.find(fileName, file) == OK) return FILEOPEN;

    // Do the actual work
    return File::destroy(fileName);
}


// Open a database file. If file already open, increment open count,
// otherwise find a vacant slot in the open files table and store
// file info there.

const Status DB::openFile(const string &fileName, File* &filePtr) {
    Status status;
    File* file;

    if (fileName.empty()) return BADFILE;

    // Check if file already open.
    if (openFiles.find(fileName, file) == OK) {
        // file is already open, call open again on the file object
        // to increment it's open count.
        status = file->open();
        filePtr = file;
    } else {
        // file is not already open
        // Otherwise create a new file object and open it
        filePtr = new File(fileName);
        status = filePtr->open();

        if (status != OK) {
            delete filePtr;
            return status;
        }
        // Insert into the mapping table
        status = openFiles.insert(fileName, filePtr);
    }
    return status;
}


// Close a database file. Get file info from open files table,
// call Unix close() only if open count now goes to zero.

const Status DB::closeFile(File* &file) {
    //if (!dynamic_cast<File*>(file)) return BADFILEPTR;
    if (!file) return BADFILEPTR;

    // Close the file
    file->close();

    // If there are no remaining references to the file, then we should delete
    // the file object and remove it from the openFiles hash table.

    if (file->openCnt == 0) {
        if (openFiles.erase(file->fileName) != OK) return BADFILEPTR;
        delete file;
    }
    return OK;
}