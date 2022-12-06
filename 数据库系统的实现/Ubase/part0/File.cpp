/* 
 * File:   File.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月4日, 下午4:18
 */

#include "File.h"

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
    if (::lseek(unixFile, pageNo * sizeof (Page), SEEK_SET) == -1) return UNIXERR;
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
    } 
    else{
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
        //if (bufMgr)
        //   bufMgr->flushFile(this);

        if (::close(unixFile) < 0)
            return UNIXERR;
    }
    return OK;
}
