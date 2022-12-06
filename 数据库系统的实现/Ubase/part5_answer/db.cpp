/* 
 * File:   db.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "db.h"
#include "bufferManager.h"
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

Status OpenFileHashTable::insert(const string fileName, File* file) {
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
Status OpenFileHashTable::find(const string fileName, File*& file) {
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

Status OpenFileHashTable::erase(const string fileName) {
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

size_t OpenFileHashTable::hash(string fileName) {
    size_t len = (size_t) fileName.length();
    size_t value = 0;

    for (size_t i = 0; i < len; i++) {
        value = 31 * value + (size_t) fileName[i];
    }
    value = value % HTSIZE;
    return value;
}

/***************************************************************
 * 以下为文件对象File8个公有方法的具体实现，包括：
 * 1、构造函数：File()
 * 2、析构函数：~File()
 * 3、取数据首页页号：getFirstPage()
 * 4、取文件名称：getFileName()
 * 5、分配新数据页函数：allocatePage()
 * 6、读页函数：readPage()
 * 7、写页函数：writePage()
 * 8、回收空白页函数：disposePage(),将其加入空白页链表
 * 9、显示所有空白页函数：listFree()
 ***************************************************************/

// 1、构造函数

File::File(const string & fname) {
    fileName = fname;
    openCnt = 0;
    unixFile = -1;
}
// 2、析构函数

File::~File() {
    if (openCnt == 0) return; //文件已经正常关闭，无需再做什么

    // 否则表明文件仍然在打开，必须先将文件在缓存中已经发生变换的页写入磁盘，然后将其关闭
    // 为了保证以上操作，先把openCnt设置为1，然后调用close()方法
    openCnt = 1;
    Status status = close();
    if (status != OK) {
        Error error;
        error.print(status);
    }
}


// 3、返回第一个数据页的页号，即文件头页的firstPage属性

const Status File::getFirstPage(int &pageNo) const {
    Page header;
    Status status;

    if ((status = readPage(0, &header)) != OK) return status;

    pageNo = HP(header).firstPage;
    return OK;
}

// 4、返回文件名称

const string File::getFileName() {
    return fileName;
}

/* 
 * 5、分配一页
 * 如果有空白页（以前释放的空白页链表），取出第一个空白页
 * 否则为文件建在一页
 */
const Status File::allocatePage(int &pageNo) {
    Page header;
    Status status;

    if ((status = readPage(0, &header)) != OK) return status; // 读入头页

    if (HP(header).nextFree != -1) {
        // 如果有空白页(以前释放的空白页链表)，取出第一个空白页返回给调用者，同时调整空白页链表
        pageNo = HP(header).nextFree;
        Page firstFree;
        if ((status = readPage(pageNo, &firstFree)) != OK) return status;
        HP(header).nextFree = HP(firstFree).nextFree;
    } else {
        // 否则为文件新建一页返回给调用者，同时调整空白页链表
        pageNo = HP(header).numPages;
        Page newPage;
        memset(&newPage, 0, sizeof newPage);
        if ((status = writePage(pageNo, &newPage)) != OK) return status;
        HP(header).numPages++;
        if (HP(header).firstPage == -1) // 如果是首次创建的数据页?
            HP(header).firstPage = pageNo;
    }
    // 因为我们修改了头页，必须将其写回文件
    if ((status = writePage(0, &header)) != OK) return status;

    return OK;
}

// 6、从文件读一页，

const Status File::readPage(const int pageNo, Page* pagePtr) const {

    // 首先检查参数的合法性
    if (!pagePtr) return BADPAGEPTR;
    if (pageNo < 0) return BADPAGENO;

    // 指定读入的偏移位置
    if (::lseek(unixFile, pageNo * sizeof (Page), SEEK_SET) == -1)
        return UNIXERR;

    // 读入操作
    int nbytes = ::read(unixFile, (char*) pagePtr, sizeof (Page));

    if (nbytes != sizeof (Page))
        return UNIXERR;
    else
        return OK;
}


