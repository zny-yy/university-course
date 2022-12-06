/* 
 * File:   main.cpp
 * Author: 张太红
 *
 * Created on 2015年11月5日, 上午11:31
 */
#include <stdio.h>   // printf()
#include "Page.h"

using namespace std;

int main() {
    Error error;
    Page* page1;
    short points = 0;


    //声明记录内容
    char rec1[33];
    char rec2[58];
    char rec3[71];
    char rec4[100];
    char rec5[4000];
    char rec3_new[71];

    // 声明记录
    Record record1, record2, record3, record, record4, record5;
    Record record3_new;
    Status status;
    RID rid1, rid2, rid3, rid4, rid5, tmpRid1, tmpRid2;

    int i, j;

    // 组装记录
    record1.data = &rec1;
    record1.length = sizeof (rec1);
    record2.data = &rec2;
    record2.length = sizeof (rec2);
    record3.data = &rec3;
    record3.length = sizeof (rec3);
    record4.data = &rec4;
    record4.length = sizeof (rec4);
    record5.data = &rec5;
    record5.length = sizeof (rec5);
    record3_new.data = &rec3_new;
    record3_new.length = sizeof (rec3_new);

    // 填充记录内容
    for (i = 0; i < 33; i++) rec1[i] = 'a';
    for (i = 0; i < 58; i++) rec2[i] = 'b';
    for (i = 0; i < 71; i++) rec3[i] = 'c';
    for (i = 0; i < 100; i++) rec4[i] = 'd';
    for (i = 0; i < 4000; i++) rec5[i] = 'e';
    for (i = 0; i < 71; i++) rec3_new[i] = 'X';

    cout << "Page类测试结果：" << endl;
    // 创建一个新页
    page1 = new Page;
    page1->init(1);
    ASSERT(page1->getFreePtr() == 0);
    ASSERT(page1->getFreeSpace() == 4080);
    ASSERT(page1->getSlotCnt() == 0)
    //page1->dumpPage();


    // 插入3条记录
    printf("插入3条记录，记录长度分别为33、58、71字节\n");
    CALL(page1->insertRecord(record1, rid1));
    ASSERT(page1->getFreePtr() == 33);
    ASSERT(page1->getFreeSpace() == 4043);
    ASSERT(page1->getSlotCnt() == -1);
    ASSERT(rid1.pageNo == 1);
    ASSERT(rid1.slotNo == 0);
    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    CALL(page1->getRecord(rid1, record));
    ASSERT(memcmp(record.data, rec1, record.length) == 0);
    //page1->dumpPage();

    CALL(page1->insertRecord(record2, rid2));
    ASSERT(page1->getFreePtr() == 91);
    ASSERT(page1->getFreeSpace() == 3981);
    ASSERT(page1->getSlotCnt() == -2);
    ASSERT(rid2.pageNo == 1);
    ASSERT(rid2.slotNo == 1);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == 33);
    ASSERT(page1->getLength(-1) == 58);

    CALL(page1->getRecord(rid2, record));
    ASSERT(memcmp(record.data, rec2, record.length) == 0);
    //page1->dumpPage();

    CALL(page1->insertRecord(record3, rid3));
    ASSERT(page1->getFreePtr() == 162);
    ASSERT(page1->getFreeSpace() == 3906);
    ASSERT(page1->getSlotCnt() == -3);
    ASSERT(rid3.pageNo == 1);
    ASSERT(rid3.slotNo == 2);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == 33);
    ASSERT(page1->getLength(-1) == 58);
    ASSERT(page1->getOffset(-2) == 91);
    ASSERT(page1->getLength(-2) == 71);

    CALL(page1->getRecord(rid3, record));
    ASSERT(memcmp(record.data, rec3, record.length) == 0);

    //page1->dumpPage();
    points += 10;
    cout << "测试【1】顺利通过！本项目你可能得到" << points << "分。" << endl;


    printf("\n现在扫描本页中的所有记录\n");
    // 先取出第一条记录的rid
    CALL(page1->firstRecord(tmpRid1));
    ASSERT(tmpRid1.pageNo == 1);
    ASSERT(tmpRid1.slotNo == 0);

    for (i = 0; i < 3; i++) {
        // 取记录的内容
        CALL(page1->getRecord(tmpRid1, record));
        cout << "Record " << i << ":";
        for (j = 0; j < record.length; j++)
            printf("%c", ((char *) record.data)[j]);
        printf("\n");

        // 取下一条记录的rid
        status = page1->nextRecord(tmpRid1, tmpRid2);
        if (i == 2) {
            ASSERT(status == ENDOFPAGE);
            break;
        }
        ASSERT(status == OK);
        tmpRid1 = tmpRid2;
    }
    points += 10;
    cout << "测试【2】顺利通过！本项目你可能得到" << points << "分。" << endl;

    printf("\n删除中间的记录，其rid2=%d.%d\n", rid2.pageNo, rid2.slotNo);
    CALL(page1->deleteRecord(rid2));

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == -1);
    ASSERT(page1->getLength(-1) == -1);
    ASSERT(page1->getOffset(-2) == 33);
    ASSERT(page1->getLength(-2) == 71);

    ASSERT(page1->getFreePtr() == 104);
    ASSERT(page1->getFreeSpace() == 3964);
    ASSERT(page1->getSlotCnt() == -3);
    CALL(page1->getRecord(rid3, record));
    ASSERT(memcmp(record.data, rec3, record.length) == 0);

    page1->dumpPage();

    points += 10;
    cout << "测试【3】顺利通过！本项目你可能得到" << points << "分。" << endl;


    printf("\n插入记录4，其长度为100字节\n");
    CALL(page1->insertRecord(record4, rid4));
    ASSERT(page1->getFreePtr() == 204);
    ASSERT(page1->getFreeSpace() == 3864);
    ASSERT(page1->getSlotCnt() == -3);
    ASSERT(rid4.pageNo == 1);
    ASSERT(rid4.slotNo == 1);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == 104);
    ASSERT(page1->getLength(-1) == 100);
    ASSERT(page1->getOffset(-2) == 33);
    ASSERT(page1->getLength(-2) == 71);

    CALL(page1->getRecord(rid4, record));
    ASSERT(memcmp(record.data, rec4, record.length) == 0);
    page1->dumpPage();

    points += 10;
    cout << "测试【4】顺利通过！本项目你可能得到" << points << "分。" << endl;

    printf("\n尝试插入一条本页无法容纳下的超长记录\n");
    FAIL(status = page1->insertRecord(record5, rid5));
    ASSERT(page1->getFreePtr() == 204);
    ASSERT(page1->getFreeSpace() == 3864);
    ASSERT(page1->getSlotCnt() == -3);
    ASSERT(rid5.pageNo == -1);
    ASSERT(rid5.slotNo == -1);
    printf("status=%d, rid returned = %d.%d\n", status, rid5.pageNo, rid5.slotNo);
    ASSERT(status == NOSPACE)
    cout << "对于超长记录，能够正确识别本页没有足够的空间" << endl;
    //page1->dumpPage();

    printf("本页剩余字节数为%d\n", page1->getFreeSpace());

    points += 10;
    cout << "测试【5】顺利通过！本项目你可能得到" << points << "分。" << endl;

    // 缩小记录的长度，再次尝试插入
    record5.length = page1->getFreeSpace() - sizeof (slot_t);
    printf("\n再次尝试插入一条长记录，本页剩余空间刚好等容纳该记录\n");
    CALL(status = page1->insertRecord(record5, rid5));
    ASSERT(page1->getFreePtr() == 4064);
    ASSERT(page1->getFreeSpace() == 0);
    ASSERT(page1->getSlotCnt() == -4);
    ASSERT(rid5.pageNo == 1);
    ASSERT(rid5.slotNo == 3);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == 104);
    ASSERT(page1->getLength(-1) == 100);
    ASSERT(page1->getOffset(-2) == 33);
    ASSERT(page1->getLength(-2) == 71);
    ASSERT(page1->getOffset(-3) == 204);
    ASSERT(page1->getLength(-3) == record5.length);

    CALL(page1->getRecord(rid5, record));
    ASSERT(memcmp(record.data, rec5, record.length) == 0);
    printf("status=%d, 返回的rid5 = %d.%d\n", status, rid5.pageNo, rid5.slotNo);
    //page1->dumpPage();

    cout << "由于我们已经删除了1条记录，事实上，现在本页共有4条记录" << endl;

    points += 10;
    cout << "测试【6】顺利通过！本项目你可能得到" << points << "分。" << endl;

    cout << endl << "逐条删除记录" << endl;
    printf("删除 rid3=%d.%d\n", rid3.pageNo, rid3.slotNo);
    page1->dumpPage();
    CALL(page1->deleteRecord(rid3));
    page1->dumpPage();
    ASSERT(page1->getFreePtr() == 3993);
    ASSERT(page1->getFreeSpace() == 71);
    ASSERT(page1->getSlotCnt() == -4);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == 33);
    ASSERT(page1->getLength(-1) == 100);
    ASSERT(page1->getOffset(-2) == -1);
    ASSERT(page1->getLength(-2) == -1);
    ASSERT(page1->getOffset(-3) == 133);
    ASSERT(page1->getLength(-3) == record5.length);

    //page1->dumpPage();

    // 删除 rec4
    printf("删除 rid4=%d.%d\n", rid4.pageNo, rid4.slotNo);
    CALL(page1->deleteRecord(rid4));
    ASSERT(page1->getFreePtr() == 3893);
    ASSERT(page1->getFreeSpace() == 171);
    ASSERT(page1->getSlotCnt() == -4);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == -1);
    ASSERT(page1->getLength(-1) == -1);
    ASSERT(page1->getOffset(-2) == -1);
    ASSERT(page1->getLength(-2) == -1);
    ASSERT(page1->getOffset(-3) == 33);
    ASSERT(page1->getLength(-3) == record5.length);

    //page1->dumpPage();

    printf("删除 rid5=%d.%d\n", rid5.pageNo, rid5.slotNo);
    CALL(page1->deleteRecord(rid5));
    ASSERT(page1->getFreePtr() == 33);
    ASSERT(page1->getFreeSpace() == 4043);
    ASSERT(page1->getSlotCnt() == -1)

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == -1);
    ASSERT(page1->getLength(-1) == -1);
    ASSERT(page1->getOffset(-2) == -1);
    ASSERT(page1->getLength(-2) == -1);
    ASSERT(page1->getOffset(-3) == -1);
    ASSERT(page1->getLength(-3) == -1);

    //page1->dumpPage();

    // 删除第一条记录
    printf("删除 rid1=%d.%d\n", rid1.pageNo, rid1.slotNo);
    CALL(page1->deleteRecord(rid1));
    ASSERT(page1->getFreePtr() == 0);
    ASSERT(page1->getFreeSpace() == 4080);
    ASSERT(page1->getSlotCnt() == 0);

    ASSERT(page1->getOffset(0) == -1);
    ASSERT(page1->getLength(0) == -1);
    ASSERT(page1->getOffset(-1) == -1);
    ASSERT(page1->getLength(-1) == -1);
    ASSERT(page1->getOffset(-2) == -1);
    ASSERT(page1->getLength(-2) == -1);
    ASSERT(page1->getOffset(-3) == -1);
    ASSERT(page1->getLength(-3) == -1);

    //page1->dumpPage();
    FAIL(status = page1->deleteRecord(rid1));
    ASSERT(status == INVALIDSLOTNO);
    FAIL(status = page1->firstRecord(rid1));
    ASSERT(status == NORECORDS);
    FAIL(status = page1->getRecord(rid1, record1));
    ASSERT(status == INVALIDSLOTNO);

    points += 10;
    cout << "测试【7】顺利通过！本项目你可能得到" << points << "分。" << endl;

    cout << endl << "测试firstRecord函数" << endl;
    CALL(page1->insertRecord(record1, rid1));
    CALL(page1->insertRecord(record2, rid2));
    CALL(page1->insertRecord(record3, rid3));
    CALL(page1->insertRecord(record4, rid4));
    CALL(page1->firstRecord(tmpRid1));
    ASSERT(tmpRid1.pageNo == 1);
    ASSERT(tmpRid1.slotNo == 0);
    CALL(page1->deleteRecord(rid1));
    CALL(page1->firstRecord(tmpRid1));
    ASSERT(tmpRid1.pageNo == 1);
    ASSERT(tmpRid1.slotNo == 1);
    CALL(page1->deleteRecord(rid2));
    CALL(page1->firstRecord(tmpRid1));
    ASSERT(tmpRid1.pageNo == 1);
    ASSERT(tmpRid1.slotNo == 2);
    CALL(page1->deleteRecord(rid3));
    CALL(page1->firstRecord(tmpRid1));
    ASSERT(tmpRid1.pageNo == 1);
    ASSERT(tmpRid1.slotNo == 3);
    CALL(page1->insertRecord(record1, rid1));
    CALL(page1->firstRecord(tmpRid1));
    ASSERT(tmpRid1.pageNo == 1);
    ASSERT(tmpRid1.slotNo == 0);
    page1->dumpPage();
    CALL(page1->deleteRecord(rid4));
    CALL(page1->deleteRecord(rid1));
    ASSERT(page1->firstRecord(tmpRid1) == NORECORDS);

    points += 10;
    cout << "测试【8】顺利通过！本项目你可能得到" << points << "分。" << endl;

    cout << endl << "再插入2条记录" << endl;
    CALL(page1->insertRecord(record1, rid1));
    ASSERT(page1->getFreePtr() == 33);
    ASSERT(page1->getFreeSpace() == 4043);
    ASSERT(page1->getSlotCnt() == -1);
    ASSERT(rid1.pageNo == 1);
    ASSERT(rid1.slotNo == 0);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);


    CALL(page1->insertRecord(record3, rid3));
    ASSERT(page1->getFreePtr() == 104);
    ASSERT(page1->getFreeSpace() == 3968);
    ASSERT(page1->getSlotCnt() == -2);
    ASSERT(rid3.pageNo == 1);
    ASSERT(rid3.slotNo == 1);

    ASSERT(page1->getOffset(0) == 0);
    ASSERT(page1->getLength(0) == 33);
    ASSERT(page1->getOffset(-1) == 33);
    ASSERT(page1->getLength(-1) == 71);

    page1->dumpPage();
    delete page1;
    
    points += 10;
    cout << "测试【9】顺利通过！本项目你可能得到" << points << "分。" << endl << endl;

    
    cout << "测试插满记录之后再插入记录" << endl; 
    char rec[100];
    for (i = 0; i < 100; i++) rec[i] = 'z';
    Record myRecord;
    myRecord.data = &rec;
    myRecord.length = sizeof (rec);
    RID rid;
    Page* page = new Page;
    page->init(1);
    for (int i = 1; i <= 39; i++) {
        CALL(page->insertRecord(myRecord, rid));
    }
    cout << "39条记录成功插入" << endl;
    ASSERT(page->insertRecord(myRecord, rid) == NOSPACE);
    cout << "能够正确判断页满" << endl; 
    delete page;

    points += 10;
    cout << endl << "测试【10】顺利通过！本项目你可能得到" << points << "分。" << endl;

    cout << endl << "恭喜，你的代码通过了所有测试！" << endl;
    return (1);
}
