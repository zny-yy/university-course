/* 
 * File:   update.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "query.h"

/*
 * 更新指定关系（表）中的一条记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Update(const string & relation,
        const int attrCnt,
        const attrInfo attrList[],
        const string & attrName,
        const Operator op,
        const Datatype type,
        const char *attrValue)
 {
    if (relation.empty()) return BADCATPARM;

    Status status;
    int itemp, iitemp;
    float ftemp, fftemp;
    RelDesc rd;
    AttrDesc ad;

    //Check the relation
    status = relCat->getInfo(relation, rd);
    if (status != OK) return status;

    if (attrValue) {
        status = attrCat->getInfo(relation, attrName, ad);
        if (status != OK) return status;
    }

    AttrDesc updateattrs[attrCnt];
    for (int i = 0; i < attrCnt; i++) {
        status = attrCat->getInfo(attrList[i].relName, attrList[i].attrName, updateattrs[i]);
        if (status != OK) return status;
    }

    RID rid;

    Record rec;
    //rec.data = &buffer;
    //rec.length=reclen;

    HeapFileScan * scan;

    scan = new HeapFileScan(relation, status);
    if (status != OK) return status;

    if (attrValue) {
        switch (ad.attrType) {
            case INTEGER:
                //Convert char* to Integer
                itemp = atoi(attrValue);
                scan->startScan(ad.attrOffset, ad.attrLen, (Datatype) ad.attrType, (char*) &itemp, op);
                break;
            case FLOAT:
                //Convert char* to Float
                ftemp = atof(attrValue);
                scan->startScan(ad.attrOffset, ad.attrLen, (Datatype) ad.attrType, (char*) &ftemp, op);
                break;
            case STRING:
                scan->startScan(ad.attrOffset, ad.attrLen, (Datatype) ad.attrType, attrValue, op);
                break;
        }
        if (status != OK) return status;
    } else {
        scan->startScan(0, 0, STRING, NULL, EQ);
    }
    
    int j = 0;
    while ((status = scan->scanNext(rid)) != FILEEOF) {
        if (status != OK) return status;

        status = scan->getRecord(rec);
        //memcpy(&buffer,(char*)rec.data,reclen);
        if (status != OK) return status;
        for (int i = 0; i < attrCnt; i++) {
            switch (updateattrs[i].attrType) {
                case INTEGER:
                    //Convert char* to Integer
                    iitemp = atoi((char*) attrList[i].attrValue);
                    memcpy((char*) rec.data + updateattrs[i].attrOffset, &iitemp, updateattrs[i].attrLen);
                    break;
                case FLOAT:
                    //Convert char* to Float
                    fftemp = atof((char*) attrList[i].attrValue);
                    memcpy((char*) rec.data + updateattrs[i].attrOffset, &fftemp, updateattrs[i].attrLen);
                    break;
                case STRING:
                    memcpy((char*) rec.data + updateattrs[i].attrOffset, attrList[i].attrValue, updateattrs[i].attrLen);
                    break;
            }
        }
        //rec.data=&buffer;
        status = scan->updateRecord(rec);
        if (status != OK) return status;
        j++;
    }

    scan->endScan();
    delete scan;
    scan = NULL;
    cout << "成功修改" << j << "条记录！" << endl;
    return OK;
}
