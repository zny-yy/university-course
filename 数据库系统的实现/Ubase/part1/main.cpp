/* 
 * File:   main.cpp
 * Author: 张太红
 *
 * Created on 2015年11月5日, 上午11:31
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib> // exit()等函数

#include "BufferManager.h"

int main() {
    cout << "UBase缓存管理器测试结果：" << endl;
    cout << "------------------------------------------" << endl;
    struct stat statusBuf;
    Error error;
    const int num = 100; //缓存池大小为100帧
    BufferManager* bufMgr = new BufferManager(num); // 创建缓存管理器对象
    DB* db = new DB;
    File* file1;
    File* file2;
    File* file3;
    File* file4;
    int i;
    int points = 0;
    int j[num];


    //检查文件是否已经存在，如果已经存在，删除之
    lstat("test.1", &statusBuf);
    if (errno == ENOENT)
        errno = 0;
    else
        (void) db->destroyFile("test.1");

    lstat("test.2", &statusBuf);
    if (errno == ENOENT)
        errno = 0;
    else
        (void) db->destroyFile("test.2");

    lstat("test.3", &statusBuf);
    if (errno == ENOENT)
        errno = 0;
    else
        (void) db->destroyFile("test.3");

    lstat("test.4", &statusBuf);
    if (errno == ENOENT)
        errno = 0;
    else
        (void) db->destroyFile("test.4");

    //创建4个关系文件
    CALL(db->createFile("test.1"));
    ASSERT(db->createFile("test.1") == FILEEXISTS);
    CALL(db->createFile("test.2"));
    CALL(db->createFile("test.3"));
    CALL(db->createFile("test.4"));

    //打开4个关系文件
    CALL(db->openFile("test.1", file1));
    CALL(db->openFile("test.2", file2));
    CALL(db->openFile("test.3", file3));
    CALL(db->openFile("test.4", file4));

    // 测试缓冲管理器
    Page* page;
    Page* page2;
    Page* page3;
    char cmp[PAGESIZE];
    int pageno, pageno2, pageno3;

    cout << "为文件test.1分配100个数据页..." << endl;
    cout << "修改缓存中的100帧..." << endl;
    cout << "解除缓存中100帧的摁钉..." << endl;
    for (i = 0; i < num; i++) {
        CALL(bufMgr->allocPage(file1, j[i], page));
        sprintf((char*) page, "test.1 Page %d %7.1f", j[i], (float) j[i]);
        CALL(bufMgr->unPinPage(file1, j[i], true));
    }

    points += 10;
    cout << "恭喜：测试1顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "顺序读文件test.1的100个数据页..." << endl;
    cout << "比较读入的内容..." << endl;
    cout << "解除缓存中100帧的摁钉..." << endl;
    for (i = 0; i < num; i++) {
        CALL(bufMgr->readPage(file1, j[i], page));
        sprintf((char*) &cmp, "test.1 Page %d %7.1f", j[i], (float) j[i]);
        ASSERT(memcmp(page, &cmp, strlen((char*) &cmp)) == 0);
        CALL(bufMgr->unPinPage(file1, j[i], true));
    }
    //bufMgr->BufDump();
    points += 10;
    cout << "恭喜：测试2顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "为文件test.2、test.3分配33个数据页..." << endl;
    cout << "随即读test.1文件33页并比较读入的内容..." << endl;
    cout << "顺序读test.2、test.3文件33页并比较读入的内容..." << endl;
    cout << "解除缓存中test.1文件33帧的摁钉..." << endl;
    cout << "解除缓存中test.2、test.3文件33帧的摁钉..." << endl;
    for (i = 0; i < num / 3; i++) {
        CALL(bufMgr->allocPage(file2, pageno2, page2));
        sprintf((char*) page2, "test.2 Page %d %7.1f", pageno2, (float) pageno2);
        CALL(bufMgr->allocPage(file3, pageno3, page3));
        sprintf((char*) page3, "test.3 Page %d %7.1f", pageno3, (float) pageno3);
        pageno = j[random() % num];
        CALL(bufMgr->readPage(file1, pageno, page));
        sprintf(cmp, "test.1 Page %d %7.1f", pageno, (float) pageno);
        ASSERT(memcmp(page, &cmp, strlen((char*) &cmp)) == 0);
        CALL(bufMgr->readPage(file2, pageno2, page2));
        sprintf((char*) &cmp, "test.2 Page %d %7.1f", pageno2, (float) pageno2);
        ASSERT(memcmp(page2, &cmp, strlen((char*) &cmp)) == 0);
        CALL(bufMgr->readPage(file3, pageno3, page3));
        sprintf((char*) &cmp, "test.3 Page %d %7.1f", pageno3, (float) pageno3);
        ASSERT(memcmp(page3, &cmp, strlen((char*) &cmp)) == 0);
        CALL(bufMgr->unPinPage(file1, pageno, true));
    }

    for (i = 0; i < num / 3; i++) {
        CALL(bufMgr->unPinPage(file2, i + 1, true));
        CALL(bufMgr->unPinPage(file2, i + 1, true));
        CALL(bufMgr->unPinPage(file3, i + 1, true));
        CALL(bufMgr->unPinPage(file3, i + 1, true));
    }

    points += 10;
    cout << "恭喜：测试3顺利通过！本项目你可能得到" << points << "分。" << endl << endl;


    cout << "顺序读test.1文件33页并比较读入的内容..." << endl;
    cout << "解除缓存中test.1文件33帧的摁钉..." << endl;

    for (i = 1; i < num / 3; i++) {
        CALL(bufMgr->readPage(file1, i, page2));
        sprintf((char*) &cmp, "test.1 Page %d %7.1f", i, (float) i);
        ASSERT(memcmp(page2, &cmp, strlen((char*) &cmp)) == 0);
        CALL(bufMgr->unPinPage(file1, i, false));
    }

    points += 10;
    cout << "恭喜：测试4顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "顺序读test.2文件33页并比较读入的内容..." << endl;
    cout << "解除缓存中test.2文件33帧的摁钉..." << endl;

    for (i = 1; i < num / 3; i++) {
        CALL(bufMgr->readPage(file2, i, page2));
        sprintf((char*) &cmp, "test.2 Page %d %7.1f", i, (float) i);
        ASSERT(memcmp(page2, &cmp, strlen((char*) &cmp)) == 0);
        //cout << (char*)page2 << endl;
        CALL(bufMgr->unPinPage(file2, i, false));
    }

    points += 10;
    cout << "恭喜：测试5顺利通过！本项目你可能得到" << points << "分。" << endl << endl;


    cout << "顺序读test.3文件33页并比较读入的内容..." << endl;
    cout << "解除缓存中test.3文件33帧的摁钉..." << endl;

    for (i = 1; i < num / 3; i++) {
        CALL(bufMgr->readPage(file3, i, page3));
        sprintf((char*) &cmp, "test.3 Page %d %7.1f", i, (float) i);
        ASSERT(memcmp(page3, &cmp, strlen((char*) &cmp)) == 0);
        //cout << (char*)page3 << endl;
        CALL(bufMgr->unPinPage(file3, i, false));
    }

    points += 10;
    cout << "恭喜：测试6顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "读test.4文件的第一数据页" << endl;
    cout << "应当提示错误信息，因为test.4尚未分配数据页" << endl;

    Status status;
    FAIL(status = bufMgr->readPage(file4, 1, page));
    error.print(status);
    //bufMgr->BufDump();

    points += 10;
    cout << "恭喜：测试7顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "为test.4文件分配第一数据页" << endl;
    cout << "解除缓存中test.4文件第一数据页的摁钉..." << endl;
    cout << "第二次应当提示错误信息，因为缓存中test.4文件第一数据页的摁钉数已经为0" << endl;
    CALL(bufMgr->allocPage(file4, i, page));
    CALL(bufMgr->unPinPage(file4, i, true));
    FAIL(status = bufMgr->unPinPage(file4, i, false));
    error.print(status);
    //bufMgr->BufDump();

    points += 10;
    cout << "恭喜：测试8顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "为test.4文件再分配100个数据页" << endl;
    for (i = 0; i < num; i++) {
        CALL(bufMgr->allocPage(file4, j[i], page));
        sprintf((char*) page, "test.4 Page %d %7.1f", j[i], (float) j[i]);
    }
    cout << "在缓存中100个帧均被钉住的情况下，为test.4文件再分配1个数据页，应当出错" << endl;
    int tmp;
    FAIL(status = bufMgr->allocPage(file4, tmp, page));
    error.print(status);



    for (i = 0; i < num; i++)
        CALL(bufMgr->unPinPage(file4, i + 2, true));

    cout << "顺序读test.1文件100页并比较读入的内容..." << endl;


    for (i = 1; i < num; i++) {
        CALL(bufMgr->readPage(file1, i, page));
        sprintf((char*) &cmp, "test.1 Page %d %7.1f", i, (float) i);
        ASSERT(memcmp(page, &cmp, strlen((char*) &cmp)) == 0);
        //cout << (char*)page << endl;
    }

    points += 10;
    cout << "恭喜：测试9顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    cout << "未解除缓存中test.1文件100帧摁钉的情况下调用commit，应当出错..." << endl;
    FAIL(status = bufMgr->commit());
    error.print(status);

    for (i = 1; i < num; i++)
        CALL(bufMgr->unPinPage(file1, i, true));

    CALL(bufMgr->commit());


    // 回收31页
    int n = 0;
    for (i = 2; i < num / 3; i++) {
        CALL(bufMgr->disposePage(file1, i));
        CALL(bufMgr->disposePage(file2, i));
        CALL(bufMgr->disposePage(file3, i));
        CALL(bufMgr->disposePage(file4, i));
        n++;
    }
    cout << "每个文件回收" << n << "页" << endl;
    cout << "回收空白页后4个文件的大小：" << endl;
    lstat("test.1", &statusBuf);
    cout << "test.1 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 413696);

    lstat("test.2", &statusBuf);
    cout << "tes2.1 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 139264);

    lstat("test.3", &statusBuf);
    cout << "test.3 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 139264);

    lstat("test.4", &statusBuf);
    cout << "test.4 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 417792);

    
    cout << "每个文件再分配31页，应当利用空白页，因此文件的大小应当不变"  << endl;
    for (i = 2; i < num / 3; i++) {
        CALL(bufMgr->allocPage(file1, j[i], page));
        CALL(bufMgr->unPinPage(file1, j[i], true));

        CALL(bufMgr->allocPage(file2, j[i], page));
        CALL(bufMgr->unPinPage(file2, j[i], true));

        CALL(bufMgr->allocPage(file3, j[i], page));
        CALL(bufMgr->unPinPage(file3, j[i], true));
        CALL(bufMgr->allocPage(file4, j[i], page));
        CALL(bufMgr->unPinPage(file4, j[i], true));
    }
    
    cout << "再次分配后4个文件的大小：" << endl;
    lstat("test.1", &statusBuf);
    cout << "test.1 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 413696);

    lstat("test.2", &statusBuf);
    cout << "test.2 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 139264);

    lstat("test.3", &statusBuf);
    cout << "test.3 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 139264);

    lstat("test.4", &statusBuf);
    cout << "test.4 " << statusBuf.st_size << endl;
    ASSERT(statusBuf.st_size == 417792);

    CALL(db->closeFile(file1));
    CALL(db->closeFile(file2));
    CALL(db->closeFile(file3));
    CALL(db->closeFile(file4));

    CALL(db->destroyFile("test.1"));
    CALL(db->destroyFile("test.2"));
    CALL(db->destroyFile("test.3"));
    CALL(db->destroyFile("test.4"));

    delete db;
    delete bufMgr;

    points += 10;
    cout << "恭喜：测试10顺利通过！本项目你可能得到" << points << "分。" << endl << endl;
    cout << endl << "恭喜：所有测试顺利通过." << endl;
    cout << "------------------------------------------" << endl;
    return 0;
}