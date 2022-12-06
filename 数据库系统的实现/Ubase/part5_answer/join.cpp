/* 
 * File:   join.cpp
 * Author: 张太红
 * 
 * Created on 2015年11月29日, 下午1:13
 */

#include "catalog.h"
#include "query.h"
#include "sort.h"

extern JoinType JoinMethod;

const int matchRec(const Record & outerRec,
        const Record & innerRec,
        const AttrDesc & attrDesc1,
        const AttrDesc & attrDesc2);

/*
 * Joins two relations.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

// implementation of nested loops join goes here

const Status QU_NL_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    Status status;
    cout << "执行嵌套循环连接" << endl;
    AttrDesc pNames[projCnt];
    AttrDesc lattr; //left attribute in where clause
    AttrDesc rattr; //right attribute in where clause
    int reclen = 0; //length of the result record

    //convert projNames[] to pNames[projCnt] by attrCat->getInfo,
    //ie. from attrInfo to AttrDesc
    for (int i = 0; i < projCnt; i++) {
        status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, pNames[i]);
        if (status != OK) return status;
        reclen += pNames[i].attrLen;
    }

    //buffer to held temprory newrec.data
    char buffer[reclen];


    //convert *attr to sattrDesc by attrCat->getInfo,
    //ie. from attrInfo to AttrDesc
    status = attrCat->getInfo(attr1->relName, attr1->attrName, lattr);
    if (status != OK) return status;
    status = attrCat->getInfo(attr2->relName, attr2->attrName, rattr);
    if (status != OK) return status;

    char filter[lattr.attrLen];
    Record outerrec, innerrec, newrec;
    RID rid;

    HeapFileScan * outerscan;
    outerscan = new HeapFileScan(lattr.relName, status);
    if (status != OK) return status;

    HeapFileScan * innerscan;
    //innerscan = new HeapFileScan(rattr.relName,status);
    //if (status != OK) return status;

    InsertFileScan * iscan;
    iscan = new InsertFileScan(result, status);
    newrec.length = reclen;

    //unconditional outer relation scan,
    outerscan->startScan(0, 0, STRING, NULL, EQ);
    if (status != OK) return status;
    int i;
    int j = 0; //calculate the number of records which will be selected

    while ((status = outerscan->scanNext(rid)) != FILEEOF) {
        if (status != OK) return status;
        status = outerscan->getRecord(outerrec);
        if (status != OK) return status;

        memcpy(filter, (char*) outerrec.data + lattr.attrOffset, lattr.attrLen);
        //cout << "filter:" << filter << endl;
        innerscan = new HeapFileScan(rattr.relName, status);
        if (status != OK) return status;

        innerscan->startScan(rattr.attrOffset, rattr.attrLen, (Datatype) rattr.attrType, filter, op);
        while ((status = innerscan->scanNext(rid)) != FILEEOF) {
            if (status != OK) return status;
            status = innerscan->getRecord(innerrec);
            if (status != OK) return status;
            //project operation
            int offset = 0; //offset of each field(attribute) which will be projected
            for (i = 0; i < projCnt; i++) {
                //cout << "relation name:" << pNames[i].relName << endl;
                //cout << "field name:" << pNames[i].attrName << endl;
                if (strcmp(pNames[i].relName, lattr.relName) == 0) {
                    memcpy(buffer + offset, (char*) outerrec.data + pNames[i].attrOffset, pNames[i].attrLen);
                    //memcpy((char*)newrec.data+offset,(char*)outerrec.data+pNames[i].attrOffset,pNames[i].attrLen);
                } else {
                    memcpy(buffer + offset, (char*) innerrec.data + pNames[i].attrOffset, pNames[i].attrLen);
                    //memcpy((char*)newrec.data+offset,(char*)innerrec.data+pNames[i].attrOffset,pNames[i].attrLen);
                }
                offset += pNames[i].attrLen;
            }
            //insert the record into result table;
            newrec.data = &buffer;
            status = iscan->insertRecord(newrec, rid);
            if (status != OK) return status;
            j++;
        }
        innerscan->endScan();
        delete innerscan;

    }
    outerscan->endScan();
    delete outerscan;

    //innerscan->endScan();
    //delete innerscan;

    delete iscan;

    return OK;
}

// implementation of sort merge join goes here

const Status QU_SM_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    //Status status;
    cout << "Doing SM Join" << endl;
    // part 5

    return OK;
}


// implementation of hash join goes here

const Status QU_Hash_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    //Status status;
    cout << "Doing Hash Join" << endl;
    // part5

    return OK;
}

const Status QU_Join(const string & result,
        const int projCnt,
        const attrInfo projNames[],
        const attrInfo *attr1,
        const Operator op,
        const attrInfo *attr2) {
    switch (JoinMethod) {
        case NLJoin:
            return QU_NL_Join(result, projCnt, projNames, attr1, op, attr2);
            break;
        case SMJoin:
            QU_SM_Join(result, projCnt, projNames, attr1, op, attr2);
            break;
        case HashJoin:
            QU_Hash_Join(result, projCnt, projNames, attr1, op, attr2);
            break;
    }
    return OK;
}

const int matchRec(const Record & outerRec,
        const Record & innerRec,
        const AttrDesc & attrDesc1,
        const AttrDesc & attrDesc2) {
    int tmpInt1, tmpInt2;
    float tmpFloat1, tmpFloat2;

    switch (attrDesc1.attrType) {
        case INTEGER:
            memcpy(&tmpInt1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof (int));
            memcpy(&tmpInt2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof (int));
            return tmpInt1 - tmpInt2;

        case FLOAT:
            memcpy(&tmpFloat1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof (float));
            memcpy(&tmpFloat2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof (float));
            return int(tmpFloat1 - tmpFloat2);

        case STRING:
            return strcmp((char *) outerRec.data + attrDesc1.attrOffset,
                    (char *) innerRec.data + attrDesc2.attrOffset);
    }

    return 0;
}
