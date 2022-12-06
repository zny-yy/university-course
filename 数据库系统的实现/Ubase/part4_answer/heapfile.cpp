/* 
 * File:   Heapfile.cpp
 * Author: 张太红
 *
 * Created on 2015年11月4日, 下午3:58
 */
#include "heapfile.h"
#include "error.h"
#include <cstdlib>

// 创建关系（堆文件）的函数

const Status createHeapFile(const string fileName) {
    File* file;
    Status status;
    DirectoryPage* dirPage;
    int dirPageNo;
    int newPageNo;
    Page* newPage;

    // 创建新文件，文件只包含1页，即头页
    status = db.createFile(fileName);
    if (status != OK) return status;

    // 打开刚才创建的文件
    status = db.openFile(fileName, file);
    if (status != OK) return status;

    // 分配目录页，是物理文件的第2页，页编号为1
    status = bufMgr->allocPage(file, dirPageNo, newPage);
    if (status != OK) return status;

    //将该页从Page*类型强制转换为DirectoryPage*类型
    dirPage = reinterpret_cast<DirectoryPage*> (newPage);

    // 初始化目录页的各项属性
    memset(dirPage->fileName, 0, MAXNAMESIZE);
    memcpy(dirPage->fileName, fileName.c_str(), fileName.length());
    dirPage->recCnt = 0;
    dirPage->pageCnt = 0;
    for (int i = 0; i < DIRCNT; i++) {
        dirPage->pages[i].pageNo = -1;
        dirPage->pages[i].freeSpace = -1;
    }

    // 分配第1个数据页，实际是物理文件的第3页
    status = bufMgr->allocPage(file, newPageNo, newPage);
    if (status != OK) return status;
    newPage->init(newPageNo);
    dirPage->pageCnt = 1;
    dirPage->pages[0].pageNo = newPageNo;
    dirPage->pages[0].freeSpace = newPage->getFreeSpace();

    // 解除目录页摁钉
    status = bufMgr->unPinPage(file, dirPageNo, true);
    if (status != OK) return status;
    // 解除数据页摁钉
    status = bufMgr->unPinPage(file, newPageNo, true);
    if (status != OK) return status;

    // 关闭文件
    status = db.closeFile(file);
    if (status != OK)
        cout << "运行createHeapFile函数创建文件后，关闭文件时发生错误" << endl;

    return OK;
}

// 删除关系（堆文件）的函数

const Status destroyHeapFile(const string fileName) {
    return (db.destroyFile(fileName));
}

// 构造函数

HeapFile::HeapFile(const string &fileName, Status &returnStatus) {
    Status status;

    status = db.openFile(fileName, filePtr);
    if (status != OK) {
        returnStatus = status;
        exit(1);
    }

    status = filePtr->getFirstPage(directoryPageNo);
    if (status != OK) {
        returnStatus = status;
        exit(1);
    }
    status = bufMgr->readPage(filePtr, directoryPageNo, curPage);
    if (status != OK) {
        returnStatus = status;
        exit(1);
    }
    directoryPage = reinterpret_cast<DirectoryPage*> (curPage);
    directoryPageNo = 1;
    directoryDirtyFlag = false;

    curPageNo = directoryPage->pages[0].pageNo;

    status = bufMgr->readPage(filePtr, curPageNo, curPage);
    if (status != OK) {
        returnStatus = status;
    }

    curIdx = 0;
    curDirtyFlag = false;
    curRec = NULLRID;
    returnStatus = status;
}

// 析构函数要关闭文件

HeapFile::~HeapFile() {
    Status status;
    // 解除目录页摁钉
    status = bufMgr->unPinPage(filePtr, directoryPageNo, directoryDirtyFlag);
    if (status != OK) {
        cout << "HeapFile析构函数在解除目录页摁钉时出错！";
    }

    // 解除当前数据页摁钉
    if (curPage != NULL) {
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        if (status != OK) {
            cout << "HeapFile析构函数在解除当前数据页摁钉时出错！";
        }
    }

    // 关闭文件
    status = db.closeFile(filePtr);
    if (status != OK) {
        cout << "HeapFile析构函数在关闭文件时出错！";
    }
}


// 从堆文件获取任意记录
// 如果记录不在当前钉住的页面，当前页面需要解除摁钉，然后读入rid中pageNo代表的页面
// 通过参数rec返回记录的指针

const Status HeapFile::getRecord(const RID &rid, Record &rec) {
    Status status;

    if (rid.pageNo == curPageNo) {
        return curPage->getRecord(rid, rec);
    } else {
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        if (status != OK) return status;

        curPageNo = rid.pageNo;
        status = bufMgr->readPage(filePtr, curPageNo, curPage);
        if (status != OK) return status;

        curDirtyFlag = false;
        status = curPage->getRecord(rid, rec);
        curRec = rid;
        return status;
    }
}

const Status HeapFileScan::startScan(const int offset_,
        const int length_,
        const Datatype type_,
        const char* filter_,
        const Operator op_) {
    Status status;

    // 首先要保证文件的第一个数据页在缓存池中（因为要从头扫描）
    // 如果文件的第一个数据页不在缓存池中，解除当前页的摁钉，然后将文件的第一个数据页读入缓存池

    // 实现代码如下：

    if (curPageNo != directoryPage->pages[0].pageNo) {
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        if (status != OK) return status;
        curPageNo = directoryPage->pages[0].pageNo;
        status = bufMgr->readPage(filePtr, curPageNo, curPage);
        if (status != OK) return status;
        curDirtyFlag = false;
        curRec = NULLRID;
        curIdx = 0;
    }
    // 然后检查其它参数是否合法
    if (!filter_) { // 没有指定过滤条件
        filter = NULL;
        return OK;
    }

    if ((offset_ < 0 || length_ < 1) ||
            (type_ != STRING && type_ != INTEGER && type_ != FLOAT) ||
            (type_ == INTEGER && length_ != sizeof (int)) ||
            (type_ == FLOAT && length_ != sizeof (float)) ||
            (op_ != LT && op_ != LTE && op_ != EQ && op_ != GTE && op_ != GT && op_ != NE)) {
        return BADSCANPARM;
    }

    offset = offset_;
    length = length_;
    type = type_;
    filter = filter_;
    op = op_;

    return OK;
}

