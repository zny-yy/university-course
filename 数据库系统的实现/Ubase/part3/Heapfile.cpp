/*
 * 文件: Heapfile.cpp
 * 班级: XXXXXXXX
 * 学号: YYYYYYYY
 * 姓名: ZZZZZZZZ
 */

#include "Heapfile.h"
#include <cstdlib>
Error error;

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
    dirPage->pageCnt = 1;   //动画中采用的是初始值为0，此处为1，表示pageCnt代表数据页的页数
    dirPage->pages[0].pageNo = newPageNo;
    dirPage->pages[0].freeSpace = newPage->getFreeSpace();
    //cout << "新创建的堆文件pages[0].freeSpace=" << newPage->getFreeSpace() << endl;

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
    //cout << "HeapFile基类的构造函数被执行"<< endl;
    returnStatus = status;
}


// 析构函数要关闭文件

HeapFile::~HeapFile() {
    Status status;
    // 解除目录页摁钉
    status = bufMgr->unPinPage(filePtr, directoryPageNo, directoryDirtyFlag);
    if (status != OK) {
        error.print(status);
    }

    // 解除当前数据页摁钉
    if (curPage != NULL) {
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        if (status != OK) {
            error.print(status);
        }
    }

    // 关闭文件
    status = db.closeFile(filePtr);
    if (status != OK) {
        error.print(status);
    }
    //cout << "HeapFile基类的析构函数被执行"<< endl;
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
    /*
     *  在这里编写你的代码
	 *		 
	 */
        
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
    RID tmpRid;
    Record rec;
    while (true) {
        if (curRec.slotNo == -1) { //当前记录无效，必须获取当前页的第一条记录
            status = curPage->firstRecord(tmpRid);
        } else { //当前记录有效，必须获取当前记录的下一条记录
            status = curPage->nextRecord(curRec, tmpRid);
        }
        if (status == OK) {
            curRec = tmpRid;
            status = getRecord(rec);
            if (status != OK) return status;
            if (matchRec(rec) == true) { //记录符合过滤条件
                outRid = curRec;
                break;
            } else continue; //记录不符合过滤条件
        }
        if (status != NORECORDS && status != ENDOFPAGE) return status; // 出现错误
        
		// 到达文件尾部
        if ((status == NORECORDS || status == ENDOFPAGE) && ((directoryPage->pages[curIdx + 1].pageNo == -1) || curIdx + 1 == DIRCNT)) {
            return FILEEOF;
        }
        
        // 读下一页
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        if (status != OK) return status;       
        /*
         *  在这里编写你的代码
	     *		 
	     */
	     
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

	/*
     *  在这里编写你的代码
	 *		 
	 */

    return OK;
}


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
    cout << "InsertFileScan类的析构函数被执行" << endl;
}


// 向文件插入记录

const Status InsertFileScan::insertRecord(const Record &rec, RID &outRid) {
    Status status;
    outRid = curRec = NULLRID; // 出错时返回的outRid及保存的curRec
    
    for (status = curPage->insertRecord(rec, curRec); status == NOSPACE; status = curPage->insertRecord(rec, curRec)) {
        if (curIdx + 1 == DIRCNT) return FILEFULL; // 文件已满
        // 释放当前的满页
        status = bufMgr->unPinPage(filePtr, curPageNo, curDirtyFlag);
        if (status != OK) return status;
        curIdx++; // 目录项下移一个
        
        if (directoryPage->pages[curIdx].pageNo != -1) { // 下一页为有效数据页
            
		  /*
           *  在这里编写你的代码
	       *		 
	       */
            
            if (status != OK) return status;
            
        } else { // 无后续数据页，需要分配新页
            status = bufMgr->allocPage(filePtr, curPageNo, curPage);
            if (status != OK) return status;            
            curPage->init(curPageNo);
            
		   /*
            *  在这里编写你的代码
	        *		 
	        */
        }
    }
    if (status != OK) return status;    
    
    directoryPage->pages[curIdx].freeSpace = curPage->getFreeSpace();
    curDirtyFlag = true;
    directoryPage->recCnt++;
    directoryDirtyFlag = true;
    outRid = curRec;
    
    return OK;
}