// 7、将内存中的一页写入文件

const Status File::writePage(const int pageNo, const Page* pagePtr) {
    if (!pagePtr) return BADPAGEPTR;
    if (pageNo < 0) return BADPAGENO;
    if (lseek(unixFile, pageNo * sizeof (Page), SEEK_SET) == -1) return UNIXERR;
    int nbytes = write(unixFile, (char*) pagePtr, sizeof (Page));

    if (nbytes != sizeof (Page)) return UNIXERR;
    else return OK;
}

// 8、回收空白页

const Status File::disposePage(const int pageNo) {
    if (pageNo < 1) return BADPAGENO;

    Page header;
    Status status;

    if ((status = readPage(0, &header)) != OK) return status;

    // 数据首页不应当被回收，一旦回收，File层并不知道文件的第二个数据页页号，
    // 也就是说，将无法调整头页的firstPage属性
    if (HP(header).firstPage == pageNo) return BADPAGENO;

    // 要回收的页号当然不能超过文件的总页数
    if (pageNo >= HP(header).numPages) return BADPAGENO;

    // 通过将该页添加到空白页链表的头部实现回收
    Page away;
    if ((status = readPage(pageNo, &away)) != OK) return status; //读入要回收的页
    memset(&away, 0, sizeof away); //清空要回收的页

    // 调整空白页链表
    HP(away).nextFree = HP(header).nextFree;
    HP(header).nextFree = pageNo;

    // 写回文件
    if ((status = writePage(pageNo, &away)) != OK) return status;
    if ((status = writePage(0, &header)) != OK) return status;

    return OK;
}

void File::listFree() {
    cout << "文件" << fileName << "的空白页链表：" << endl;
    cout << "nextFree";
    int pageNo = 0;
    while (true) {
        Page page;
        if (readPage(pageNo, &page) != OK) break;
        pageNo = HP(page).nextFree;
        cout << "->" << pageNo;
        if (pageNo == -1) break;
    }
    cout << endl;
}

// 文件对象的相等比较操作符的实现

bool File::operator==(const File & other) const {
    return fileName == other.fileName;
}

/***************************************************************
 * 以下为文件对象File的4个私有方法的具体实现，包括：
 * 1、创建文件函数：create()
 * 2、删除文件函数：destroy()
 * 3、打开文件函数：open()
 * 4、关闭文件函数：close()
 ***************************************************************/
Status const File::create(const string &fName) {
    int file;
    if ((file = ::open(fName.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0666)) < 0) {
        if (errno == EEXIST)
            return FILEEXISTS;
        else
            return UNIXERR;
    }

    // 新文件只包含一个DB头页
    Page header;
    memset(&header, 0, sizeof header);
    HP(header).nextFree = -1;
    HP(header).firstPage = -1;
    HP(header).numPages = 1;
    if (write(file, (char*) &header, sizeof header) != sizeof header)
        return UNIXERR;
    if (::close(file) < 0)
        return UNIXERR;

    return OK;
}

const Status File::destroy(const string &fileName) {
    if (::remove(fileName.c_str()) < 0) return UNIXERR;
    return OK;
}

const Status File::open() {
    if (openCnt == 0) {
        // 首次打开
        if ((unixFile = ::open(fileName.c_str(), O_RDWR)) < 0)
            return UNIXERR;

        openCnt = 1;
    } else {
        openCnt++;
    }

    return OK;
}

const Status File::close() {
    if (openCnt <= 0)
        return FILENOTOPEN;

    openCnt--;

    // 当打开次数为0时，真正关闭文件
    if (openCnt == 0) {
        if (bufMgr) {
            Status s = bufMgr->rejectFile(this);
            if (s != OK) return s;
        }
        if (::close(unixFile) < 0) return UNIXERR;
    }
    return OK;
}