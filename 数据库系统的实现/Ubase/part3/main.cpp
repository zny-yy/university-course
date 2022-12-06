#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>  // for exit()
#include <errno.h>
#include "Heapfile.h"

extern Status createHeapFile(string FileName);  // 声明(在其他文件中定义的)全局函数
extern Status destroyHeapFile(string FileName);

// 定义全局变量
DB db;
BufferManager* bufMgr;



struct Data{    
	  int i;
	  float f;
	  char c[30];	
}record1;	

char record2[30]={'a','a','a','a','a','a','a'};



int main(int argc, char **argv) {
    cout << "堆文件（关系）接口测试..." << endl << endl;

    struct stat statusBuf;
    int points = 0;
    Error error;
    HeapFile* file1;
    HeapFileScan *scan1, *scan2;
    InsertFileScan *iScan;
    Status status;
    int i, j;
    int num = 10000;
    RID newRid, rec2Rid;
    RID* ridArray;

    ridArray = new RID[num];

    typedef struct {
        int i;
        float f;
        char s[64];
    } RECORD;

    RECORD rec1, rec2;
    int rec1Len;
    Record dbrec1, dbrec2;

    bufMgr = new BufferManager(100);

    cout << "删除堆文件（关系）table1，测试destroyHeapFile函数" << endl;

    lstat("table1", &statusBuf);
    if (errno == ENOENT) {
        errno = 0;
        ASSERT(destroyHeapFile("table1") != OK);
    } else {
        CALL(destroyHeapFile("table1"));
    }
    points += 5;
    cout << "测试1通过！本周项目你可能得到" << points << "分。" << endl << endl;

    cout << "创建堆文件（关系）table1,测试createHeapFile函数的功能是否正确：" << endl;
    status = createHeapFile("table1");
    ASSERT(status == OK);
    lstat("table1", &statusBuf);
    ASSERT(statusBuf.st_size == 12288);
    points += 5;
    cout << "测试2通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 用空格填充rec1.s
    memset(rec1.s, ' ', sizeof (rec1.s));

    cout << "向堆文件（关系）table1插入" << num << "条记录" << endl;

    cout << "启动插入扫描器，将改变打开的table1文件" << endl;
    iScan = new InsertFileScan("table1", status);
    
    for (i = 0; i < num; i++) {
        sprintf(rec1.s, "This is record %05d", i);
        rec1.i = i;
        rec1.f = i;

        dbrec1.data = &rec1;
        dbrec1.length = sizeof (RECORD);
        status = iScan->insertRecord(dbrec1, newRid);

        // 保存记录的rid
        ridArray[i] = newRid;

        if (status != OK) {
            cout << "插入记录时出错" << endl;
            cout << "发生错误之前，已经插入" << i << "条记录" << endl;
            error.print(status);
            exit(1);
        }
    }
    delete iScan; // 销毁扫描器，同时关闭文件
    file1 = NULL;

    lstat("table1", &statusBuf);
    ASSERT(statusBuf.st_size == 782336);
    cout << "向堆文件（关系）table1插入" << num << "成功" << endl;
    points += 5;
    cout << "测试3通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 顺序扫描文件，检查每条记录存储无误
    cout << "扫描堆文件（关系）table1" << endl;
    scan1 = new HeapFileScan("table1", status);
    ASSERT(status == OK);

    scan1->startScan(0, 0, STRING, NULL, EQ);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        // reconstruct record i
        sprintf(rec1.s, "This is record %05d", i);
        rec1.i = i;
        rec1.f = i;
        status = scan1->getRecord(dbrec2);
        ASSERT(status == OK);
        ASSERT(memcmp(&rec1, dbrec2.data, sizeof (RECORD)) == 0);
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "扫描堆文件（关系）table1得到" << i << "条记录" << endl;
    ASSERT(i == num);
    // 删除扫描器对象
    scan1->endScan();
    delete scan1;
    scan1 = NULL;
    points += 5;
    cout << "测试4通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 不使用扫描器，而是用file->getRecord()取出记录号能被7整除的所有记录
    cout << "用file->getRecord()取出记录号能被7整除的所有记录" << endl;
    file1 = new HeapFile("table1", status); // 打开文件
    ASSERT(status == OK);
    // 取出记录号能被7整除的所有记录
    for (i = 0; i < num; i = i + 7) {
        // 为了对比，重新构造记录
        sprintf(rec1.s, "This is record %05d", i);
        rec1.i = i;
        rec1.f = i;

        // 读记录
        // printf("getting record (%d.%d)\n",ridArray[i].pageNo, ridArray[i].slotNo);
        status = file1->getRecord(ridArray[i], dbrec2);
        if (status != OK) {
            error.print(status);
            exit(1);
        }
        // 对比
        if (memcmp(&rec1, dbrec2.data, sizeof (RECORD)) != 0) {
            cout << "第" << i << "记录读出的内容和以前存入的不相同" << endl;
            exit(1);
        }
    }
    cout << "getRecord()测试成功通过" << endl;
    delete file1; // 关闭文件
    delete [] ridArray;
    points += 5;
    cout << "测试5通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 接着扫描文件，删除所有单数记录
    cout << "扫描堆文件（关系）table1文件，删除所有单数记录" << endl;

    scan1 = new HeapFileScan("table1", status);
    ASSERT(status == OK);
    scan1->startScan(0, 0, STRING, NULL, EQ);
    i = 0;
    int deleted = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        ASSERT(status == OK);
        if ((i % 2) != 0) {
            //printf("deleting record %d with rid(%d.%d)\n",i,rec2Rid.pageNo, rec2Rid.slotNo);
            status = scan1->deleteRecord();
            if (status != OK) //&& ( status != NORECORDS))
            {
                cout << "使用deleteRecord删除记录时出错" << endl;
                error.print(status);
                exit(1);
            }
            deleted++;
        }
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "共计删除" << deleted << "条记录" << endl;
    if (deleted != num / 2) {
        cout << "!!! 应当删除" << num / 2 << "条记录!" << endl;
        exit(1);
    }
    scan1->endScan();
    delete scan1;
    scan1 = NULL;
    points += 5;
    cout << "测试6通过！本周项目你可能得到" << points << "分。" << endl << endl;


    cout << "扫描文件，统计并删除剩余记录数量" << endl;
    scan1 = new HeapFileScan("table1", status);
    ASSERT(status == OK);
    i = 0;
    scan1->startScan(0, 0, STRING, NULL, EQ);
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        if (i > 0) // 第一条记录不删除
        {
            status = scan1->deleteRecord();
            if ((status != OK) && (status != NORECORDS)) {
                cout << "使用deleteRecord删除记录时出错" << endl;
                error.print(status);
                exit(1);
            }
        }
        i++;
    }
    // 减掉没有删除的1条记录
    i--;
    ASSERT(status == FILEEOF);
    scan1->endScan();
    delete scan1;
    scan1 = NULL;

    scan1 = new HeapFileScan("table1", status);
    ASSERT(status == OK);
    scan1->startScan(0, 0, STRING, NULL, EQ);

    // 删除剩余的记录（应该只有1条)
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        status = scan1->deleteRecord();
        if ((status != OK) && (status != NORECORDS)) {
            cout << "使用deleteRecord删除记录时出错" << endl;
            error.print(status);
            exit(1);
        }
        i++;
    }

    scan1->endScan();
    delete scan1;
    cout << "扫描器共计得到" << i << "条记录，并将它们全部删除" << endl;
    if (i != (num + 1) / 2) {
        cout << "!!! 扫描器应当看到到 " << (num + 1) / 2 << "条记录" << endl;
        exit(1);
    }
    status = destroyHeapFile("table1");
    if (status != OK) {
        cerr << "运行destroyHeapFile时出错" << endl;
        error.print(status);
        exit(1);
    }
    points += 5;
    cout << "测试7通过！本周项目你可能得到" << points << "分。" << endl << endl;


    destroyHeapFile("table2");
    status = createHeapFile("table2");
    if (status != OK) {
        cerr << "运行createHeapFile时出错" << endl;
        error.print(status);
        exit(1);
    }
    iScan = new InsertFileScan("table2", status);
    if (status != OK) {
        cout << "运行new insertFileScan时出错" << endl;
        error.print(status);
        exit(1);
    }

    cout << "向table2插入" << num << "条变长记录" << endl;
    int smallest, largest;
    for (i = 0; i < num; i++) {
        // 随机生成记录的第三个字段长度（2+ 0-61）即，长度为2到63的一个随机数字
        rec1Len = 2 + rand() % (sizeof (rec1.s) - 2);
        //cout << "record length is " << rec1Len << endl;
        memset((void *) rec1.s, 32 + rec1Len, rec1Len - 1);
        //cout << (char*) rec1.s << endl;
        rec1.s[rec1Len - 1] = 0;
        // 记录的总长度：4+4+2~63，即10~71
        rec1Len += sizeof (rec1.i) + sizeof (rec1.f);
        rec1.i = i;
        rec1.f = rec1Len;
        dbrec1.data = &rec1;
        dbrec1.length = rec1Len;

        status = iScan->insertRecord(dbrec1, newRid);
        if (status != OK) {
            cout << "使用insertrecord插入记录时出错" << endl;
            error.print(status);
            exit(1);
        }
        if (i == 0 || rec1Len < smallest) smallest = rec1Len;
        if (i == 0 || rec1Len > largest) largest = rec1Len;
    }
    cout << "向堆文件（关系）table2插入" << num << "条变长记录成功" << endl;
    cout << "其中最短记录为" << smallest << "字节，最长记录为" << largest << "字节" << endl;

    delete iScan;
    file1 = NULL;
    points += 5;
    cout << "测试8通过！本周项目你可能得到" << points << "分。" << endl << endl;


    cout << "扫描堆文件（关系）table2（前半部分）" << endl;
    j = num / 2;
    scan1 = new HeapFileScan("table2", status);
    ASSERT(status == OK);
    scan1->startScan(0, sizeof (int), INTEGER, (char*) &j, LT);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        status = scan1->getRecord(dbrec2);
        if (status != OK) break;
        memcpy(&rec2, dbrec2.data, dbrec2.length);
        if (rec2.i >= j || rec2.f != dbrec2.length || rec2.s[0] != 32 + dbrec2.length - 8)
            cout << "读取第" << i << "条记录时出错" << endl;
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "扫描器总共扫描到" << i << "记录" << endl;
    if (i != num / 2) {
        cout << "!!! 扫描器应当扫描到" << num / 2 << "条记录" << endl;
        exit(1);
    }
    scan1->endScan();
    delete scan1;
    scan1 = NULL;
    points += 5;
    cout << "测试9通过！本周项目你可能得到" << points << "分。" << endl << endl;


    cout << "接着对table2尝试启动两个扫描器同时并发扫描" << endl;
    int Ioffset = (char*) &rec1.i - (char*) &rec1;
    //cout << Ioffset << "," << (char*) &rec1.i << ","  << (char*) &rec1 << endl;
    int Ivalue = num / 2;
    int Foffset = (char*) &rec1.f - (char*) &rec1;
    //cout << Foffset << "," << (char*) &rec1.f << ","  << (char*) &rec1 << endl;
    float Fvalue = 0;

    // 第1个扫描器
    scan1 = new HeapFileScan("table2", status);
    ASSERT(status == OK);

    // 过滤条件为第1个整型字段的值小于num/2（5000）
    status = scan1->startScan(Ioffset, sizeof (int), INTEGER, (char*) &Ivalue, LT);
    ASSERT(status == OK);

    // 第2个扫描器
    scan2 = new HeapFileScan("table2", status);
    ASSERT(status == OK);

    // 过滤条件为第2个浮点型字段的值大于等于0
    status = scan2->startScan(Foffset, sizeof (float), FLOAT, (char*) &Fvalue, GTE);
    ASSERT(status == OK);
    int count = 0;
    for (i = 0; i < num; i++) {
        status = scan1->scanNext(rec2Rid);
        if (status == OK) {
            count++;
        } else if (status != FILEEOF) {
            error.print(status);
            exit(1);
        }
        status = scan2->scanNext(rec2Rid);
        if (status == OK) {
            count++;
        } else if (status != FILEEOF) {
            error.print(status);
            exit(1);
        }
    }
    cout << "两个扫描器总共扫描到" << count << "记录" << endl;
    if (count != num / 2 + num) {
        cout << "!!! 两个扫描器应当扫描到" << num / 2 + num << "条记录!" << endl;
        exit(1);
    }
    if (scan1->scanNext(rec2Rid) != FILEEOF) {
        cout << "!!!第1个扫描器抵达文件尾部但没有返回FILEEOF状态!" << endl;
        exit(1);
    }
    if (scan2->scanNext(rec2Rid) != FILEEOF) {
        cout << "!!! 第2个扫描器抵达文件尾部但没有返回FILEEOF状态!" << endl;
        exit(1);
    }
    delete scan1;
    delete scan2;
    scan1 = scan2 = NULL;

    cout << "删除table2" << endl;
    if ((status = destroyHeapFile("table2")) != OK) {
        cout << "删除table2时遇到错误" << endl;
        error.print(status);
        exit(1);
    }
    points += 5;
    cout << "测试10通过！本周项目你可能得到" << points << "分。" << endl << endl;


    destroyHeapFile("table3");
    status = createHeapFile("table3");

    if (status != OK) {
        cerr << "用createHeapFile创建table3时出错" << endl;
        error.print(status);
        exit(1);
    }

    iScan = new InsertFileScan("table3", status);
    if (status != OK) {
        cerr << "调用new insertFileScan时出错" << endl;
        error.print(status);
        exit(1);
    }
    cout << "向table3插入" << num << "条记录" << endl;
    for (i = 0; i < num; i++) {
        sprintf(rec1.s, "This is record %05d", i);
        rec1.i = i;
        rec1.f = i;

        dbrec1.data = &rec1;
        dbrec1.length = sizeof (RECORD);
        status = iScan->insertRecord(dbrec1, newRid);
        if (status != OK) {
            cout << "运行insertrecord时出错" << endl;
            error.print(status);
            exit(1);
        }
    }
    delete iScan;
    file1 = NULL;
    points += 5;
    cout << "测试11通过！本周项目你可能得到" << points << "分。" << endl << endl;

    int numDeletes = 0;
    cout << endl;
    cout << "现在扫描table3，删除第1001到第2000之间的所有记录" << endl;

    scan1 = new HeapFileScan("table3", status);
    ASSERT(status == OK);
    status = scan1->startScan(0, 0, STRING, NULL, EQ);
    ASSERT(status == OK);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        // reconstruct record i
        sprintf(rec1.s, "This is record %05d", i);
        rec1.i = i;
        rec1.f = i;
        status = scan1->getRecord(dbrec2);
        if (status != OK) break;
        if (memcmp(&rec1, dbrec2.data, sizeof (RECORD)) != 0) {
            cout << "第" << i << "读出的内容与之前写入的内容不符" << endl;
            exit(1);
        }
        if ((i > 1000) && (i <= 2000)) {
            status = scan1->deleteRecord();
            if ((status != OK) && (status != NORECORDS)) {
                cout << "运行deleteRecord时出错" << endl;
                error.print(status);
                exit(1);
            } else {
                numDeletes++;
            }
        }
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "扫描器得到了" << i << "条记录" << endl;
    if (i != num) {
        cout << "错误，扫描器应当扫描到" << (int) num << "条记录!!" << endl;
        exit(1);
    }
    cout << "扫描器总共删除了" << numDeletes << "条记录" << endl;
    if (numDeletes != 1000) {
        cout << "错误，应当删除1000条记录!!!" << endl;
        exit(1);
    }
    delete scan1;

    // 重新扫描，记录总数应当减少1000条
    scan1 = new HeapFileScan("table3", status);
    ASSERT(status == OK);
    scan1->startScan(0, 0, STRING, NULL, EQ);
    ASSERT(status == OK);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        i++;
    }
    cout << "经过删除后，扫到的记录应当减少1000条" << endl;
    cout << "实际扫描到" << i << "条记录" << endl;
    ASSERT(i == num - 1000);
    delete scan1;
    points += 5;
    cout << "测试12通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 执行过滤条件扫描 #1
    scan1 = new HeapFileScan("table3", status);
    ASSERT(status == OK);

    int filterVal1 = num * 3 / 4;
    cout << "过滤扫描，过滤条件为i字段GTE（大于等于）" << filterVal1 << endl;
    status = scan1->startScan(0, sizeof (int), INTEGER, (char *) &filterVal1, GTE);
    ASSERT(status == OK);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {

        status = scan1->getRecord(dbrec2);
        if (status != OK) break;

        // 验证扫描得到的记录确实满足条件
        RECORD* currRec = (RECORD*) dbrec2.data;
        if (!(currRec->i >= filterVal1)) {
            cerr << "!!返回的记录不满足过滤条件：i的值为" << currRec->i << endl;
            exit(1);
        }
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "扫描table3得到" << i << "满足条件的记录" << endl;
    if (i != num / 4) {
        cout << "!!!扫描器应当扫到" << num / 4 << "条记录!" << endl;
        exit(1);
    }
    delete scan1;
    points += 5;
    cout << "测试13通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 执行过滤条件扫描 #2
    scan1 = new HeapFileScan("table3", status);
    ASSERT(status == OK);
    float filterVal2 = num * 9 / 10;
    cout << "过滤扫描，过滤条件为f字段GT（大于）" << filterVal2 << endl;
    status = scan1->startScan(sizeof (int), sizeof (float), FLOAT, (char *) &filterVal2, GT);
    ASSERT(status == OK);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {

        status = scan1->getRecord(dbrec2);
        if (status != OK) break;

        // 验证扫描得到的记录确实满足条件
        RECORD *currRec = (RECORD *) dbrec2.data;
        if (!(currRec->f > filterVal2)) {
            cerr << "!!返回的记录不满足过滤条件：f的值为" << currRec->f << endl;
            exit(1);
        }
        i++;
    }
    if (status != FILEEOF) error.print(status);
    cout << "扫描table3得到" << i << "满足条件的记录" << endl;
    if (i != num / 10 - 1) {
        cout << "!!!!!!扫描器应当扫到" << num / 10 - 1 << "条记录!" << endl;
        exit(1);
    }
    delete scan1;
    points += 5;
    cout << "测试14通过！本周项目你可能得到" << points << "分。" << endl << endl;


    // 打开堆文件
    file1 = new HeapFile("table3", status);
    ASSERT(status == OK);
    delete file1;

    cout << "测试使用错误参数调用startScan" << endl;
    scan1 = new HeapFileScan("table3", status);
    ASSERT(status == OK);
    status = scan1->startScan(0, 20, INTEGER, "Hi", EQ);
    if (status == BADSCANPARM) {
        cout << "通过了BADSCANPARAM测试" << endl;
    } else {
        cout << "应当返回BADSCANPARM，而实际的返回为：" << endl;
        error.print(status);
        exit(1);
    }
    points += 5;
    cout << "测试15通过！本周项目你可能得到" << points << "分。" << endl << endl;


    cout << "尝试插入比页本身大的记录" << endl;
    iScan = new InsertFileScan("table3", status);
    ASSERT(status == OK);
    char bigdata[8192];
    sprintf(bigdata, "big record");
    dbrec1.data = (void *) &bigdata;
    dbrec1.length = 8192;
    status = iScan->insertRecord(dbrec1, rec2Rid);
    if ((status == INVALIDRECLEN) || (status == NOSPACE)) {
        cout << "通过了大记录插入测试" << endl;
    } else {
        cout << "插入大记录时出错" << endl;
        error.print(status);
        cout << "应当返回INVALIDRECLEN或NOSPACE" << endl;
        exit(1);
    }
    delete iScan;

    delete scan1;
    points += 5;
    cout << "测试16通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 在此可以编写更多的错误处理测试

    // 删除文件
    if ((status = destroyHeapFile("table3")) != OK) {
        cout << endl << "删除文件table3时出错" << endl;
        error.print(status);
        exit(1);
    }
    points += 5;
    cout << "测试17通过！本周项目你可能得到" << points << "分。" << endl << endl;

    typedef struct {
        char id[10]; // 学号，9位，最后1为存储0x0表示字符串结束
        char name[30]; // 姓名
        char phone[12]; // 电话
        int age; // 年龄
        float grade; // 平均成绩
    } STUDENT;

    STUDENT student;
    destroyHeapFile("students");
    status = createHeapFile("students");

    iScan = new InsertFileScan("students", status);
    if (status != OK) {
        cerr << "调用new insertFileScan时出错" << endl;
        error.print(status);
        exit(1);
    }
    cout << "向students插入3条记录" << endl;
    // 第1条
    memcpy(student.id, "124632231", 9);
    student.id[9] = 0;
    sprintf(student.name, "张寿德");
    memcpy(student.phone, "18146401185", 11);
    student.phone[11] = 0;
    student.age = 22;
    student.grade = 88.32;

    dbrec1.data = &student;
    dbrec1.length = sizeof (STUDENT);
    
    status = iScan->insertRecord(dbrec1, newRid);
    if (status != OK) {
        cout << "运行insertrecord时出错" << endl;
        error.print(status);
        exit(1);
    }

    // 第2条
    memcpy(student.id, "220130865", 9);
    student.id[9] = 0;
    sprintf(student.name, "温志强");
    memcpy(student.phone, "15894740776", 11);
    student.phone[11] = 0;
    student.age = 21;
    student.grade = 90.6;
    
    dbrec1.data = &student;
    dbrec1.length = sizeof (STUDENT);
    
    status = iScan->insertRecord(dbrec1, newRid);
    if (status != OK) {
        cout << "运行insertrecord时出错" << endl;
        error.print(status);
        exit(1);
    }

    // 第3条
    memcpy(student.id, "220130898", 9);
    student.id[9] = 0;
    memcpy((void*) student.name, "王聪慧子", 13);
    student.name[12] = 0;
    memcpy(student.phone, "13079955786", 11);
    student.phone[11] = 0;
    student.age = 21;
    student.grade = 90.6;
    
    dbrec1.data = &student;
    dbrec1.length = sizeof (STUDENT);
    
    status = iScan->insertRecord(dbrec1, newRid);
    if (status != OK) {
        cout << "运行insertrecord时出错" << endl;
        error.print(status);
        exit(1);
    }

    delete iScan;
    points += 5;
    cout << "测试18通过！本周项目你可能得到" << points << "分。" << endl << endl;

    cout << "现在扫描students，删除第2记录" << endl;

    scan1 = new HeapFileScan("students", status);
    ASSERT(status == OK);
    status = scan1->startScan(0, 0, STRING, NULL, EQ);
    ASSERT(status == OK);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        if ((i == 1)) {
            status = scan1->deleteRecord();
            if ((status != OK) && (status != NORECORDS)) {
                cout << "运行deleteRecord时出错" << endl;
                error.print(status);
                exit(1);
            }
        }
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "扫描器得到了" << i << "条记录" << endl;
    if (i != 3) {
        cout << "错误，扫描器应当扫描到3条记录!!" << endl;
        exit(1);
    }
    delete scan1;


    cout << "现在扫描students剩余的记录" << endl;
    scan1 = new HeapFileScan("students", status);
    ASSERT(status == OK);
    status = scan1->startScan(0, 0, STRING, NULL, EQ);
    ASSERT(status == OK);
    i = 0;
    while ((status = scan1->scanNext(rec2Rid)) != FILEEOF) {
        status = scan1->getRecord(dbrec2);
        if (status != OK) break;

        // 验证扫描得到的记录确实满足条件
        STUDENT* scanedRec = (STUDENT*) dbrec2.data;
        cout << scanedRec->id << endl;
        cout << scanedRec->name << endl;
        cout << scanedRec->phone << endl;
        cout << scanedRec->age << endl;
        cout << scanedRec->grade << endl;
        i++;
    }
    ASSERT(status == FILEEOF);
    cout << "扫描器得到了" << i << "条记录" << endl;
    if (i != 2) {
        cout << "错误，扫描器应当扫描到2条记录!!" << endl;
        exit(1);
    }
    delete scan1;
    points += 5;
    cout << "测试19通过！本周项目你可能得到" << points << "分。" << endl << endl;

    // 删除文件
    if ((status = destroyHeapFile("students")) != OK) {
        cout << endl << "删除文件students时出错" << endl;
        error.print(status);
        exit(1);
    }

    delete bufMgr;
    points += 5;
    cout << "测试20通过！本周项目你可能得到" << points << "分。" << endl << endl;


    cout << "恭喜，你的代码通过了所有测试！！！" << endl;
   
    return 1;

}