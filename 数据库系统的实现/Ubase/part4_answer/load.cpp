/* 
 * File:   load.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "catalog.h"

//
// 将记录从标准二进制文件导入表中
//
// 返回值:
// 	成功：OK
// 	否则：错误代码（error.h文件中声明的Status枚举值）
//

const Status UT_Load(const string & relation, const string & fileName) {
    Status status;
    RelDesc rd;
    AttrDesc *attrs;
    int attrCnt;

    if (relation.empty() || fileName.empty() || relation == string(RELCATNAME)
            || relation == string(ATTRCATNAME))
        return BADCATPARM;

    // 打开Unix数据文件
    int fd;
    if ((fd = open(fileName.c_str(), O_RDONLY, 0)) < 0)
        return UNIXERR;

    //检查relcat确保表是存在的
    status = relCat->getInfo(relation, rd);
    if (status != OK) return status;

    //检查attrcat确保表的字段是存在的
    status = attrCat->getRelInfo(relation, attrCnt, attrs);
    if (status != OK) return status;

    //获取记录长度  
    int reclen = 0;
    for (int i = 0; i < attrCnt; i++)
        reclen += attrs[i].attrLen;

    InsertFileScan *iScan;
    RID rid;
    Record rec;
    rec.length = reclen;
    iScan = new InsertFileScan(relation, status);
    if (status != OK) return status;

    //逐行从文件导入记录
    char buffer[reclen];

    while (read(fd, buffer, reclen)) {
        rec.data = &buffer;
        status = iScan->insertRecord(rec, rid);
        if (status != OK) return status;
    }
    delete iScan;

    // 关闭Unix文件
    if (close(fd) < 0)
        return UNIXERR;
    else
        return OK;
}