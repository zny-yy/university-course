/* 
 * File: main.cpp
 * 学号: 
 * 姓名:
 * 班级:
 */

#include <cstdlib>
#include <sys/stat.h>
#include <stdio.h>  //sprintf
#include "DB.h"

using namespace std;

// ubase IO层测试驱动程序
int main(int argc, char** argv) {
    // 1、测试Error类
    Error error;
    cout << "ubase错误信息一览表" << endl;
    cout << "-----------------------------------" << endl;
    cout << "代码\t" << "错误含义" << endl;
    cout << "-----------------------------------" << endl;
    for (int i = NOTUSED1; i <= NOTUSED2; i++) {
        cout << i << "\t";
        error.print((Status) i);
    }
    cout << "-----------------------------------" << endl;
    cout << "1、Error类通过了测试" << endl << endl;

    
    // 2、测试Page类
    
    Page page1;
    ASSERT(sizeof page1 == 4096);
    cout << "2、Page类通过了测试" << endl << endl;

    
	// 3、测试DB类
	
    DB db;
    string fileName = "test.1";
    CALL(db.createFile(fileName)); // 创建一个文件
    cout << "3.1、createFile方法创建新文件成功" << endl;

    int i = 0, j = 0;
    ASSERT(i == j);
    // 新创建的文件只有一页，因此其大小一定等于PAGESIZE
    int fd = ::open(fileName.c_str(), O_RDONLY);
    struct stat stat;
    fstat(fd, &stat);
    ASSERT(stat.st_size == PAGESIZE);
    ::close(fd);
    cout << "3.2、createFile方法新创建文件大小与PAGESIZE相同" << endl;

    ASSERT(db.createFile(fileName) == FILEEXISTS); // 创建一个已经存在的文件
    cout << "3.3、createFile可以检测到同名文件的存在" << endl;

    File* file1 = NULL;
    CALL(db.openFile(fileName, file1)); // 打开一个文件
    cout << "3.4、openFile打开文件成功" << endl;
    CALL(db.openFile(fileName, file1)); // 打开同一个文件
    cout << "3.5、openFile可以多次打开同一文件" << endl;

    CALL(db.closeFile(file1)); // 关闭已经打开的个文件
    cout << "3.6、closeFile关闭文件成功" << endl;
    CALL(db.closeFile(file1)); // 再次关闭已经打开的个文件
    cout << "3.7、closeFile第二次关闭同一文件成功" << endl;

    ASSERT(db.closeFile(file1) == BADFILEPTR); // 第三次关闭同一个文件
    cout << "3.8、closeFile对打开2次而关闭三次的特殊情形处理正确" << endl;

    File* file2 = NULL;
    ASSERT(db.closeFile(file2) == BADFILEPTR);
    cout << "3.9、closeFile可以处理空指针文件对象" << endl;

    CALL(db.destroyFile(fileName)); // 删除已经完全关闭的个文件
    cout << "3.10、destroyFile删除文件成功" << endl;

    ASSERT(db.destroyFile(fileName) == UNIXERR);
    cout << "3.11、destroyFile删除不存在的文件，处理正确" << endl;

    cout << "3、DB类通过了测试" << endl << endl;

    
	// 4、测试File类

    string fileName2 = "test.2";
    CALL(db.createFile(fileName2)); // 创建一个文件
    File* file3 = NULL;
    CALL(db.openFile(fileName2, file3)); // 打开一个文件
    int firstPage = 9999;
    CALL(file3->getFirstPage(firstPage));
    ASSERT(firstPage == -1);
    cout << "4.1、对于新创建的文件test.2，getFirstPage应当得到-1，正确" << endl;

    ASSERT(file3->getFileName() == fileName2);
    cout << "4.2、getFileName得到正确的文件名称" << endl;

    int pageNo = 0;
    CALL(file3->allocatePage(pageNo)); // 第一个数据页
    ASSERT(pageNo == 1);
    cout << "4.3、为test.2文件allocatePage分配一页成功" << endl;

    ASSERT(file3->disposePage(pageNo) == BADPAGENO);
    cout << "4.4、首个数据页不能被disposePage回收，正确" << endl;

    ASSERT(file3->disposePage(2) == BADPAGENO);
    cout << "4.5、超出的数据页不能被disposePage回收，正确" << endl;

    CALL(file3->allocatePage(pageNo));
    ASSERT(pageNo == 2);
    CALL(file3->allocatePage(pageNo));
    ASSERT(pageNo == 3);
    CALL(file3->allocatePage(pageNo));
    ASSERT(pageNo == 4);
    CALL(file3->allocatePage(pageNo));
    ASSERT(pageNo == 5);
    CALL(file3->allocatePage(pageNo));
    ASSERT(pageNo == 6);
    cout << "4.6、连续用allocatePage分配5页，正确" << endl;

    CALL(file3->disposePage(2));
    CALL(file3->disposePage(3));
    CALL(file3->disposePage(4));
    CALL(file3->disposePage(5));
    CALL(file3->disposePage(6));
    cout << "4.7、连续用disposePage回收5页，正确" << endl;

    //此时，文件test.2有1个头页+1个数据业+5个空白页组成
    file3->listFree();
    CALL(file3->allocatePage(pageNo));
    ASSERT(pageNo == 6);
    cout << "4.8、存在空白页是用allocatePage分配一页，正确" << endl;

    //此时，文件test.2有1个头页+2个数据业+4个空白页组成
    file3->listFree();

    const int num = 100; // 准备用allocatePage分配100页
    int pageNumber[num]; // 保存allocatePage分配得到的页号
    cout << "为文件test.2分配100个数据页..." << endl;

    Page* page = new Page;
    for (int i = 0; i < num; i++) {
        CALL(file3->allocatePage(pageNumber[i]));
    }
    cout << "4.9、为test.2用allocatePage分配100页，成功" << endl;

    for (int i = 0; i < num; i++) {
        memset(page, 0, PAGESIZE);
        snprintf((char*) page, PAGESIZE, "test.2 Page %d %7.1f", pageNumber[i], (float) pageNumber[i]);
        CALL(file3->writePage(pageNumber[i], page));
    }
    cout << "4.10、test.2用writePage写入刚分配的100页，成功" << endl;


    Page* pageReadOut = new Page;
    char cmp[PAGESIZE];
    for (int i = 0; i < num; i++) {
        memset(pageReadOut, 0, PAGESIZE);
        CALL(file3->readPage(pageNumber[i], pageReadOut));
        //cout << (char*) pageReadOut << endl;
        sprintf((char*) &cmp, "test.2 Page %d %7.1f", pageNumber[i], (float) pageNumber[i]);
        ASSERT(memcmp(pageReadOut, &cmp, strlen((char*) &cmp)) == 0);
    }
    cout << "4.11、从test.2用readPage读出刚写入的100页，成功" << endl;

    CALL(db.closeFile(file3)); // 关闭个文件test.2

    /*
     * 文件test.2的最终打小应当为:
     * 文件头页：1*PAGESIZE+
     * 两先前分配的数据页：2*PAGESIZE+
     * 循环连续分配的数据页：num*PAGESIZE
     */
    fileName = "test.2";
    fd = ::open(fileName.c_str(), O_RDONLY);
    fstat(fd, &stat);
    ASSERT(stat.st_size == PAGESIZE * (1 + 2 + num));
    ::close(fd);
    cout << "4.12、test.2文件的最终大小为" << PAGESIZE * (1 + 2 + num) << " 字节，正确" << endl;

    CALL(db.destroyFile("test.2")); // 删除文件test.2

    cout << "4、File类通过了测试" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "恭喜，你的代码通过了所有测试" << endl << endl;

    return 0;
}
