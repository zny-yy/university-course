/* 
 * File:   print.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include <stdio.h>
#include "catalog.h"


#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

int countChineseChar(char* rec) {
    int chineseCharNum = 0;
    char oneByte = rec[0];
    int i = 0;
    while (oneByte !=0) {
        if (oneByte < 0){  // 遇到了汉字
            i += 3; // 一个Utf8汉字占3个字节，所以，就前移3个字节
            chineseCharNum++;
        }
        else
            i += 1;
        oneByte = rec[i];
    }

    return chineseCharNum;

}


//
// 计算每个输出字段的列宽度
//

const Status UT_computeWidth(const int attrCnt,
        const AttrDesc attrs[],
        int *&attrWidth) {

    for (int i = 0; i < attrCnt; i++) {
        int namelen = strlen(attrs[i].attrName);
        switch (attrs[i].attrType) {
            case INTEGER:
                attrWidth[i] = MAX(namelen, 10);
            case FLOAT:
                attrWidth[i] = MAX(namelen, 10);
                break;
            case STRING:
                attrWidth[i] = MAX(namelen, attrs[i].attrLen);
                break;
        }
    }

    return OK;
}

/*
 * 输出一条记录
 * 函数的参数含义：
 * rec      ：记录存放缓存区的指针
 * attrCnt  ：字段数量（列数量）
 * attrs[]  ：每个字段的描述信息
 * attrWidth：每个字段的实际输出宽度
 */

void UT_printRec(const int attrCnt, const AttrDesc attrs[], int* attrWidth, const Record &rec) {
    int tempi;
    float tempf;
    for (int i = 0; i < attrCnt; i++) {
        char temp[attrs[i].attrLen + 1];
        memset(temp, 0, attrs[i].attrLen + 1);

        switch (attrs[i].attrType) {
            case INTEGER:
                memcpy(&tempi, (char *) rec.data + attrs[i].attrOffset, sizeof (int));
                printf("%-*d  ", attrWidth[i], tempi);
                break;
            case FLOAT:
                memcpy(&tempf, (char *) rec.data + attrs[i].attrOffset, sizeof (float));
                printf("%-*.2f  ", attrWidth[i], tempf);
                break;
            default:
                memcpy(temp, (char *) rec.data + attrs[i].attrOffset, attrs[i].attrLen);
                int chineseCharNo = countChineseChar(temp);
                //printf("%d", chineseCharNo);
                
                printf("%-*.*s  ", attrWidth[i]  + chineseCharNo, attrWidth[i], temp);
                break;
        }
        if (i < (attrCnt - 1))
            printf("\t");
    }

    printf("\n");
}

/*
 * 输出指定表的内容
 * 返回值:
 * 	成功：OK
 *	否则：错误代码（error.h文件中声明的Status枚举值）
 */

const Status UT_Print(string relation) {
    Status status;
    RelDesc rd;
    AttrDesc *attrs;
    int attrCnt;

    if (relation.empty()) relation = RELCATNAME;

    int *attrWidth;

    RID rid;
    Record rec;
    HeapFileScan *hfs;

    // 检查relcat确保表是存在的
    status = relCat->getInfo(relation, rd);
    if (status == FILEEOF) return RELNOTFOUND;
    if ((status != OK) && (status != FILEEOF)) return status;

    // 检查attrcat确保表的字段是存在的
    status = attrCat->getRelInfo(relation, attrCnt, attrs);
    if (status != OK) return status;
    attrWidth = new int [attrCnt];
    status = UT_computeWidth(attrCnt, attrs, attrWidth);
    if (status != OK) return status;

    // 输出表头
    cout << endl;
    for (int i = 0; i < attrCnt; i++) {
        printf("%-*.*s  ", attrWidth[i], attrWidth[i], attrs[i].attrName);
        if (i < (attrCnt - 1))
            printf("\t");
    }
    printf("\n");
    for (int i = 0; i < attrCnt; i++) {
        string str1(attrWidth[i], '-');
        printf("%-*.*s  ", attrWidth[i], attrWidth[i], str1.c_str());
        if (i < (attrCnt - 1))
            printf("\t");
    }
    printf("\n");


    hfs = new HeapFileScan(relation, status);
    if (status != OK) return status;

    hfs->startScan(0, 0, STRING, NULL, EQ);
    if (status != OK) return status;

    // 输出记录
    int j = 0;
    while ((status = hfs->scanNext(rid)) != FILEEOF) {
        if (status != OK) {
            hfs->endScan();
            delete hfs;
            delete [] attrs;
            return status;
        }
        status = hfs->getRecord(rec);
        if (status != OK) {
            hfs->endScan();
            delete hfs;
            delete [] attrs;
            return status;
        }
        UT_printRec(attrCnt, attrs, attrWidth, rec);
        j++;
    }
    cout << endl;
    cout << "共计输出" << j << "条记录!" << endl;

    hfs->endScan();
    delete hfs;
    delete [] attrs;

    return OK;
}