const Status HeapFileScan::markScan() {
    // 为扫描器建立快照标记
    markedPageNo = curPageNo;
    markedRec = curRec;
    markedIdx = curIdx;
    return OK;
}

const Status HeapFileScan::resetScan() {
    Status status;
    if (markedPageNo == curPageNo) return OK;

    status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
    if (status != OK) return status;

    status = bufMgr->readPage(filePtr, markedPageNo, curPage);
    if (status != OK) return status;

    curPageNo = markedPageNo;
    curDirtyFlag = false;
    curRec = markedRec;
    curIdx = markedIdx;

    return OK;
}

const Status HeapFileScan::scanNext(RID& outRid) {
    Status status = OK;
    RID nextRid;
    RID tmpRid;
    Record rec;

    // cout << "in ScanNext(), curPageNo is " << curPageNo << endl;
    while (true) {
        if (curRec.slotNo == -1) {
            status = curPage->firstRecord(tmpRid);
            if ((status == NORECORDS) && (directoryPage->pages[curIdx + 1].pageNo == -1)) {
                return FILEEOF;
            }
            if ((status == NORECORDS) && (directoryPage->pages[curIdx + 1].pageNo != -1)) {
                status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
                if (status != OK) return status;

                curPageNo = directoryPage->pages[curIdx + 1].pageNo;
                status = bufMgr->readPage(filePtr, curPageNo, curPage);
                if (status != OK) return status;
                curIdx++;
                curRec = NULLRID;
                continue;
            }

            if ((status != OK) && (status != NORECORDS)) return status;
            curRec = tmpRid;
        } else {
            status = curPage->nextRecord(curRec, nextRid);
            if (status == OK) {
                tmpRid = nextRid;
                curRec = tmpRid;
            }
            if ((status == ENDOFPAGE) && (directoryPage->pages[curIdx + 1].pageNo == -1))
                return FILEEOF;

            if ((status == ENDOFPAGE) && (directoryPage->pages[curIdx + 1].pageNo != -1)) {
                status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
                if (status != OK) return status;

                curPageNo = directoryPage->pages[curIdx + 1].pageNo;
                status = bufMgr->readPage(filePtr, curPageNo, curPage);
                if (status != OK) return status;
                curIdx++;
                curRec = NULLRID;
                continue;
            }
            if ((status != OK) && (status != ENDOFPAGE))
                return status;
        }
        status = getRecord(rec);
        if (status != OK) return status;

        if (matchRec(rec) == true) {
            outRid = curRec;
            break;
        }
    }
    return OK;
}


// 从文件删除记录 

const Status HeapFileScan::deleteRecord() {
    Status status;

    if (curRec.pageNo != curPageNo) return BADPAGENO;

    status = curPage->deleteRecord(curRec);
    if (status != OK) return status;

    status = markDirty();
    if (status != OK) return status;

    directoryPage->recCnt--;
    directoryDirtyFlag = true;

    return OK;
}

const Status HeapFileScan::updateRecord(const Record & rec) {

    Status status;

    if (curRec.pageNo != curPageNo) return BADPAGENO;

    status = curPage->updateRecord(rec, curRec);
    if (status != OK) return status;

    status = markDirty();
    if (status != OK) return status;

    return OK;
}

// 更新当前记录

InsertFileScan::~InsertFileScan() {
    Status status;
    // 解除扫描器钉住的最后一页
    if (curPage != NULL) {
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        curPage = NULL;
        curRec = NULLRID;
        curPageNo = 0;
        if (status != OK) cerr << "error in unpin of data page\n";
    }
}

// 向文件插入记录

const Status InsertFileScan::insertRecord(const Record & rec, RID& outRid) {
    Status status;
    RID rid;
    int prevPageNo;
    Page* prevPage;
    while (directoryPage->pages[curIdx].freeSpace < (rec.length + (int) sizeof (slot_t))) {
        prevPageNo = curPageNo;
        prevPage = curPage;
        if (directoryPage->pages[curIdx + 1].pageNo == -1) {
            status = bufMgr->allocPage(filePtr, curPageNo, curPage);
            if (status != OK) return status;

            prevPage->setNextPage(curPageNo);
            status = bufMgr->unPinPage(filePtr, prevPageNo, true);
            if (status != OK) return status;


            curPage->init(curPageNo);
            curRec = NULLRID;
            curIdx++;
            directoryPage->pageCnt++;
            directoryPage->pages[curIdx].pageNo = curPageNo;
            directoryPage->pages[curIdx].freeSpace = curPage->getFreeSpace();
            directoryDirtyFlag = true;
            break;
        } else {
            status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
            if (status != OK) return status;
            curPageNo = directoryPage->pages[curIdx + 1].pageNo;
            status = bufMgr->readPage(filePtr, curPageNo, curPage);
            if (status != OK) return status;
            curRec = NULLRID;
            curIdx++;
        }
    }
    status = curPage->insertRecord(rec, rid);
    if (status != OK) return status;
    directoryPage->pages[curIdx].freeSpace = curPage->getFreeSpace();
    curRec = rid;
    curDirtyFlag = true;
    directoryPage->recCnt++;
    directoryDirtyFlag = true;
    outRid = rid;

    return OK;
}
