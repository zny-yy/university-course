/* 
 * File:   select.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "catalog.h"
#include "query.h"

// 扫描选择操作函数的声明，QU_Select将使用此函数完成查询操作
const Status ScanSelect(const string & result,
        const int projCnt,
        const AttrDesc projNames[],
        const AttrDesc *attrDesc,
        const Operator op,
        const char *filter,
        const int reclen);

/*
 * 从指定关系（表）查询记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Select(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr,
        const Operator op,
        const char *attrValue) {
    // Qu_Select sets up things and then calls ScanSelect to do the actual work
    // cout << "Doing QU_Select " << endl;
    Status status;
    AttrDesc sprojNames[projCnt];
    AttrDesc sattrDesc;
    int reclen = 0;
    //convert projNames[] to sprojNames[projCnt] by attrCat->getInfo,
    //ie. from attrInfo to AttrDesc
    for (int i = 0; i < projCnt; i++) {
        status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, sprojNames[i]);
        if (status != OK) return status;
        reclen += sprojNames[i].attrLen;
    }
    //if attrValue is not NULL,in this case,WHERE clause was supplied
    //convert *attr to sattrDesc by attrCat->getInfo,
    //ie. from attrInfo to AttrDesc
    if (attrValue) {
        status = attrCat->getInfo(attr->relName, attr->attrName, sattrDesc);
        if (status != OK) return status;

    }

    status = ScanSelect(result,
            projCnt,
            sprojNames,
            &sattrDesc,
            op,
            attrValue,
            reclen);
    if (status != OK) return status;
    return OK;
}

const Status ScanSelect(const string & result,
        const int projCnt,
        const AttrDesc projNames[],
        const AttrDesc *attrDesc,
        const Operator op,
        const char *filter,
        const int reclen) {
    //cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;
    Status status;
    Record rec, irec;
    RID rid;
    char buffer[reclen];

    int itemp;
    float ftemp;
    HeapFileScan * scan;
    scan = new HeapFileScan(projNames[0].relName, status);
    if (status != OK) return status;

    InsertFileScan * iscan;
    iscan = new InsertFileScan(result, status);
    irec.length = reclen;
    /*
        cout << attrDesc->attrOffset << endl;
        cout << attrDesc->attrLen << endl;
        cout << attrDesc->attrType << endl;
        cout << filter << endl;
        cout << op << endl;
     */
    if (filter) //if filter is not NULL,in this case,WHERE clause was supplied
    {
        switch (attrDesc->attrType) {
            case INTEGER:
                //int itemp;
                itemp = atoi(filter);
                scan->startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType, (char*) &itemp, op);
                break;
            case FLOAT:
                //float ftemp;
                ftemp = atof(filter);
                scan->startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType, (char*) &ftemp, op);
                break;
            case STRING:
                scan->startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType, filter, op);
                break;
        }
    } else {
        scan->startScan(0, 0, STRING, NULL, EQ);
    }
    if (status != OK) return status;
    //int j = 0; //calculate the number of records which will be selected
    //fetch records
    while ((status = scan->scanNext(rid)) != FILEEOF) {
        if (status != OK) return status;
        status = scan->getRecord(rec);
        if (status != OK) return status;
        //project operation
        int offset = 0; //offset of each field(attribute) which will be projected
        for (int i = 0; i < projCnt; i++) {
            memcpy(buffer + offset, (char*) rec.data + projNames[i].attrOffset, projNames[i].attrLen);
            //memcpy((char*)irec.data+offset,(char*)rec.data+projNames[i].attrOffset,projNames[i].attrLen);
            offset += projNames[i].attrLen;
        }
        //insert the record into result table;
        irec.data = &buffer;
        status = iscan->insertRecord(irec, rid);
        if (status != OK) return status;
        //j++;
    }

    scan->endScan();
    delete scan;
    delete iscan;

    /*
        if (j>0)
        {
            UT_Print(result);
        }
        cout << endl;
        cout << j << " record(s) selected!" << endl;
        if(result.c_str()=="Tmp_Minirel_Result")
        {
            destroyHeapFile(result);
        }
     */
    return OK;

}
