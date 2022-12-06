/* 
 * File:   insert.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "query.h"

/*
 * 向指定关系（表）插入一条记录
 *
 * 返回:
 * 	成功：OK
 * 	否则：错误代码
 */

const Status QU_Insert(const string & relation,
        const int attrCnt,
        const attrInfo attrList[]) {
    Status status;
    RelDesc rd;
    AttrDesc *ad;
    int attrno;

    //Check if the table exist
    status = relCat->getInfo(relation, rd);
    if (status != OK) return status;

    //Check the attributes of the table match the suplied parameters
    status = attrCat->getRelInfo(relation, attrno, ad);
    if (status != OK) return status;

    if (attrno != attrCnt) return ATTRNUMBERMISMATCH;

    //get record length
    int reclen = 0;
    for (int i = 0; i < attrno; i++)
        reclen += ad[i].attrLen;

    char buffer[reclen];
    Record dbrec;
    dbrec.length = reclen;
    int offset = 0;
    int j;

    //Reorder the value list to conform to the offsets specified for each attribute in the AttrCat table
    for (int i = 0; i < attrno; i++) {
        for (j = 0; j < attrCnt; j++) {
            if (strncmp(ad[i].attrName, attrList[j].attrName, ad[i].attrLen) == 0) break;
        }

        switch (ad[i].attrType) {
            case INTEGER:
                //Convert void* to Integer
                int itemp;
                itemp = atoi((char*) attrList[j].attrValue);
                memcpy(buffer + offset, &itemp, ad[i].attrLen);
                //memcpy((char*)dbrec.data+offset,&itemp,ad[i].attrLen);
                break;
            case FLOAT:
                //Convert void* to Float
                float ftemp;
                ftemp = atof((char*) attrList[j].attrValue);
                memcpy(buffer + offset, &ftemp, ad[i].attrLen);
                //memcpy((char*)dbrec.data+offset,&ftemp,ad[i].attrLen);
                break;
            case STRING:
                memcpy(buffer + offset, attrList[j].attrValue, ad[i].attrLen);
                //memcpy((char*)dbrec.data+offset,attrList[j].attrValue,ad[i].attrLen);
                break;
        }

        offset += ad[i].attrLen;
    }
    //Insert the record
    RID rid;
    InsertFileScan *iScan;
    iScan = new InsertFileScan(relation, status);
    dbrec.data = &buffer;
    status = iScan->insertRecord(dbrec, rid);
    if (status != OK) {
        delete iScan;
        return status;
    }
    cout << endl << "成功插入1条记录！" << endl;
    delete iScan;
    if (status != OK) return status;

    return OK;
}
