/* 
 * File:   delete.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "query.h"

/*
 * 删除指定关系（表）中的一条记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Delete(const string &relation,
        const string &attrName,
        const Operator op,
        const Datatype type,
        const char *attrValue) {
    if (relation.empty()) return BADCATPARM;

    Status status;
    int itemp;
    float ftemp;
    AttrDesc ad;
    status = attrCat->getInfo(relation, attrName, ad);
    if (status != OK) return status;

    //Record rec;
    RID rid;
    HeapFileScan* scan;

    scan = new HeapFileScan(relation, status);
    if (status != OK) return status;

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
    //scan->startScan(ad.attrOffset,ad.attrLen,type,attrValue,op);
    if (status != OK) return status;
    int i = 0;
    while ((status = scan->scanNext(rid)) != FILEEOF) {
        if (status != OK){
            delete scan;
            return status;
        }
        status = scan->deleteRecord();
        if (status != OK){
            delete scan;
            return status;
        }
        i++;
    }

    scan->endScan();
    delete scan;
    scan = NULL;
    cout << endl;
    cout << "成功删除" << i << "条记录！" << endl;
    return OK;
}
